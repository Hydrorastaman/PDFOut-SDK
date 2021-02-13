#pragma once

#include <vector>
#include <unordered_map>

#include <font/TrueType/Cmap.h>
#include <InternalTools.h>

namespace kernel{ namespace font{ namespace truetype{
  class CmapFormat6 : public Cmap {
    PACK(struct Table6 {
      uint16_t mFormat;     /**< Format number is set to 6 */
      uint16_t mLength;     /**< Length in bytes */
      uint16_t mLanguage;   /**< Language code(see above) */
      uint16_t mFirstCode;  /**< First character code of subrange */
      uint16_t mEntryCount; /**< Number of character codes in subrange */
      //uint16_t mGlyphIndexArray[mEntryCount]; /**< Array of glyph index values for character codes in the range */
    });

  public:
    CmapFormat6(uint16_t platformId, uint16_t encodingId);
    ~CmapFormat6(void);

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
    CmapFormat6(CmapFormat6 const &) = delete;
    CmapFormat6 &operator=(CmapFormat6 const &) = delete;

  private:
    Table6 mCmap6;
    std::vector<uint16_t> mGlyphIndices;
    std::unordered_map<uint16_t, uint16_t> mGlyphToChar;
  };
}}}
