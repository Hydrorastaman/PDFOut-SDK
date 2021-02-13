#pragma once

#include <unordered_map>

#include <font/TrueType/Cmap.h>

namespace kernel{ namespace font{ namespace truetype{

  class CmapFormat0 : public Cmap{
  public:
    CmapFormat0(uint16_t platformId, uint16_t encodingId);
    CmapFormat0(CmapFormat0 const &obj);
    ~CmapFormat0(void);

    void load(pdfout::InputStream *stream);
    void save(pdfout::OutputStream *stream) const;
    void toGlyphIndices(void const *charCodes, uint64_t count, std::vector<uint16_t> &glyphIndices, uint16_t defaultValue) const;
    void toCharCodes(void const *glyphIndices, uint64_t count, pdfout::UString &charCodes, uint32_t defaultValue) const;
    void toText(void const *glyphIndices, uint64_t count, pdfout::UString &text, uint32_t defaultValue) const;
    std::unique_ptr<ToUnicodeCMapStream> toUnicodeCMap(TablePost *post) const;
    std::unique_ptr<ObjectStream> CIDtoGID(void) const;
    std::unique_ptr<ObjectArray> toW(std::unordered_map<uint16_t, pdfout::CIDMetrics> const &cidMetrics, float scale);
    void getCIDMetrics(TableHmtx *hmtx, TableGlyf *glyf, std::unordered_map<uint16_t, pdfout::CIDMetrics> &cidMetrics, uint16_t defaultCID) const;

    std::unique_ptr<Cmap> createSubset(std::vector<uint32_t> const &cids) const;
    std::unique_ptr<Cmap> createSuperset(Cmap const *cmap) const;

    std::unique_ptr<Cmap> clone(void) const;

  private:
    void makeGlyphToChar(void);

  private:
    CmapFormat0 &operator=(CmapFormat0 const &) = delete;

  private:
    uint8_t mGlyphIndices[256]; // char to glyph mapping, i.e. mGlyphIndices[charId] == glyphId
    std::unordered_map<uint8_t, uint8_t> mGlyphToChar;
  };

}}}
