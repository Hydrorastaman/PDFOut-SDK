#pragma once

#include <memory>
#include <cstdint>

#include <InternalTools.h>
#include <font/TrueType/Table.h>

namespace pdfout{

  class InputStream;
  class OutputStream;

}

namespace kernel{ namespace font{ namespace truetype{

  class TableGlyf;

  /**
   * @brief TrueType font maximum profile, "maxp"
   */
  PACK(struct TableMaxpData{
    int32_t mVersion;                /**< Table version number 0x00010000 for version 1.0. */
    uint16_t mNumGlyphs;             /**< The number of glyphs in the font. */
    uint16_t mMaxPoints;             /**< Maximum points in a non_composite glyph. */
    uint16_t mMaxContours;           /**< Maximum contours in a non_composite glyph. */
    uint16_t mMaxCompositePoints;    /**< Maximum points in a composite glyph. */
    uint16_t mMaxCompositeContours;  /**< Maximum contours in a composite glyph. */
    uint16_t mMaxZones;              /**< 1 if instructions do not use the twilight zone (Z0), or 2 if instructions do use Z0; should be set to 2 in most cases. */
    uint16_t mMaxTwilightPoints;     /**< Maximum points used in Z0. */
    uint16_t mMaxStorage;            /**< Number of Storage Area locations. */
    uint16_t mMaxFunctionDefs;       /**< Number of FDEFs. */
    uint16_t mMaxInstructionDefs;    /**< Number of IDEFs. */
    uint16_t mMaxStackElements;      /**< Maximum stack depth2. */
    uint16_t mMaxSizeOfInstructions; /**< Maximum byte count for glyph instructions. */
    uint16_t mMaxComponentElements;  /**< Maximum number of components referenced at “top level” for any composite glyph. */
    uint16_t mMaxComponentDepth;     /**< Maximum levels of recursion; 1 for simple components. */
  });

  class TableMaxp : public Table{
  public:
    TableMaxp(void);
    ~TableMaxp(void);

    void load(pdfout::InputStream *stream);
    void save(pdfout::OutputStream *stream) const;

    std::unique_ptr<TableMaxp> createSubset(std::vector<uint16_t> const &gids, FontInfo const &fontInfo) const;
    std::unique_ptr<TableMaxp> createSuperset(TableMaxp const *maxp) const;

  private:
    //TableMaxp(TableMaxp const &) = delete;
    TableMaxp &operator=(TableMaxp const &) = delete;

  public:
    TableMaxpData mData;
  };

}}}
