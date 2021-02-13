#include <font/TrueType/CmapFormat12.h>

#include <memory>
#include <algorithm>

#include <InternalTools.h>

#include <font/TrueType/TableHmtx.h>
#include <font/TrueType/TablePost.h>

#include <font/ToUnicode.h>
#include <font/AdobeGlyphList.h>
#include <font/MacintoshGlyphList.h>

#undef min
#undef max

using namespace pdfout;

namespace{
  PACK(struct Table12{
    uint16_t mFormat;        /**< Format number is set to 12. */
    uint16_t mReserved;      /**< Reserved; set to 0 */
    uint32_t mLength;        /**< This is the length in bytes of the subtable. */
    uint32_t mLanguage;      /**< See “Note on the language field in 'cmap' subtables“ in https://www.microsoft.com/typography/otspec/cmap.htm#language */
    uint32_t mNumGroups;     /**< Number of groupings which follow */
  });

  PACK(struct SequentalMapGroupRecord{
    uint32_t mStartCharCode; /**< First character code in this group */
    uint32_t mEndCharCode;   /**< Last character code in this group */
    uint32_t mStartGlyphID;  /**< Glyph index corresponding to the starting character code */
  });
}

namespace kernel{ namespace font{ namespace truetype{

  CmapFormat12::CmapFormat12(uint16_t platformId, uint16_t encodingId)
    : Cmap(Format12, platformId, encodingId){
  }

  CmapFormat12::CmapFormat12(CmapFormat12 const &obj)
    : Cmap(obj),
    mCharToGlyph(obj.mCharToGlyph),
    mGlyphToChar(),
    mGidToGid(){
  }

  CmapFormat12::~CmapFormat12(void) {}

  void CmapFormat12::load(pdfout::InputStream *stream){
    Endian const endian = getEndian();

    Table12 cmap12;
    stream->read(&cmap12, sizeof(Table12), 1);

    if (endian == EndianLittle){
      swapByteOrder(cmap12.mFormat);
      swapByteOrder(cmap12.mLength);
      swapByteOrder(cmap12.mLanguage);
      swapByteOrder(cmap12.mNumGroups);
    }

    if (cmap12.mFormat != Format12)
      RUNTIME_EXCEPTION("invalid cmap format");

    mLanguage = cmap12.mLanguage;

    std::vector<SequentalMapGroupRecord> groups(cmap12.mNumGroups);
    stream->read(groups.data(), sizeof(SequentalMapGroupRecord), cmap12.mNumGroups);

    if (endian == EndianLittle){
      for (auto &elem : groups){
        swapByteOrder(elem.mStartCharCode);
        swapByteOrder(elem.mEndCharCode);
        swapByteOrder(elem.mStartGlyphID);
      }
    }

    std::size_t glyphsCount = 0;
    for (auto const &segment : groups)
      glyphsCount += segment.mEndCharCode - segment.mStartCharCode + 1;

    mCharToGlyph.reserve(glyphsCount);

    for (auto const &segment : groups){
      for (uint32_t charId = segment.mStartCharCode, glyphId = segment.mStartGlyphID; charId <= segment.mEndCharCode; ++charId, ++glyphId)
        mCharToGlyph[charId] = glyphId;
    }

    makeGlyphToChar();
  }

  bool CmapFormat12::isSequental(std::size_t from, std::size_t to, std::vector<uint16_t> const &indices) const{
    uint16_t prev = indices[from];
    for (std::size_t i = from + 1; i <= to; ++i){
      if (indices[i] == prev + 1)
        ++prev;
      else
        return false;
    }

    return true;
  }

  void CmapFormat12::save(pdfout::OutputStream *stream) const{
    std::vector<uint32_t> charCodes;
    charCodes.reserve(mCharToGlyph.size());
    for (auto const &elem : mCharToGlyph)
      charCodes.push_back(elem.first);

    std::sort(std::begin(charCodes), std::end(charCodes));

    std::vector<uint32_t> glyphIndices;
    glyphIndices.reserve(mCharToGlyph.size());
    for (auto const &elem : charCodes)
      glyphIndices.push_back(mCharToGlyph[elem]);

    std::vector<SequentalMapGroupRecord> groups;
    groups.push_back({charCodes[0], charCodes[0], glyphIndices[0]});
    std::size_t index = 0;
    SequentalMapGroupRecord *ptr = &groups[index];

    for (std::size_t i = 1, size = charCodes.size(); i < size; ++i){
      if (charCodes[i] == ptr->mEndCharCode + 1 && glyphIndices[i] == ptr->mStartGlyphID + ptr->mEndCharCode - ptr->mStartCharCode + 1)
        ++ptr->mEndCharCode;
      else{
        groups.push_back({charCodes[i], charCodes[i], glyphIndices[i]});
        ++index;
        ptr = &groups[index];
      }
    }

    Table12 cmap12;
    cmap12.mFormat = Format12;
    cmap12.mReserved = 0;
    cmap12.mLength = sizeof(Table12) + groups.size() * sizeof(SequentalMapGroupRecord);
    cmap12.mLanguage = mLanguage;
    cmap12.mNumGroups = groups.size();

    Endian const endian = getEndian();
    if (endian == EndianLittle){
      for (auto &elem : groups){
        swapByteOrder(elem.mStartCharCode);
        swapByteOrder(elem.mEndCharCode);
        swapByteOrder(elem.mStartGlyphID);
      }

      swapByteOrder(cmap12.mFormat);
      swapByteOrder(cmap12.mLength);
      swapByteOrder(cmap12.mLanguage);
      swapByteOrder(cmap12.mNumGroups);
    }

    stream->write(&cmap12, sizeof(Table12), 1);
    stream->write(groups.data(), sizeof(SequentalMapGroupRecord), groups.size());
  }

  void CmapFormat12::toGlyphIndices(void const *charCodes, uint64_t count, std::vector<uint16_t> &glyphIndices, uint16_t defaultValue) const{
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

  void CmapFormat12::toCharCodes(void const *glyphIndices, uint64_t count, pdfout::UString &charCodes, uint32_t defaultValue) const{
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

  void CmapFormat12::toText(void const *glyphIndices, uint64_t count, pdfout::UString &text, uint32_t defaultValue) const{
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

  std::unique_ptr<Cmap> CmapFormat12::createSubset(std::vector<uint32_t> const &cids) const{
    std::unique_ptr<CmapFormat12> cmap = std::make_unique<CmapFormat12>(getPlatformId(), getEncodingId());
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

  std::unique_ptr<Cmap> CmapFormat12::createSuperset(Cmap const *cmap) const{
    std::unique_ptr<CmapFormat12> superset = std::make_unique<CmapFormat12>(*this);

    CmapFormat12 const *cmap12 = (CmapFormat12 const *) cmap;
    if (cmap12){
      for (auto const &elem : cmap12->mCharToGlyph)
        superset->mCharToGlyph[elem.first] = elem.second;
    }

    superset->makeGlyphToChar();

    return superset;
  }

  std::unique_ptr<ToUnicodeCMapStream> CmapFormat12::toUnicodeCMap(TablePost *post) const{
    NOT_IMPLEMENTED();
  }

  std::unique_ptr<ObjectStream> CmapFormat12::CIDtoGID(void) const{
    NOT_IMPLEMENTED();
  }

  std::unique_ptr<ObjectArray> CmapFormat12::toW(std::unordered_map<uint16_t, pdfout::CIDMetrics> const &cidMetrics, float scale){
    NOT_IMPLEMENTED();
  }

  void CmapFormat12::getCIDMetrics(TableHmtx *hmtx, TableGlyf *glyf, std::unordered_map<uint16_t, pdfout::CIDMetrics> &cidMetrics, uint16_t defaultCID) const{
    NOT_IMPLEMENTED();
  }

  std::unique_ptr<Cmap> CmapFormat12::clone(void) const{
    return std::make_unique<CmapFormat12>(*this);
  }

  void CmapFormat12::makeGlyphToChar(void){
    if (!mGlyphToChar.empty())
      return;

    mGlyphToChar.reserve(mCharToGlyph.size());

    for (auto const &elem : mCharToGlyph)
      mGlyphToChar[elem.second] = elem.first;
  }

}}}
