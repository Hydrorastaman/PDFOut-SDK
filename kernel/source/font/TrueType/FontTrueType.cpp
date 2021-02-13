#include <font/TrueType/FontTrueType.h>

#include <vector>
#include <numeric>
#include <algorithm>
#include <exception>

#include <font/CMapName.h>
#include <font/FontFileStream.h>
#include <font/ToUnicodeCMapStream.h>
#include <font/CIDSystemInfoDictionary.h>

#include <stream/InputStream.h>
#include <stream/OutputStream.h>
#include <stream/MemoryOStream.h>

#include <object/ObjectArray.h>
#include <object/ObjectNumber.h>
#include <object/ObjectInteger.h>
#include <object/ObjectRectangle.h>
#include <object/ObjectByteString.h>

#include <font/TrueType/Table.h>
#include <font/TrueType/TableHead.h>
#include <font/TrueType/TableHhea.h>
#include <font/TrueType/TableHmtx.h>
#include <font/TrueType/TableMaxp.h>
#include <font/TrueType/TableOS2.h>
#include <font/TrueType/TablePost.h>
#include <font/TrueType/TableName.h>
#include <font/TrueType/TableCmap.h>
#include <font/TrueType/TableLoca.h>
#include <font/TrueType/TableGlyf.h>
#include <font/TrueType/TableDttf.h>
#include <font/TrueType/TableKern.h>
#include <font/TrueType/TableCff.h>

#include <InternalTools.h>

#undef min
#undef max

namespace{
  /**
   * @brief TrueType scaler type
   */
  enum TrueTypeVersion{
    TrueTypeVersionTTF = 0x00010000, /**< TrueType fonts */
    TrueTypeVersionOTTO = 0x4F54544F /**< OpenType font with PostScript outlines (that is, a 'CFF ' table instead of a 'glyf' table) */
  };

  /**
   * @brief TrueType offset table
   */
  PACK(struct TrueTypeOffsetTable{
    int32_t mVersion;        /**< TrueType version */
    uint16_t mNumOfTables;   /**< Number of tables */
    uint16_t mSearchRange;   /**< (Maximum power of 2 <= numTables) * 16 */
    uint16_t mEntrySelector; /**< Log2(maximum power of 2 <= numTables) */
    uint16_t mRangeShift;    /**< NumTables * 16 - searchRange */
  });

  struct CIDtoGID{
    uint32_t mCid;
    uint16_t mGid;
    uint32_t mOffset;
  };

  TrueTypeOffsetTable *makeTrueTypeOffsetTable(uint16_t numOfTables);
  TrueTypeOffsetTable *loadTrueTypeOffsetTable(pdfout::InputStream *stream);
  void saveTrueTypeOffsetTable(pdfout::OutputStream *stream, TrueTypeOffsetTable *offsetTable);
}

using namespace kernel;
using namespace pdfout;

namespace kernel{ namespace font{ namespace truetype{

  FontTrueType::FontTrueType(pdfout::InputStream *stream)
    : SimpleFont(),
    mTablesOffset(),
    mTables(TrueTypeTableInvalid){

    if (!enumTables(stream) || !loadFontInfo(stream))
      RUNTIME_EXCEPTION("invalid TrueType font data");

    makeCIDMetrics();
  }

  FontTrueType::FontTrueType()
    : SimpleFont(),
    mTablesOffset(),
    mTables(TrueTypeTableInvalid),
    mCIDMetrics(){
  }

  FontTrueType::~FontTrueType(void) {}

  UString FontTrueType::getRevision(void) const{
    TableHead *head = getTableHead();
    if (!head)
      return UString("", Encoding_CHAR);

    char revision[MAX_ULONG_LENGTH + 3] = {0};
    sprintf(revision, "0x%X", head->mData.mFontRevision);

    return UString(revision, Encoding_CHAR);
  }

  UString FontTrueType::getPostScriptName(void) const{
    TableName *name = getTableName();
    if (!name)
      return UString("", Encoding_CHAR);

    return name->getPostScriptName();
  }

  UString FontTrueType::getFamilyName(void) const{
    TableName *name = getTableName();
    if (!name)
      return UString("", Encoding_CHAR);

    return name->getFamilyName();
  }

  UString FontTrueType::getSubFamilyName(void) const{
    TableName *name = getTableName();
    if (!name)
      return UString("", Encoding_CHAR);

    return name->getSubFamilyName();
  }

  void FontTrueType::saveToStream(pdfout::OutputStream *stream) const{
    struct TableInfo{
      TrueTypeTableRecord mRecord;
      std::unique_ptr<MemoryOStream> mStream;

      TableInfo(void) : mRecord(), mStream(nullptr) {}
      TableInfo(TableInfo const &) = delete;
      TableInfo &operator=(TableInfo &rsh){
        if (this != &rsh){
          mRecord = rsh.mRecord;
          mStream = std::move(rsh.mStream);
        }

        return *this;
      }
    };

    std::map<TrueTypeTable, TableInfo> tables;

    TableHead *head = getTableHead();
    if (head)
      head->mData.mCheckSumAdjustment = 0;

    TableInfo headInfo;
    headInfo.mStream.reset(saveTable(TrueTypeTableHead, headInfo.mRecord));
    if (headInfo.mStream)
      tables[TrueTypeTableHead] = std::move(headInfo);

    TableInfo hheaInfo;
    hheaInfo.mStream.reset(saveTable(TrueTypeTableHhea, hheaInfo.mRecord));
    if (hheaInfo.mStream)
      tables[TrueTypeTableHhea] = std::move(hheaInfo);

    TableInfo maxpInfo;
    maxpInfo.mStream.reset(saveTable(TrueTypeTableMaxp, maxpInfo.mRecord));
    if (maxpInfo.mStream)
      tables[TrueTypeTableMaxp] = std::move(maxpInfo);

    TableInfo os2Info;
    os2Info.mStream.reset(saveTable(TrueTypeTableOS2, os2Info.mRecord));
    if (os2Info.mStream)
      tables[TrueTypeTableOS2] = std::move(os2Info);

    TableInfo nameInfo;
    nameInfo.mStream.reset(saveTable(TrueTypeTableName, nameInfo.mRecord));
    if (nameInfo.mStream)
      tables[TrueTypeTableName] = std::move(nameInfo);

    TableInfo cmapInfo;
    cmapInfo.mStream.reset(saveTable(TrueTypeTableCmap, cmapInfo.mRecord));
    if (cmapInfo.mStream)
      tables[TrueTypeTableCmap] = std::move(cmapInfo);

    TableInfo hmtxInfo;
    hmtxInfo.mStream.reset(saveTable(TrueTypeTableHmtx, hmtxInfo.mRecord));
    if (hmtxInfo.mStream)
      tables[TrueTypeTableHmtx] = std::move(hmtxInfo);

    TableInfo glyfInfo;
    glyfInfo.mStream.reset(saveTable(TrueTypeTableGlyf, glyfInfo.mRecord));
    if (glyfInfo.mStream)
      tables[TrueTypeTableGlyf] = std::move(glyfInfo);

    TableInfo locaInfo;
    locaInfo.mStream.reset(saveTable(TrueTypeTableLoca, locaInfo.mRecord));
    if (locaInfo.mStream)
      tables[TrueTypeTableLoca] = std::move(locaInfo);

    TableInfo postInfo;
    postInfo.mStream.reset(saveTable(TrueTypeTablePost, postInfo.mRecord));
    if (postInfo.mStream)
      tables[TrueTypeTablePost] = std::move(postInfo);

    TableInfo kernInfo;
    kernInfo.mStream.reset(saveTable(TrueTypeTableKern, kernInfo.mRecord));
    if (kernInfo.mStream)
      tables[TrueTypeTableKern] =  std::move(kernInfo);

    uint64_t startOffset = stream->tellp();
    uint64_t recordOffset = startOffset + sizeof(TrueTypeOffsetTable);
    uint64_t tableOffset = recordOffset + tables.size() * sizeof(TrueTypeTableRecord);

    std::unique_ptr<TrueTypeOffsetTable> offsetTable(makeTrueTypeOffsetTable(tables.size()));
    saveTrueTypeOffsetTable(stream, offsetTable.get());

    Endian const endian = getEndian();

    //uint32_t checkSumAdjustment = 0;
    //for (auto &elem : tables)
    //  checkSumAdjustment += elem.second.mRecord.mCheckSum;

    //uint32_t magic = 0xB1B0AFBA;
    //swapByteOrder(magic);
    //checkSumAdjustment = magic - checkSumAdjustment;

    //char headTag[5] = {0};
    //tableIdToTag(TrueTypeTableHead, headTag);
    //tables[std::string(headTag)].mRecord.mCheckSum = checkSumAdjustment;

    for (auto &elem : tables){
      tableOffset += getPadTo4(tableOffset);
      elem.second.mRecord.mOffset = (uint32_t) tableOffset;
      if (endian == EndianLittle){
        swapByteOrder(elem.second.mRecord.mCheckSum);
        swapByteOrder(elem.second.mRecord.mOffset);
        swapByteOrder(elem.second.mRecord.mLength);
      }

      stream->seekp(recordOffset);
      stream->write(&elem.second.mRecord, sizeof(TrueTypeTableRecord), 1);
      recordOffset += sizeof(TrueTypeTableRecord);

      stream->seekp(tableOffset);
      uint64_t size = elem.second.mStream->getSize();
      stream->write(elem.second.mStream->getData(), sizeof(uint8_t), size);
      tableOffset += size;
    }

  }

  std::unique_ptr<kernel::ObjectIndirectReference> FontTrueType::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
    NOT_IMPLEMENTED();
    //std::unique_ptr<FontType0Dictionary> fontDictionary(makeFontDictionary());
    //std::unique_ptr<FontDescriptorDictionary> fontDescriptor(makeFontDescriptor());
    //std::unique_ptr<FontCIDDictionary> descendantFont(makeDescendantFont());

    //std::unique_ptr<ObjectArray> descendantArray = std::make_unique<ObjectArray>(IndirectTypeNonIndirectable);
    //descendantArray->insert(descendantFont->getReference());

    //fontDictionary->addKey(FontType0DictionaryKeyDescendantFonts, std::move(descendantArray));
    //descendantFont->addKey(FontCIDDictionaryKeyFontDescriptor, fontDescriptor->getReference());

    //if (params->mIsFontEmbedded){ // embed font program
    //  std::unique_ptr<FontFileStream> fileStream = std::make_unique<FontFileStream>();
    //  saveToStream(fileStream->getStream());
    //  fileStream->addKey(FontFileStreamKeyLength1, std::make_unique<ObjectInteger>(fileStream->getStream()->getSize()));
    //  fontDescriptor->addKey(FontDescriptorDictionaryKeyFontFile2, std::move(fileStream));
    //}

    //fontDictionary->serialize(stream, params);
    //descendantFont->serialize(stream, params);
    //fontDescriptor->serialize(stream, params);

    //std::unique_ptr<ObjectIndirectReference> ref = fontDictionary->getReference();
    //return ref;
  }

  void FontTrueType::toCharCodes(void const *glyphIndices, uint16_t elemSize, uint64_t count, pdfout::UString &charCodes, uint32_t defaultValue) const{
    TableCmap *tableCmap = getTableCmap();
    if (!tableCmap){
      charCodes = UString("", Encoding_ASCII);
      return;
    }

    Cmap *cmap = nullptr;
    switch (elemSize){
      case sizeof(uint8_t):
        cmap = tableCmap->getCmap(PlatformIdMacintosh, MacintoshEncodingRoman);
        break;

      case sizeof(uint16_t):
        cmap = tableCmap->getUnicodeCmap();
        break;

      default:
        NOT_IMPLEMENTED();
    }

    if (cmap)
      cmap->toCharCodes(glyphIndices, count, charCodes, defaultValue);
    else
      charCodes = UString("", Encoding_ASCII);
  }

  void FontTrueType::toText(void const *glyphIndices, uint16_t elemSize, uint64_t count, pdfout::UString &text, uint32_t defaultValue) const{
    TableCmap *tableCmap = getTableCmap();
    if (!tableCmap){
      text = UString("", Encoding_ASCII);
      return;
    }

    Cmap *cmap = nullptr;
    switch (elemSize){
      case sizeof(uint8_t):
        cmap = tableCmap->getCmap(PlatformIdMacintosh, MacintoshEncodingRoman);
        break;

      case sizeof(uint16_t):
        cmap = tableCmap->getUnicodeCmap();
        break;

      default:
        NOT_IMPLEMENTED();
    }

    if (cmap)
      cmap->toText(glyphIndices, count, text, defaultValue);
    else
      text = UString("", Encoding_ASCII);
  }

  int32_t FontTrueType::getTextWidth(pdfout::UString const &text, std::vector<int32_t> &widths) const{
    UString ucs2le = text.encode(Encoding_UCS2LE);
    uint16_t *source = (uint16_t *) ucs2le.getData();
    std::size_t const count = ucs2le.getSize() / sizeof(uint16_t);
    auto const eof = std::end(mCIDMetrics);
    widths.resize(count);

    int32_t sum = 0;
    for (auto i = 0; i < count; ++i)
      widths[i] = getCharWidth(source[i]);

    sum = std::accumulate(std::begin(widths), std::end(widths), 0);

    int32_t kern = 0;
    for (auto i = 0; i < count - 1; ++i)
      kern += getKerningWidth(source[i], source[i + 1]);

    return sum + kern;
  }

  int32_t FontTrueType::getTextWidth(pdfout::UString const &text) const{
    std::vector<int32_t> widths;
    int32_t const sum = getTextWidth(text, widths);
    return sum;
  }

  int32_t FontTrueType::getCharWidth(uint16_t ch) const{
    auto elem = mCIDMetrics.find(ch);
    if (elem != std::end(mCIDMetrics))
      return (elem->second.mLSB + elem->second.mWidth + elem->second.mRSB);

    return 0;
  }

  void FontTrueType::getCIDMetrics(uint16_t cid, pdfout::CIDMetrics &metrics) const{
    auto elem = mCIDMetrics.find(cid);
    if (elem != std::end(mCIDMetrics))
      metrics = elem->second;
    else
      memset(&metrics, 0, sizeof(pdfout::CIDMetrics));
  }

  int32_t FontTrueType::getKerningWidth(uint16_t cidLhs, uint16_t cidRhs) const{
    TableKern *kern = getTableKern();
    if (!kern)
      return 0;

    TableCmap *tableCmap = getTableCmap();
    if (!tableCmap)
      return 0;

    Cmap *cmap = tableCmap->getUnicodeCmap();
    if (!cmap)
      return 0;

    uint16_t cids[2] = {cidLhs, cidRhs};
    std::vector<uint16_t> gids;
    cmap->toGlyphIndices(cids, 2, gids);

    return kern->getKerningWidth(gids[0], gids[1]);
  }

  int32_t FontTrueType::getLineGap(void) const{
    TableOS2 *os2 = getTableOS2();
    if (os2)
      return os2->getLineGap();

    return 0;
  }

  int32_t FontTrueType::getAscent(void) const{
    TableOS2 *os2 = getTableOS2();
    if (os2)
      return os2->getAscent();

    return 0;
  }

  int32_t FontTrueType::getDescent(void) const{
    TableOS2 *os2 = getTableOS2();
    if (os2)
      return os2->getDescent();

    return 0;
  }

  int32_t FontTrueType::getCapHeight(void) const{
    TableOS2 *os2 = getTableOS2();
    if (os2)
      return os2->getCapHeight();

    return 0;
  }

  int32_t FontTrueType::getUnitsPerEm(void) const{
    TableHead *head = getTableHead();
    if (head)
      return head->getUnitsPerEm();

    return 0;
  }

  int32_t FontTrueType::getWinAscent(void) const{
    TableOS2 *os2 = getTableOS2();
    if (os2)
      return os2->getWinAscent();

    return 0;
  }

  int32_t FontTrueType::getWinDescent(void) const{
    TableOS2 *os2 = getTableOS2();
    if (os2)
      return os2->getWinDescent();

    return 0;
  }

  int32_t FontTrueType::getWinInternalLeading(void) const{
    return getWinAscent() + getWinDescent() - getUnitsPerEm();
  }

  int32_t FontTrueType::getWinExternalLeading(void) const{
    TableHhea *hhea = getTableHhea();
    if (hhea)
      return std::max(0, hhea->getLineGap() - ((getWinAscent() + getWinDescent()) - (hhea->getAscender() - hhea->getDescender())));

    return 0;
  }

  int32_t FontTrueType::getBreakChar(void) const{
    TableOS2 *os2 = getTableOS2();
    if (os2)
      return os2->getBreakChar();

    return 0;
  }

  int32_t FontTrueType::getDefaultChar(void) const{
    TableOS2 *os2 = getTableOS2();
    if (os2)
      return os2->getDefaultChar();

    return 0;
  }

  int32_t FontTrueType::getUnderlineYSize(void) const{
    TableOS2 *os2 = getTableOS2();
    if (os2)
      return os2->getUnderlineYSize();

    return 0;
  }

  bool FontTrueType::isBold(void) const {return getMacStyle() & MacStyleBold; }
  bool FontTrueType::isItalic(void) const {return getMacStyle() & MacStyleItalic;}
  bool FontTrueType::isUnderline(void) const {return getMacStyle() & MacStyleUnderline;}
  bool FontTrueType::isShadow(void) const {return getMacStyle() & MacStyleShadow;}
  bool FontTrueType::isCondensed(void) const {return getMacStyle() & MacStyleCondensed;}
  bool FontTrueType::isExtended(void) const {return getMacStyle() & MacStyleExtended;}

  std::unique_ptr<SimpleFont> FontTrueType::createSubset(std::vector<uint32_t> const &cids) const{
    std::unique_ptr<SimpleFont> subset(new FontTrueType());
    FontTrueType *rawSubset = (FontTrueType *) subset.get();

    TableCmap *cmap = getTableCmap();
    Cmap *uniCmap = cmap->getUnicodeCmap();
    if (!uniCmap)
      RUNTIME_EXCEPTION("Unicode-based CMAP subtable is missed");

    uint16_t const defaultChar = getDefaultChar();
    uint16_t const breakChar = getBreakChar();

    std::vector<uint32_t> cids32;
    std::vector<uint16_t> uniCids;
    uniCids.reserve(cids.size() + 2);
    if (std::find(std::begin(cids), std::end(cids), defaultChar) == std::end(cids))
      uniCids.push_back(defaultChar);

    if (std::find(std::begin(cids), std::end(cids), breakChar) == std::end(cids))
      uniCids.push_back(breakChar);

    for (auto elem : cids){
      if (elem < 0x10000)
        uniCids.push_back(elem);
    }

    std::vector<uint16_t> gids;
    uniCmap->toGlyphIndices(uniCids.data(), uniCids.size(), gids, 0);

    TableGlyf *glyf = getTableGlyf();
    TableLoca *loca = getTableLoca();
    if (glyf && loca) {
      std::vector<uint16_t> extraGids;
      std::vector<uint32_t> offsets;
      std::vector<uint32_t> extraOffsets;

      glyf->extractCompoundGids(gids, extraGids);

      UString extraCids("");
      uniCmap->toCharCodes(extraGids.data(), extraGids.size(), extraCids, 0);

      offsets = loca->getGidOffset(gids);
      extraOffsets = loca->getGidOffset(extraGids);

      std::size_t const size = gids.size();
      std::size_t const extraSize = extraGids.size();
      std::vector<CIDtoGID> cid2gid(size + extraSize);
      for (std::size_t i = 0; i < size; ++i)
        cid2gid[i] = {uniCids[i], gids[i], offsets[i]};

      for (std::size_t i = 0; i < extraSize; ++i)
        cid2gid[size + i] = {((uint16_t *) extraCids.getData())[i], extraGids[i], extraOffsets[i]};

      std::sort(std::begin(cid2gid), std::end(cid2gid), [](CIDtoGID const &lhs, CIDtoGID const &rhs){
        if (lhs.mOffset < rhs.mOffset)
          return true;
        else if (lhs.mOffset > rhs.mOffset)
          return false;

        return lhs.mGid < rhs.mGid;}
      );

      cids32.resize(cid2gid.size());
      gids.resize(cid2gid.size());
      for (std::size_t i = 0; i < cid2gid.size(); ++i){
        cids32[i] = cid2gid[i].mCid;
        gids[i] = cid2gid[i].mGid;
      }

      rawSubset->setTableLoca(loca->createSubset(gids));
    }
    else {
      cids32.assign(std::begin(cids), std::end(cids));
    }

    FontInfo subsetInfo;

    // glyf
    if (glyf)
      rawSubset->setTableGlyf(glyf->createSubset(gids));

    TableGlyf *subsetGlyf = rawSubset->getTableGlyf();
    if (subsetGlyf)
      subsetGlyf->updateFontInfo(subsetInfo);

    // head
    TableHead *head = getTableHead();
    if (head)
      rawSubset->setTableHead(head->createSubset(subsetInfo));

    // cmap
    if (cmap)
      rawSubset->setTableCmap(cmap->createSubset(cids32));

    // hmtx
    TableHmtx *hmtx = getTableHmtx();
    if (hmtx)
      rawSubset->setTableHmtx(hmtx->createSubset(gids));

    TableHmtx *subsetHmtx = rawSubset->getTableHmtx();
    subsetHmtx->updateFontInfo(subsetInfo);

    // hhea
    TableHhea *hhea = getTableHhea();
    if (hhea)
      rawSubset->setTableHhea(hhea->createSubset(gids, subsetInfo));

    // maxp
    TableMaxp *maxp = getTableMaxp();
    if (maxp)
      rawSubset->setTableMaxp(maxp->createSubset(gids, subsetInfo));

    // OS/2
    TableOS2 *os2 = getTableOS2();
    if (os2)
      rawSubset->setTableOS2(os2->createSubset(subsetInfo, defaultChar, breakChar));

    // name
    TableName *name = getTableName();
    if (name)
      rawSubset->setTableName(name->createSubset());

    // post
    TablePost *post = getTablePost();
    if (post)
      rawSubset->setTablePost(post->createSubset(gids));

    rawSubset->makeCIDMetrics();

    return subset;
  }

  std::unique_ptr<SimpleFont> FontTrueType::createSuperset(SimpleFont const *font) const{
    if (!dynamic_cast<FontTrueType const *>(font))
      RUNTIME_EXCEPTION("Incoming partial font is not a TrueType font");

    std::unique_ptr<SimpleFont> superset(new FontTrueType());
    FontTrueType *rawSuperset = (FontTrueType *) superset.get();

    FontTrueType *ttf = (FontTrueType *) font;

    // head
    TableHead *head = getTableHead();
    if (head)
      rawSuperset->setTableHead(head->createSuperset(ttf->getTableHead()));

    // hhea
    TableHhea *hhea = getTableHhea();
    if (hhea)
      rawSuperset->setTableHhea(hhea->createSuperset(ttf->getTableHhea()));

    // maxp
    TableMaxp *maxp = getTableMaxp();
    if (maxp)
      rawSuperset->setTableMaxp(maxp->createSuperset(ttf->getTableMaxp()));

    // hmtx
    TableHmtx *hmtx = getTableHmtx();
    if (hmtx)
      rawSuperset->setTableHmtx(hmtx->createSuperset(ttf->getTableHmtx()));

    // loca
    TableLoca *loca = getTableLoca();
    if (loca)
      rawSuperset->setTableLoca(loca->createSuperset(ttf->getTableLoca()));

    // glyf, merge loca table before
    TableGlyf *glyf = getTableGlyf();
    if (glyf)
      rawSuperset->setTableGlyf(glyf->createSuperset(ttf->getTableGlyf(), rawSuperset->getTableLoca()));

    // cmap
    TableCmap *cmap = getTableCmap();
    if (cmap)
      rawSuperset->setTableCmap(cmap->createSuperset(ttf->getTableCmap()));

    // OS/2
    TableOS2 *os2 = getTableOS2();
    if (os2)
      rawSuperset->setTableOS2(std::make_unique<TableOS2>(*os2));

    // name
    TableName *name = getTableName();
    if (name)
      rawSuperset->setTableName(std::make_unique<TableName>(*name));

    // post
    TablePost *post = getTablePost();
    if (post)
      rawSuperset->setTablePost(std::make_unique<TablePost>(*post));

    rawSuperset->makeCIDMetrics();

    return superset;
  }

  std::unique_ptr<ObjectStream> FontTrueType::cid2gid(void) const{
    TableCmap *tableCmap = getTableCmap();
    if (!tableCmap)
      return nullptr;

    Cmap *cmap = tableCmap->getUnicodeCmap();
    if (!cmap)
      return nullptr;

    return cmap->CIDtoGID();
  }

  std::unique_ptr<ObjectArray> FontTrueType::toW(void) const{
    TableCmap *tableCmap = getTableCmap();
    if (!tableCmap)
      return nullptr;

    Cmap *cmap = tableCmap->getUnicodeCmap();
    if (!cmap)
      return nullptr;

    float const scale = 1000.f / getUnitsPerEm();
    TableHmtx *tableHmtx = getTableHmtx();

    return cmap->toW(mCIDMetrics, scale);
  }

  std::unique_ptr<ToUnicodeCMapStream> FontTrueType::toUnicode(void) const{
    TablePost *tablePost = getTablePost();
    TableCmap *tableCmap = getTableCmap();

    if (tableCmap){
      Cmap *cmap = tableCmap->getUnicodeCmap();

      if (cmap){
        switch (cmap->getFormat()){
          case Format0:
          case Format4:
            return cmap->toUnicodeCMap(tablePost);
        }
      }
    }

    NOT_IMPLEMENTED();
    return nullptr;
  }

  bool FontTrueType::enumTables(pdfout::InputStream *stream){
    Endian const endian = getEndian();

    std::unique_ptr<TrueTypeOffsetTable> offsetTable(loadTrueTypeOffsetTable(stream));

    if (offsetTable->mVersion != TrueTypeVersionTTF &&
        offsetTable->mVersion != TrueTypeVersionOTTO)
      return false;

    char tag[5] = {0};
    TrueTypeTableRecord tableRecord;

    for (uint16_t i = 0; i < offsetTable->mNumOfTables; ++i){
      stream->read(&tableRecord, sizeof(uint8_t), sizeof(TrueTypeTableRecord));
      memcpy(tag, tableRecord.mTag, 4 * sizeof(uint8_t));

      if (endian == EndianLittle){
        swapByteOrder(tableRecord.mCheckSum);
        swapByteOrder(tableRecord.mOffset);
        swapByteOrder(tableRecord.mLength);
      }

      mTablesOffset[getTableId(tag)] = tableRecord;
    }

    // tables LOCA and GLYF should exist
    if (mTablesOffset.find(TrueTypeTableLoca) == std::end(mTablesOffset) ||
        mTablesOffset.find(TrueTypeTableGlyf) == std::end(mTablesOffset)) {
      return false;
    }

    return true;
  }

  TableHead *FontTrueType::getTableHead(void) const {return (TableHead *) mTables[TrueTypeTableHead].get();}
  TableHhea *FontTrueType::getTableHhea(void) const {return (TableHhea *) mTables[TrueTypeTableHhea].get();}
  TableCmap *FontTrueType::getTableCmap(void) const {return (TableCmap *) mTables[TrueTypeTableCmap].get();}
  TableGlyf *FontTrueType::getTableGlyf(void) const {return (TableGlyf *) mTables[TrueTypeTableGlyf].get();}
  TableHmtx *FontTrueType::getTableHmtx(void) const {return (TableHmtx *) mTables[TrueTypeTableHmtx].get();}
  TableLoca *FontTrueType::getTableLoca(void) const {return (TableLoca *) mTables[TrueTypeTableLoca].get();}
  TableMaxp *FontTrueType::getTableMaxp(void) const {return (TableMaxp *) mTables[TrueTypeTableMaxp].get();}
  TableName *FontTrueType::getTableName(void) const {return (TableName *) mTables[TrueTypeTableName].get();}
  TableOS2  *FontTrueType::getTableOS2(void) const  {return (TableOS2 *)  mTables[TrueTypeTableOS2].get();}
  TablePost *FontTrueType::getTablePost(void) const {return (TablePost *) mTables[TrueTypeTablePost].get();}
  TableDttf *FontTrueType::getTableDttf(void) const {return (TableDttf *) mTables[TrueTypeTableDttf].get();}
  TableKern *FontTrueType::getTableKern(void) const {return (TableKern *) mTables[TrueTypeTableKern].get();}

  void FontTrueType::setTableHead(std::unique_ptr<TableHead> table) {mTables[TrueTypeTableHead] = std::move(table);}
  void FontTrueType::setTableHhea(std::unique_ptr<TableHhea> table) {mTables[TrueTypeTableHhea] = std::move(table);}
  void FontTrueType::setTableCmap(std::unique_ptr<TableCmap> table) {mTables[TrueTypeTableCmap] = std::move(table);}
  void FontTrueType::setTableGlyf(std::unique_ptr<TableGlyf> table) {mTables[TrueTypeTableGlyf] = std::move(table);}
  void FontTrueType::setTableHmtx(std::unique_ptr<TableHmtx> table) {mTables[TrueTypeTableHmtx] = std::move(table);}
  void FontTrueType::setTableLoca(std::unique_ptr<TableLoca> table) {mTables[TrueTypeTableLoca] = std::move(table);}
  void FontTrueType::setTableMaxp(std::unique_ptr<TableMaxp> table) {mTables[TrueTypeTableMaxp] = std::move(table);}
  void FontTrueType::setTableName(std::unique_ptr<TableName> table) {mTables[TrueTypeTableName] = std::move(table);}
  void FontTrueType::setTableOS2(std::unique_ptr<TableOS2> table)   {mTables[TrueTypeTableOS2 ] = std::move(table);}
  void FontTrueType::setTablePost(std::unique_ptr<TablePost> table) {mTables[TrueTypeTablePost] = std::move(table);}
  void FontTrueType::setTableDttf(std::unique_ptr<TableDttf> table) {mTables[TrueTypeTableDttf] = std::move(table);}
  void FontTrueType::setTableKern(std::unique_ptr<TableKern> table) {mTables[TrueTypeTableKern] = std::move(table);}

  uint16_t FontTrueType::getMacStyle(void) const{
    TableHead *head = getTableHead();
    if (head)
      return head->getMacStyle();

    return 0;
  }

  bool FontTrueType::loadFontInfo(pdfout::InputStream *stream){
    loadTable<TableDttf>(stream, TrueTypeTableDttf);
    TableDttf *dttf = getTableDttf();

    loadTable<TableHead>(stream, TrueTypeTableHead);
    loadTable<TableHhea>(stream, TrueTypeTableHhea);
    loadTable<TableOS2>(stream, TrueTypeTableOS2);
    loadTable<TableName>(stream, TrueTypeTableName);
    loadTable<TableCmap>(stream, TrueTypeTableCmap);
    loadTable<TableMaxp>(stream, TrueTypeTableMaxp);
    loadTable<TablePost>(stream, TrueTypeTablePost);
    loadTable<TableKern>(stream, TrueTypeTableKern);
    loadTable<TableCff>(stream, TrueTypeTableCFF);

    auto const end = std::end(mTablesOffset);

    TableMaxp *maxp = getTableMaxp();
    TableHead *head = getTableHead();
    TableHhea *hhea = getTableHhea();

    if (dttf && maxp)
      maxp->mData.mNumGlyphs = dttf->mData.mMaxGlyphIndexUsed + 1;

    if (dttf && hhea)
      hhea->mData.mNumberOfHMetrics = dttf->mData.mMaxGlyphIndexUsed + 1;

    auto table = mTablesOffset.find(TrueTypeTableHmtx);
    if (table != end){
      if (hhea && maxp){
        std::unique_ptr<TableHmtx> hmtx = std::make_unique<TableHmtx>(hhea->mData.mNumberOfHMetrics, maxp->mData.mNumGlyphs);
        stream->seekg(table->second.mOffset);
        hmtx->load(stream);
        if (dttf)
          hmtx->update(dttf);

        setTableHmtx(std::move(hmtx));
      }
    }

    table = mTablesOffset.find(TrueTypeTableLoca);
    if (table != end){
      if (maxp && head){
        std::unique_ptr<TableLoca> loca = std::make_unique<TableLoca>(maxp->mData.mNumGlyphs, head->mData.mIndexToLocFormat);
        stream->seekg(table->second.mOffset);
        loca->load(stream);
        if (dttf)
          loca->unpack(dttf);

        setTableLoca(std::move(loca));
      }
    }

    TableLoca *loca = getTableLoca();

    table = mTablesOffset.find(TrueTypeTableGlyf);
    if (table != end){
      if (loca){
        std::unique_ptr<TableGlyf> glyf = std::make_unique<TableGlyf>(loca);
        stream->seekg(table->second.mOffset);
        glyf->load(stream);
        
        setTableGlyf(std::move(glyf));
      }
    }

    return true;
  }

  MemoryOStream *FontTrueType::saveTable(TrueTypeTable tableTag, TrueTypeTableRecord &record) const{
    Table *table = mTables[tableTag].get();
    if (!table)
      return nullptr;

    std::unique_ptr<MemoryOStream> stream = std::make_unique<MemoryOStream>();

    table->save(stream.get());

    uint32_t size = (uint32_t) stream->getSize();
    int const padding = getPadTo4(size);
    uint8_t pad[4] = {0};
    stream->seekp(stream->getSize());
    stream->write(pad, sizeof(uint8_t), padding);

    tableIdToTag(tableTag, record.mTag);
    record.mLength = size; // (uint32_t) stream->getSize();
    record.mCheckSum = calculateChecksum((uint32_t *) stream->getData(), stream->getSize());
    record.mOffset = 0;

    return stream.release();
  }

  std::unique_ptr<FontDescriptorDictionary> FontTrueType::makeFontDescriptor(void) const{
    std::unique_ptr<FontDescriptorDictionary> fontDescriptor = std::make_unique<FontDescriptorDictionary>();

    fontDescriptor->addKey(FontDescriptorDictionaryKeyStemV, std::make_unique<ObjectNumber>(1));

    TableName *name = getTableName();

    if (name){
      UString postScriptName = name->getPostScriptName(); // name->getFamilyName();
      fontDescriptor->addKey(FontDescriptorDictionaryKeyFontName, std::make_unique<ObjectName>(postScriptName.getData(), postScriptName.getSize()));

      if (auto fontFamilyName = name->getNameRecord(NameIdFontFamilyName))
        fontDescriptor->addKey(FontDescriptorDictionaryKeyFontFamily, std::make_unique<ObjectByteString>(fontFamilyName->mName));
    }

    float scale = 1.f;

    TableHead *head = getTableHead();
    if (head){
      fontDescriptor->addKey(FontDescriptorDictionaryKeyFontBBox, std::make_unique<ObjectRectangle>(head->getBBox()));
      scale = 1000.0 / head->getUnitsPerEm();
    }

    uint16_t flags = 0;

    TableOS2 *os2 = getTableOS2();
    if (os2){
      if (os2->isSymbolic())
        flags |= FontFlagsSymbolic;
      else
        flags |= FontFlagsNonsymbolic;

      if (os2->isSerif())
        flags |= FontFlagsSerif;

      if (os2->isScript())
        flags |= FontFlagsScript;

      fontDescriptor->addKey(FontDescriptorDictionaryKeyFontStretch, std::make_unique<ObjectName>(os2->getWidthClass()));
      fontDescriptor->addKey(FontDescriptorDictionaryKeyFontWeight, std::make_unique<ObjectNumber>(os2->getWeightClass()));
      fontDescriptor->addKey(FontDescriptorDictionaryKeyCapHeight, std::make_unique<ObjectNumber>(os2->getCapHeight() * scale));
      fontDescriptor->addKey(FontDescriptorDictionaryKeyAscent, std::make_unique<ObjectNumber>(os2->getAscent() * scale));
      fontDescriptor->addKey(FontDescriptorDictionaryKeyDescent, std::make_unique<ObjectNumber>(os2->getDescent() * scale));
    }

    bool isFixedPitch = false;
    TablePost *post = getTablePost();
    if (post){
      if (isFixedPitch = post->isFixedPitch())
        flags |= FontFlagsFixedPitch;

      if (post->getItalicAngle())
        flags |= FontFlagsItalic;

      fontDescriptor->addKey(FontDescriptorDictionaryKeyFlags, std::make_unique<ObjectInteger>(flags));
      fontDescriptor->addKey(FontDescriptorDictionaryKeyItalicAngle, std::make_unique<ObjectNumber>(post->getItalicAngle()));
    }

    return fontDescriptor;
  }

  uint32_t FontTrueType::calculateChecksum(uint32_t *table, uint32_t length) const{
    uint32_t sum = 0;
    uint32_t longs = (length + 3) / 4;
    uint32_t value;
    Endian const endian = getEndian();

    while (longs-- > 0){
      value = *table++;
      if (endian == EndianLittle)
        swapByteOrder(value);

      sum += value;
    }

    return sum;
  }

  void FontTrueType::makeCIDMetrics(void){
    if (!mCIDMetrics.empty())
      return;

    TableHmtx *tableHmtx = getTableHmtx();
    TableCmap *tableCmap = getTableCmap();
    TableGlyf *tableGlyf = getTableGlyf();
    TableOS2 *tableOS2 = getTableOS2();
    uint32_t const defaultGID = tableOS2 ? tableOS2->getDefaultChar() : 0;
    Cmap *cmap = nullptr;
    if (tableCmap && tableHmtx){
      cmap = tableCmap->getUnicodeCmap();
      if (cmap)
        cmap->getCIDMetrics(tableHmtx, tableGlyf, mCIDMetrics, defaultGID);
    }
  }

}}}

namespace{
  TrueTypeOffsetTable *makeTrueTypeOffsetTable(uint16_t numOfTables){
    std::unique_ptr<TrueTypeOffsetTable> table = std::make_unique<TrueTypeOffsetTable>();
    table->mVersion = kernel::font::truetype::TableVersion10;
    table->mNumOfTables = numOfTables;

    uint16_t const maxPowOf2 = getMaxPowOf2LessThan(numOfTables);

    table->mSearchRange = std::pow(2, maxPowOf2) * 16;
    table->mEntrySelector = maxPowOf2;
    table->mRangeShift = table->mNumOfTables * 16 - table->mSearchRange;

    return table.release();
  }

  TrueTypeOffsetTable *loadTrueTypeOffsetTable(pdfout::InputStream *stream){
    std::unique_ptr<TrueTypeOffsetTable> table = std::make_unique<TrueTypeOffsetTable>();
    stream->read(table.get(), sizeof(TrueTypeOffsetTable), 1);
    if (getEndian() == EndianLittle){
      swapByteOrder(table->mVersion);
      swapByteOrder(table->mNumOfTables);
      swapByteOrder(table->mSearchRange);
      swapByteOrder(table->mEntrySelector);
      swapByteOrder(table->mRangeShift);
    }

    return table.release();
  }

  void saveTrueTypeOffsetTable(pdfout::OutputStream *stream, TrueTypeOffsetTable *table){
    TrueTypeOffsetTable data;
    memcpy(&data, table, sizeof(TrueTypeOffsetTable));

    if (getEndian() == EndianLittle){
      swapByteOrder(data.mVersion);
      swapByteOrder(data.mNumOfTables);
      swapByteOrder(data.mSearchRange);
      swapByteOrder(data.mEntrySelector);
      swapByteOrder(data.mRangeShift);
    }

    stream->write(&data, sizeof(TrueTypeOffsetTable), 1);
  }
}
