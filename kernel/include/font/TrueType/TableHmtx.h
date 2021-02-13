#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <unordered_map>

#include <InternalTools.h>
#include <stream/InputStream.h>
#include <stream/OutputStream.h>
#include <font/TrueType/Table.h>

namespace kernel{ namespace font{ namespace truetype{

  PACK(struct LongHorMetrics{
    uint16_t mAdvanceWidth;
    int16_t mLsb;
  });

  class TableDttf;

  /**
   * @brief TrueType font horizontal metrix, "hmtx"
   */
  class TableHmtx : public Table{
  public:
    TableHmtx(uint16_t numOfMetrics, uint16_t numGlyphs);
    ~TableHmtx(void);

    void load(pdfout::InputStream *stream);
    void save(pdfout::OutputStream *stream) const;

    void update(TableDttf *dttf);
    std::unique_ptr<TableHmtx> createSubset(std::vector<uint16_t> const &gids) const;
    std::unique_ptr<TableHmtx> createSuperset(TableHmtx const *hmtx) const;

    void updateFontInfo(FontInfo &fontInfo) const;

  private:
    //TableHmtx(TableHmtx const &) = delete;
    TableHmtx &operator=(TableHmtx const &) = delete;

  public:
    uint16_t mNumOfMetrics;
    uint16_t mNumGlyphs;
    std::vector<LongHorMetrics> mHMetrix;   /**< Paired advance width and left side bearing values for each glyph. The value numOfHMetrics comes from the 'hhea' table.
                                                If the font is monospaced, only one entry need be in the array, but that entry is required. The last entry applies to all subsequent glyphs */
    std::vector<int16_t> mLeftSideBearing; /**< Here the advanceWidth is assumed to be the same as the advanceWidth for the last entry above.
                                                The number of entries in this array is derived from numGlyphs (from 'maxp' table) minus numberOfHMetrics.
                                                This generally is used with a run of monospaced glyphs (e.g., Kanji fonts or Courier fonts). Only one run is allowed and it must be at the end.
                                                This allows a monospaced font to vary the left side bearing values for each glyph. */
  };

}}}
