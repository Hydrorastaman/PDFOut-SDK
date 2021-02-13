#include <font/TrueType/CmapFormat6.h>

#include <memory>
#include <stream/InputStream.h>

using namespace pdfout;

namespace kernel{ namespace font{ namespace truetype{
  CmapFormat6::CmapFormat6(uint16_t platformId, uint16_t encodingId)
    : Cmap(Format6, platformId, encodingId){
  }

  CmapFormat6::~CmapFormat6(void) {}

  void CmapFormat6::load(pdfout::InputStream *stream){
    Endian const endian = getEndian();

    stream->read(&mCmap6, sizeof(Table6), 1);

    if (endian == EndianLittle){
      swapByteOrder(mCmap6.mFormat);
      swapByteOrder(mCmap6.mLength);
      swapByteOrder(mCmap6.mLanguage);
      swapByteOrder(mCmap6.mFirstCode);
      swapByteOrder(mCmap6.mEntryCount);
    }

    if (mCmap6.mFormat != Format6)
      RUNTIME_EXCEPTION("invalid cmap format");

    mLanguage = mCmap6.mLanguage;

    mGlyphIndices.resize(mCmap6.mEntryCount);
    stream->read(mGlyphIndices.data(), sizeof(uint16_t), mCmap6.mEntryCount);
    if (endian == EndianLittle)
      for (auto &elem : mGlyphIndices)
        swapByteOrder(elem);

    makeGlyphToChar();
  }

  void CmapFormat6::save(pdfout::OutputStream *stream) const{
    Endian const endian = getEndian();

    Table6 cmap6;
    memcpy(&cmap6, &mCmap6, sizeof(Table6));
    if (endian == EndianLittle){
      swapByteOrder(cmap6.mFormat);
      swapByteOrder(cmap6.mLength);
      swapByteOrder(cmap6.mLanguage);
      swapByteOrder(cmap6.mFirstCode);
      swapByteOrder(cmap6.mEntryCount);
    }

    stream->write(&cmap6, sizeof(Table6), 1);

    uint16_t value = 0;
    for (auto const &elem : mGlyphIndices){
      value = elem;
      swapByteOrder(value);
      stream->write(&value, sizeof(uint16_t), 1);
    }
  }

  void CmapFormat6::toGlyphIndices(void const *charCodes, uint64_t count, std::vector<uint16_t> &glyphIndices, uint16_t defaultValue) const{
    uint16_t const *chars = (uint16_t const *) charCodes;
    glyphIndices.resize(count);
    uint16_t const minValue = mCmap6.mFirstCode;
    uint16_t const maxValue = mCmap6.mFirstCode + mCmap6.mEntryCount;
    for (auto i = 0; i < count; ++i) {
      if (chars[i] >= minValue && chars[i] < maxValue)
        glyphIndices[i] = mGlyphIndices[chars[i] - minValue];
      else
        glyphIndices[i] = defaultValue;
    }
  }

  void CmapFormat6::toCharCodes(void const *glyphIndices, uint64_t count, pdfout::UString &charCodes, uint32_t defaultValue) const{
    uint16_t const *glyphs = (uint16_t const *) glyphIndices;
    std::vector<uint16_t> chars(count);

    auto charCode = std::end(mGlyphToChar);
    auto const end = std::end(mGlyphToChar);
    for (auto i = 0; i < count; ++i) {
      charCode = mGlyphToChar.find(glyphs[i]);
      if (charCode != end)
        chars[i] = charCode->second;
      else
        chars[i] = defaultValue;
    }

    charCodes = UString((uint8_t *) chars.data(), chars.size() * sizeof(uint16_t), Encoding_WCHAR_T);
  }

  void CmapFormat6::toText(void const *glyphIndices, uint64_t count, pdfout::UString &text, uint32_t defaultValue) const{
    NOT_IMPLEMENTED();
  }

  std::unique_ptr<Cmap> CmapFormat6::createSubset(std::vector<uint32_t> const &cids) const{
    NOT_IMPLEMENTED();
  }

  std::unique_ptr<Cmap> CmapFormat6::createSuperset(Cmap const *cmap) const{
    NOT_IMPLEMENTED();
  }

  std::unique_ptr<ToUnicodeCMapStream> CmapFormat6::toUnicodeCMap(TablePost *post) const{
    NOT_IMPLEMENTED();
  }

  std::unique_ptr<ObjectStream> CmapFormat6::CIDtoGID(void) const{
    NOT_IMPLEMENTED();
  }

  std::unique_ptr<ObjectArray> CmapFormat6::toW(std::unordered_map<uint16_t, pdfout::CIDMetrics> const &cidMetrics, float scale){
    NOT_IMPLEMENTED();
  }

  void CmapFormat6::getCIDMetrics(TableHmtx *hmtx, TableGlyf *glyf, std::unordered_map<uint16_t, pdfout::CIDMetrics> &cidMetrics, uint16_t defaultCID) const{
    NOT_IMPLEMENTED();
  }

  std::unique_ptr<Cmap> CmapFormat6::clone(void) const{
    NOT_IMPLEMENTED();
  }

  void CmapFormat6::makeGlyphToChar(void){
    if (!mGlyphToChar.empty())
      return;

    mGlyphToChar.reserve(mGlyphIndices.size());
    std::size_t const size = mGlyphIndices.size();
    uint16_t const minValue = mCmap6.mFirstCode;
    for (auto i = 0; i < size; ++i)
      mGlyphToChar[mGlyphIndices[i]] = i + minValue;
  }

}}}
