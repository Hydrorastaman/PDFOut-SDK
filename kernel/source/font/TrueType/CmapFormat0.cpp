#include <font/TrueType/CmapFormat0.h>

#include <memory>

#include <UString.h>
#include <Exception.h>
#include <InternalTools.h>
#include <font/TrueType/TablePost.h>

using namespace pdfout;

namespace{
  PACK(struct Table0{
    uint16_t mFormat;   /**< Format number is set to 0 */
    uint16_t mLength;   /**< This is the length in bytes of the subtable */
    uint16_t mLanguage; /**< See "Note on the language field in 'cmap' subtables" in https://www.microsoft.com/typography/otspec/cmap.htm#language */
  });
}

namespace kernel{ namespace font{ namespace truetype{

  CmapFormat0::CmapFormat0(uint16_t platformId, uint16_t encodingId)
    : Cmap(Format0, platformId, encodingId){
    memset(mGlyphIndices, 0, 256 * sizeof(uint8_t));
  }

  CmapFormat0::CmapFormat0(CmapFormat0 const &obj)
    : Cmap(obj){
    memcpy(mGlyphIndices, obj.mGlyphIndices, 256 * sizeof(uint8_t));
  }

  CmapFormat0::~CmapFormat0(void) {}

  void CmapFormat0::load(pdfout::InputStream *stream){
    Table0 cmap0;
    stream->read(&cmap0, sizeof(Table0), 1);

    if (getEndian() == EndianLittle){
      swapByteOrder(cmap0.mFormat);
      swapByteOrder(cmap0.mLength);
      swapByteOrder(cmap0.mLanguage);
    }

    if (cmap0.mFormat != Format0)
      RUNTIME_EXCEPTION("invalid cmap format");

    mLanguage = cmap0.mLanguage;

    if (!cmap0.mLength)
        return;

    std::size_t const remainSize = cmap0.mLength - sizeof(Table0);
    stream->read(mGlyphIndices, sizeof(uint8_t), remainSize);

    makeGlyphToChar();
  }

  void CmapFormat0::save(pdfout::OutputStream *stream) const{
    Table0 cmap0;
    cmap0.mFormat = Format0;
    cmap0.mLanguage = mLanguage;
    cmap0.mLength = sizeof(Table0) + 256 * sizeof(uint8_t);

    if (getEndian() == EndianLittle){
      swapByteOrder(cmap0.mFormat);
      swapByteOrder(cmap0.mLength);
      swapByteOrder(cmap0.mLanguage);
    }

    stream->write(&cmap0, sizeof(Table0), 1);
    stream->write(mGlyphIndices, sizeof(uint8_t), 256);
  }

  void CmapFormat0::toGlyphIndices(void const *charCodes, uint64_t count, std::vector<uint16_t> &glyphIndices, uint16_t defaultValue) const{
    uint8_t const *chars = (uint8_t const *) charCodes;
    glyphIndices.resize(count);
    for (auto i = 0; i < count; ++i){
      if (chars[i] < 256)
        glyphIndices[i] = mGlyphIndices[chars[i]];
      else
        glyphIndices[i] = defaultValue;
    }
  }

  void CmapFormat0::toCharCodes(void const *glyphIndices, uint64_t count, pdfout::UString &charCodes, uint32_t defaultValue) const{
    uint8_t const *glyphs = (uint8_t const *) glyphIndices;
    std::vector<uint8_t> chars(count);

    auto charCode = std::end(mGlyphToChar);
    auto const end = std::end(mGlyphToChar);
    for (auto i = 0; i < count; ++i) {
      charCode = mGlyphToChar.find(glyphs[i]);
      if (charCode != end)
        chars[i] = charCode->second;
      else
        chars[i] = defaultValue;
    }

    charCodes = UString(chars.data(), chars.size() * sizeof(uint8_t), Encoding_ASCII);
  }

  void CmapFormat0::toText(void const *glyphIndices, uint64_t count, pdfout::UString &text, uint32_t defaultValue) const{
    return toCharCodes(glyphIndices, count, text, defaultValue);
  }

  std::unique_ptr<Cmap> CmapFormat0::createSubset(std::vector<uint32_t> const &cids) const{
    std::unique_ptr<CmapFormat0> cmap = std::make_unique<CmapFormat0>(getPlatformId(), getEncodingId());
    memset(cmap->mGlyphIndices, 0, 256);

    for (std::size_t i = 0; i < cids.size(); ++i){
      uint32_t const cid = cids[i];
      if (cid < 256)
        cmap->mGlyphIndices[cid] = i;
    }

    //for (std::size_t i = 0; i < gids.size(); ++i){
    //  auto cid = mGlyphToChar.find(gids[i]);
    //  if (cid != std::end(mGlyphToChar))
    //    cmap->mGlyphIndices[cid->second] = i;
    //}

    cmap->makeGlyphToChar();

    return cmap;
  }

  std::unique_ptr<Cmap> CmapFormat0::createSuperset(Cmap const *cmap) const{
    std::unique_ptr<CmapFormat0> superset = std::make_unique<CmapFormat0>(*this);

    CmapFormat0 const *cmap0 = (CmapFormat0 const *) cmap;
    if (cmap0){
      for (std::size_t i = 0; i < 256; ++i){
        if (cmap0->mGlyphIndices[i] && !superset->mGlyphIndices[i])
          superset->mGlyphIndices[i] = cmap0->mGlyphIndices[i];
      }
    }

    superset->makeGlyphToChar();

    return superset;
  }

  void CmapFormat0::makeGlyphToChar(void){
    if (!mGlyphToChar.empty())
      return;

    mGlyphToChar.reserve(256);

    for (auto i = 0; i < 256; ++i)
      mGlyphToChar[mGlyphIndices[i]] = i;
  }

  std::unique_ptr<ToUnicodeCMapStream> CmapFormat0::toUnicodeCMap(TablePost *post) const{
    std::unique_ptr<ToUnicodeCMapStream> toUnicodeCMapStream = std::make_unique<ToUnicodeCMapStream>();

    UnicodeInfo uniInfo = {0};
    uint16_t glyphNameId = 0;
    std::vector<uint32_t> values;
    for (std::size_t i = 0; i < 256; ++i){
      post->getUnicode(mGlyphIndices[i], uniInfo);
      toUnicodeCMapStream->addBfChar(i, uniInfo);
    }

    return toUnicodeCMapStream;
  }

  std::unique_ptr<ObjectStream> CmapFormat0::CIDtoGID(void) const{
    NOT_IMPLEMENTED();
  }

  std::unique_ptr<ObjectArray> CmapFormat0::toW(std::unordered_map<uint16_t, pdfout::CIDMetrics> const &cidMetrics, float scale){
    NOT_IMPLEMENTED();
  }

  void CmapFormat0::getCIDMetrics(TableHmtx *hmtx, TableGlyf *glyf, std::unordered_map<uint16_t, pdfout::CIDMetrics> &cidMetrics, uint16_t defaultCID) const{
    NOT_IMPLEMENTED();
  }

  std::unique_ptr<Cmap> CmapFormat0::clone(void) const{
    return std::make_unique<CmapFormat0>(*this);
  }

}}}
