#pragma once

#include <font/TrueType/Cmap.h>

namespace kernel{ namespace font{ namespace truetype{

  class CmapFormat2 : public Cmap{
  public:
    CmapFormat2(uint16_t platformId, uint16_t encodingId);
    ~CmapFormat2(void);

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
    CmapFormat2(CmapFormat2 const &) = delete;
    CmapFormat2 &operator=(CmapFormat2 const &) = delete;
  };

}}}
