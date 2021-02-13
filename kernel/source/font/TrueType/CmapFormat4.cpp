#include <font/TrueType/CmapFormat4.h>

#include <memory>
#include <algorithm>

#include <InternalTools.h>

#include <font/TrueType/Glyph.h>
#include <font/TrueType/TableGlyf.h>
#include <font/TrueType/TableHmtx.h>
#include <font/TrueType/TablePost.h>

#include <font/ToUnicode.h>
#include <font/AdobeGlyphList.h>
#include <font/MacintoshGlyphList.h>

#undef min
#undef max

using namespace pdfout;

namespace{
  PACK(struct Table4{
    uint16_t mFormat;        /**< Format number is set to 4. */
    uint16_t mLength;        /**< This is the length in bytes of the subtable. */
    uint16_t mLanguage;      /**< See “Note on the language field in 'cmap' subtables“ in https://www.microsoft.com/typography/otspec/cmap.htm#language */
    uint16_t mSegCountX2;    /**< 2 x segCount. */
    uint16_t mSearchRange;   /**< 2 x (2**floor(log2(segCount))) */
    uint16_t mEntrySelector; /**< log2(searchRange/2) */
    uint16_t mRangeShift;    /**< 2 x segCount - searchRange */
  });
}

namespace kernel{ namespace font{ namespace truetype{

  CmapFormat4::CmapFormat4(uint16_t platformId, uint16_t encodingId)
    : Cmap(Format4, platformId, encodingId){
  }

  CmapFormat4::CmapFormat4(CmapFormat4 const &obj)
    : Cmap(obj),
    mCharToGlyph(obj.mCharToGlyph),
    mGlyphToChar(),
    mGidToGid(){
  }

  CmapFormat4::~CmapFormat4(void) {}

  void CmapFormat4::load(pdfout::InputStream *stream){
    Endian const endian = getEndian();

    Table4 cmap4;
    stream->read(&cmap4, sizeof(Table4), 1);

    if (endian == EndianLittle){
      swapByteOrder(cmap4.mFormat);
      swapByteOrder(cmap4.mLength);
      swapByteOrder(cmap4.mLanguage);
      swapByteOrder(cmap4.mSegCountX2);
      swapByteOrder(cmap4.mSearchRange);
      swapByteOrder(cmap4.mEntrySelector);
      swapByteOrder(cmap4.mRangeShift);
    }

    if (cmap4.mFormat != Format4)
      RUNTIME_EXCEPTION("invalid cmap format");

    mLanguage = cmap4.mLanguage;

    uint16_t segCount = cmap4.mSegCountX2 / 2;
    std::size_t size = segCount * sizeof(uint16_t);

    std::vector<uint16_t> endCount(segCount);
    stream->read(endCount.data(), sizeof(uint8_t), size);

    if (endian == EndianLittle)
      for (auto &elem : endCount)
        swapByteOrder(elem);

    uint16_t reservedPad = 0;
    stream->read(&reservedPad, sizeof(uint16_t), 1);

    std::vector<uint16_t> startCount(segCount);
    stream->read(startCount.data(), sizeof(uint8_t), size);

    if (endian == EndianLittle)
      for (auto &elem : startCount)
        swapByteOrder(elem);

    std::vector<int16_t> idDelta(segCount);
    stream->read(idDelta.data(), sizeof(uint8_t), size);

    if (endian == EndianLittle)
      for (auto &elem : idDelta)
        swapByteOrder(elem);

    std::size_t glyphsCount = (cmap4.mLength - sizeof(Table4) - (segCount * 4 + 1) * sizeof(uint16_t)) / 2;
    std::size_t reserved = 0;
    for (std::size_t segment = 0; segment < segCount && endCount[segment] != 0xFFFF; ++segment)
      reserved += endCount[segment] - startCount[segment] + 1;

    std::vector<uint16_t> idRangeOffset(segCount);
    stream->read(idRangeOffset.data(), sizeof(uint16_t), segCount);

    std::vector<uint16_t> glyphIndices(glyphsCount);
    stream->read(glyphIndices.data(), sizeof(uint16_t), glyphsCount);

    if (endian == EndianLittle){
      for (auto &elem : idRangeOffset)
        swapByteOrder(elem);

      for (auto &elem : glyphIndices)
        swapByteOrder(elem);
    }

    uint16_t *idRangeOffsetPtr = idRangeOffset.data();

    mCharToGlyph.reserve(reserved);

    uint16_t glyphId = 0;
    uint16_t index = 0;
    for (std::size_t segment = 0; segment < segCount && endCount[segment] != 0xFFFF; ++segment){
      int16_t const delta = idDelta[segment];
      uint16_t const start = startCount[segment];
      uint16_t const end = endCount[segment];
      uint16_t const range = idRangeOffset[segment];
      for (uint16_t c = start; c <= end; ++c){
        if (range != 0){
          index = range / 2 + c + segment - start - segCount;
          if (index >= glyphsCount)
            continue;

          glyphId = glyphIndices[index];
          if (glyphId != 0)
            glyphId += delta;
        }
        else
          glyphId = c + delta;

        mCharToGlyph[c] = glyphId % 0x10000;
      }
    }

    makeGlyphToChar();
  }

  bool CmapFormat4::isSequental(std::size_t from, std::size_t to, std::vector<uint16_t> const &indices) const{
    uint16_t prev = indices[from];
    for (std::size_t i = from + 1; i <= to; ++i){
      if (indices[i] == prev + 1)
        ++prev;
      else
        return false;
    }

    return true;
  }

  void CmapFormat4::save(pdfout::OutputStream *stream) const{
    std::vector<uint16_t> charCodes;
    charCodes.reserve(mCharToGlyph.size());
    for (auto const &elem : mCharToGlyph)
      charCodes.push_back(elem.first);

    std::sort(std::begin(charCodes), std::end(charCodes));

    std::vector<uint16_t> glyphIndices;
    glyphIndices.reserve(mCharToGlyph.size());
    for (auto const &elem : charCodes)
      glyphIndices.push_back(mCharToGlyph[elem]);

    enum RangeType{
      RangeTypeDelta = 0,
      RangeTypeRO
    };

    struct Value{
      uint16_t mCharCode;   /**< start char code */
      uint16_t mGlyphIndex; /**< start glyph index */
      uint16_t mSize;       /**< size of sequental elements */
      uint16_t mIndex;      /**< index in charCodes vector */
      uint16_t mSegNum;     /**< segment number */
      RangeType mType;      /**< segment range type */

      Value(uint16_t charCode, uint16_t glyphIndex, uint16_t size, uint16_t index)
        : mCharCode(charCode), mGlyphIndex(glyphIndex), mSize(size), mIndex(index), mSegNum(0), mType(RangeTypeDelta){
      }
    };

    std::vector<Value> values;

    if (!charCodes.empty() && !glyphIndices.empty()) {
      values.push_back(std::move(Value(charCodes[0], glyphIndices[0], 1, 0)));
      std::size_t index = 0;
      Value *ptr = &values[index];

      for (std::size_t i = 1, size = charCodes.size(); i < size; ++i){
        if (charCodes[i] == ptr->mCharCode + ptr->mSize && glyphIndices[i] == ptr->mGlyphIndex + ptr->mSize)
          ++ptr->mSize;
        else{
          values.push_back(std::move(Value(charCodes[i], glyphIndices[i], 1, i)));
          ++index;
          ptr = &values[index];
        }
      }

      std::sort(std::begin(values), std::end(values),[](Value const &lhs, Value const &rhs) -> bool {return lhs.mCharCode < rhs.mCharCode || (lhs.mCharCode ==  rhs.mCharCode && lhs.mSize > rhs.mSize);});
    }

    uint16_t segCount = 1;

    if (!values.empty()) {
      uint16_t endCode = values[0].mCharCode + values[0].mSize;
      values[0].mSegNum = segCount;
      values[0].mType = RangeTypeRO;
      if (values[0].mSize > 4){
        endCode = 0;
        values[0].mType = RangeTypeDelta;
      }

      for (std::size_t i = 1, size = values.size(); i < size; ++i){
        if (values[i].mSize > 4){
          values[i].mSegNum = ++segCount ;
          values[i].mType = RangeTypeDelta;
          endCode = 0;
        }
        else{
          if (values[i].mCharCode == endCode)
            values[i].mSegNum = segCount;
          else
            values[i].mSegNum = ++segCount;

          values[i].mType = RangeTypeRO;
          endCode = values[i].mCharCode + values[i].mSize;
        }
      }

      ++segCount;
    }

    std::vector<uint16_t> startCount(segCount, 0);
    std::vector<uint16_t> endCount(segCount, 0);
    std::vector<uint16_t> idRangeOffset(segCount, 0);
    std::vector<uint16_t> idDelta(segCount, 0);
    std::vector<uint16_t> glyphIndicesToWrite;

    uint16_t offset = 0;
    uint16_t segNum = 0;
    for (auto const &elem : values){
      segNum = elem.mSegNum - 1;
      if (elem.mType == RangeTypeDelta){
        idDelta[elem.mSegNum - 1] = ((int16_t) elem.mGlyphIndex - elem.mCharCode) % 0x10000;
        startCount[segNum] = elem.mCharCode;
        endCount[segNum] = elem.mCharCode + elem.mSize - 1;
      }
      else{ // range offset
        if (idRangeOffset[segNum] == 0){
          idRangeOffset[segNum] = 2 * (offset + segCount - segNum);
          startCount[segNum] = elem.mCharCode;
          endCount[segNum] = elem.mCharCode + elem.mSize - 1;
        }
        else
          endCount[segNum] += elem.mSize;

        for (uint16_t i = 0; i < elem.mSize; ++i)
          glyphIndicesToWrite.push_back(elem.mGlyphIndex + i);

        offset += elem.mSize;
      }
    }

    startCount[segCount - 1] = 0xFFFF;
    endCount[segCount - 1] = 0xFFFF;

    Table4 cmap4;
    cmap4.mFormat = 4;
    cmap4.mLength = sizeof(Table4) + (segCount * 4 + 1 + glyphIndicesToWrite.size()) * sizeof(uint16_t);
    cmap4.mLanguage = mLanguage;
    cmap4.mSegCountX2 = segCount * 2;
    cmap4.mSearchRange = 2 * (std::pow(2, std::floor(std::log2(segCount))));
    cmap4.mEntrySelector = std::log2(cmap4.mSearchRange / 2);
    cmap4.mRangeShift = cmap4.mSegCountX2 - cmap4.mSearchRange;

    Endian const endian = getEndian();
    if (endian == EndianLittle){
      for (auto &elem : startCount)
        swapByteOrder(elem);

      for (auto &elem : endCount)
        swapByteOrder(elem);

      for (auto &elem : idRangeOffset)
        swapByteOrder(elem);

      for (auto &elem : idDelta)
        swapByteOrder(elem);

      for (auto &elem : glyphIndicesToWrite)
        swapByteOrder(elem);

      swapByteOrder(cmap4.mFormat);
      swapByteOrder(cmap4.mLength);
      swapByteOrder(cmap4.mLanguage);
      swapByteOrder(cmap4.mSegCountX2);
      swapByteOrder(cmap4.mSearchRange);
      swapByteOrder(cmap4.mEntrySelector);
      swapByteOrder(cmap4.mRangeShift);
    }

    stream->write(&cmap4, sizeof(Table4), 1);
    stream->write(endCount.data(), sizeof(uint16_t), endCount.size());
    uint16_t reserved = 0;
    stream->write(&reserved, sizeof(uint16_t), 1);
    stream->write(startCount.data(), sizeof(uint16_t), startCount.size());
    stream->write(idDelta.data(), sizeof(int16_t), idDelta.size());
    stream->write(idRangeOffset.data(), sizeof(uint16_t), idRangeOffset.size());
    stream->write(glyphIndicesToWrite.data(), sizeof(uint16_t), glyphIndicesToWrite.size());
  }

  void CmapFormat4::toGlyphIndices(void const *charCodes, uint64_t count, std::vector<uint16_t> &glyphIndices, uint16_t defaultValue) const{
    uint16_t const *chars = (uint16_t *) charCodes;
    glyphIndices.resize(count);
    auto glyph = std::end(mCharToGlyph);
    auto const end = std::end(mCharToGlyph);
    for (auto i = 0; i < count; ++i){
      glyph = mCharToGlyph.find(chars[i]);
      if (glyph != end)
        glyphIndices[i] = glyph->second;
      else
        glyphIndices[i] = defaultValue;
    }
  }

  void CmapFormat4::toCharCodes(void const *glyphIndices, uint64_t count, pdfout::UString &charCodes, uint32_t defaultValue) const{
    uint16_t const *glyphs = (uint16_t *) glyphIndices;
    std::vector<uint16_t> chars(count);

    auto charCode = std::end(mGlyphToChar);
    auto const end = std::end(mGlyphToChar);
    for (auto i = 0; i < count; ++i){
      charCode = mGlyphToChar.find(glyphs[i]);
      if (charCode != end)
        chars[i] = charCode->second;
      else
        chars[i] = defaultValue;
    }

    charCodes = UString((uint8_t *) chars.data(), chars.size() * sizeof(uint16_t), Encoding_UCS2LE);
  }

  void CmapFormat4::toText(void const *glyphIndices, uint64_t count, pdfout::UString &text, uint32_t defaultValue) const{
    uint16_t const *glyphs = (uint16_t *) glyphIndices;
    std::vector<uint16_t> chars(count);

    auto charCode = std::end(mGlyphToChar);
    auto const end = std::end(mGlyphToChar);
    for (auto i = 0; i < count; ++i){
      charCode = mGlyphToChar.find(glyphs[i]);
      if (charCode != end){
        if (charCode->second >= 0xE000 && charCode->second <= 0xFFFF){
          // Unicode Private Area
          UnicodeInfo uniInfo = {0, {defaultValue}};
          if (glyphs[i] < 258) // use standard Macintosh order
            getUnicodeInMacintoshOrder(glyphs[i] - 1, uniInfo);
          chars[i] = uniInfo.mValue[0];
        }
        else
          chars[i] = charCode->second;
      }
      else
        chars[i] = defaultValue;
    }

    text = UString((uint8_t *) chars.data(), chars.size() * sizeof(uint16_t), Encoding_UCS2LE);
  }

  std::unique_ptr<Cmap> CmapFormat4::createSubset(std::vector<uint32_t> const &cids) const{
    std::unique_ptr<CmapFormat4> cmap = std::make_unique<CmapFormat4>(getPlatformId(), getEncodingId());
    cmap->mGidToGid.resize(cids.size());

    for (std::size_t i = 0; i < cids.size(); ++i){
      auto gid = mCharToGlyph.find(cids[i]);
      if (gid != std::end(mCharToGlyph)){
        cmap->mCharToGlyph[cids[i]] = i;
        cmap->mGidToGid[i] = gid->second;
      }
      else{
        cmap->mCharToGlyph[cids[i]] = 0;
        cmap->mGidToGid[i] = 0;
      }
    }

    cmap->makeGlyphToChar();

    return cmap;
  }

  std::unique_ptr<Cmap> CmapFormat4::createSuperset(Cmap const *cmap) const{
    std::unique_ptr<CmapFormat4> superset = std::make_unique<CmapFormat4>(*this);

    CmapFormat4 const *cmap4 = (CmapFormat4 const *) cmap;
    if (cmap4){
      for (auto const &elem : cmap4->mCharToGlyph)
        superset->mCharToGlyph[elem.first] = elem.second;
    }

    superset->makeGlyphToChar();

    return superset;
  }

  std::unique_ptr<ToUnicodeCMapStream> CmapFormat4::toUnicodeCMap(TablePost *post) const{
    std::unique_ptr<ToUnicodeCMapStream> toUnicodeCMapStream = std::make_unique<ToUnicodeCMapStream>();

    UnicodeInfo uniInfo = {0};
    uint16_t glyphNameId = 0;
    std::vector<uint32_t> values;
    bool const useGidToGid = !mGidToGid.empty();

    for (auto iter = std::begin(mCharToGlyph); iter != std::end(mCharToGlyph); ++iter){
      uint16_t const cid = iter->first;
      uint16_t const gid = useGidToGid ? mGidToGid[iter->second] : iter->second;

      if (cid >= 0xE000 && cid <= 0xFFFF){
        // Unicode Private Area
        if (post)
          post->getUnicode(gid, uniInfo);
        else
          uniInfo = {0};
      }
      else{
        // use Identity cmap mapping
        uniInfo.mCount = 1;
        uniInfo.mValue[0] = cid;
      }

      toUnicodeCMapStream->addBfChar(cid, uniInfo);
    }

    return toUnicodeCMapStream;
  }

  std::unique_ptr<ObjectStream> CmapFormat4::CIDtoGID(void) const{
    Endian const endian = getEndian();
    std::unique_ptr<ObjectStream> cid2gid = std::make_unique<ObjectStream>();

    uint16_t defaultValue = 0xFFFD;
    if (endian == EndianLittle)
      swapByteOrder(defaultValue);

    uint16_t maxCID = 0;
    for (auto elem : mCharToGlyph)
      maxCID = std::max(maxCID, elem.first);

    std::vector<uint16_t> data(maxCID + 1, defaultValue); /**< Lastly, if a CID does not map to a Unicode code point, the value 0xFFFD
                                                     (expressed as “<FFFD>” in the “ToUnicode” mapping file) shall be used as its Unicode code point. */
    for (auto iter = std::begin(mCharToGlyph); iter != std::end(mCharToGlyph); ++iter)
      data[iter->first] = iter->second;

    if (endian == EndianLittle)
      for (auto &elem : data)
        swapByteOrder(elem);

    cid2gid->write(data.data(), sizeof(uint16_t), data.size());

    return cid2gid;
  }

  std::unique_ptr<ObjectArray> CmapFormat4::toW(std::unordered_map<uint16_t, pdfout::CIDMetrics> const &cidMetrics, float scale){
    if (cidMetrics.empty() || mCharToGlyph.empty())
      return nullptr;

    std::unordered_map<uint16_t, int32_t> charInfo;
    charInfo.reserve(mCharToGlyph.size());

    for (auto const &elem : mCharToGlyph){
      auto cidInfo = cidMetrics.find(elem.first);
      if (cidInfo != std::end(cidMetrics))
        charInfo[elem.first] = std::lround((float) (cidInfo->second.mLSB + cidInfo->second.mWidth + cidInfo->second.mRSB) * scale);
    }

    auto from = std::begin(charInfo);
    auto to = std::begin(charInfo);
    auto const eof = std::end(charInfo);
    auto iter = std::begin(charInfo);
    std::advance(iter, 1);

    std::vector<int32_t> values;
    std::unique_ptr<ObjectArray> result = std::make_unique<ObjectArray>(IndirectTypeNonIndirectable);

    bool isRange = false; // when Cf Ct W

    /**
     * Creates Cf Ct W
     */
    auto makeRange = [&](){
      result->insert(std::make_unique<ObjectInteger>(from->first));
      result->insert(std::make_unique<ObjectInteger>(to->first));
      result->insert(std::make_unique<ObjectInteger>(from->second));
    };

    /**
     * Creates C [W1 W2 ... Wn]
     */
    auto makeSequence = [&](bool isIncludeTo){
      values.clear();
      for (auto tmp = from; tmp != to; ++tmp)
        values.push_back(tmp->second);

      if (isIncludeTo)
        values.push_back(to->second);

      if (!values.empty()){
        std::unique_ptr<ObjectArray> sequence = std::make_unique<ObjectArray>(values.data(), values.size(), IndirectTypeNonIndirectable);
        result->insert(std::make_unique<ObjectInteger>(from->first));
        result->insert(std::move(sequence));
      }
    };

    for (; iter != eof; ++iter){
      if (iter->first == to->first + 1){
        if (iter->second == to->second){
          if (isRange) // append item to current range
            to = iter;
          else{ // drop non-range values and start new range
            makeSequence(false);
            from = to;
            to = iter;
            isRange = true;
          }
        }
        else{
          if (isRange){
            makeRange();
            isRange = false;
            from = to = iter;
          }
          else
            to = iter;
        }
      }
      else{
        if (isRange)
          makeRange();
        else
          makeSequence(true);

        from = to = iter;
        isRange = false;
      }
    }

    if (isRange)
      makeRange();
    else
      makeSequence(true);

    return result;
  }

  void CmapFormat4::getCIDMetrics(TableHmtx *hmtx, TableGlyf *glyf, std::unordered_map<uint16_t, pdfout::CIDMetrics> &cidMetrics, uint16_t defaultCID) const{
    if (!hmtx || !glyf)
      return;

    cidMetrics.reserve(mCharToGlyph.size());

    auto defaultGIDIter = mCharToGlyph.find(defaultCID);
    uint16_t const defaultGID = (defaultGIDIter != std::end(mCharToGlyph)) ? defaultGIDIter->second : 0;
    Glyph const *defaultGlyph = glyf->getGlyph(defaultGID);

    uint16_t const maxGID = hmtx->mNumGlyphs - 1;
    uint16_t const maxIndex = hmtx->mNumOfMetrics - 1;
    int32_t const defaultWidth = defaultGlyph ? defaultGlyph->getGlyphDescription().mXMax - defaultGlyph->getGlyphDescription().mXMin : hmtx->mHMetrix[maxIndex].mAdvanceWidth;
    int32_t const defaultLSB = hmtx->mHMetrix[maxIndex].mLsb;
    int32_t const defaultRSB = hmtx->mHMetrix[maxIndex].mAdvanceWidth - defaultLSB - defaultWidth;

    for (auto const &elem : mCharToGlyph){
      uint16_t const gid = elem.second;
      pdfout::CIDMetrics metrics = {defaultLSB, defaultWidth, defaultRSB};

      if (gid <= maxIndex){
        Glyph const *glyph = glyf->getGlyph(gid);
        metrics.mLSB = hmtx->mHMetrix[gid].mLsb;
        metrics.mWidth = glyph ? glyph->getGlyphDescription().mXMax - glyph->getGlyphDescription().mXMin
                               : hmtx->mHMetrix[gid].mAdvanceWidth - metrics.mLSB;
        metrics.mRSB = hmtx->mHMetrix[gid].mAdvanceWidth - metrics.mLSB - metrics.mWidth;
      }
      else if (gid <= maxGID){
        metrics.mLSB = hmtx->mLeftSideBearing[gid - maxIndex - 1];
        metrics.mWidth = hmtx->mHMetrix[maxIndex].mAdvanceWidth - metrics.mLSB;
        metrics.mRSB = 0;
      }

      cidMetrics[elem.first] = metrics;
    }
  }

  std::unique_ptr<Cmap> CmapFormat4::clone(void) const{
    return std::make_unique<CmapFormat4>(*this);
  }

  void CmapFormat4::makeGlyphToChar(void){
    if (!mGlyphToChar.empty())
      return;

    mGlyphToChar.reserve(mCharToGlyph.size());

    for (auto const &elem : mCharToGlyph)
      mGlyphToChar[elem.second] = elem.first;
  }

}}}
