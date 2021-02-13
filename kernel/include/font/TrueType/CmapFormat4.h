#pragma once

#include <map>
#include <unordered_map>

#include <UString.h>
#include <font/TrueType/Cmap.h>

namespace kernel{ namespace font{ namespace truetype{

  class CmapFormat4 : public Cmap{
  public:
    CmapFormat4(uint16_t platformId, uint16_t encodingId);
    CmapFormat4(CmapFormat4 const &obj);
    ~CmapFormat4(void);

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
    bool isSequental(std::size_t from, std::size_t to, std::vector<uint16_t> const &indices) const;

  private:
    CmapFormat4 &operator=(CmapFormat4 const &) = delete;

  private:
    mutable std::unordered_map<uint16_t, uint16_t> mCharToGlyph;
    std::unordered_map<uint16_t, uint16_t> mGlyphToChar;
    std::vector<uint16_t> mGidToGid; // mGidToGid[subset GID] == Source GID mapping for subsets
  };

}}}
