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

  /**
   * @brief TrueType font horizontal header, "hhea"
   */
  PACK(struct TableHheaData{
    int32_t mVersion;             /**< Table version number  0x00010000 for version 1.0. */
    int16_t mAscender;            /**< Typographic ascent. (Distance from baseline of highest ascender) */
    int16_t mDescender;           /**< Typographic descent. (Distance from baseline of lowest descender) */
    int16_t mLineGap;             /**< Typographic line gap. Negative LineGap values are treated as zero in Windows 3.1, System 6, and System 7. */
    uint16_t mAdvanceWidthMax;    /**< Maximum advance width value in 'hmtx' table. */
    int16_t mMinLeftSideBearing;  /**< Minimum left sidebearing value in 'hmtx' table. */
    int16_t mMinRightSideBearing; /**< Minimum right sidebearing value; calculated as Min(aw - lsb - (xMax - xMin)). */
    int16_t mXMaxExtent;          /**< Max(lsb + (xMax - xMin)). */
    int16_t mCaretSlopeRise;      /**< Used to calculate the slope of the cursor (rise/run); 1 for vertical. */
    int16_t mCaretSlopeRun;       /**< 0 for vertical. */
    int16_t mCaretOffset;         /**< The amount by which a slanted highlight on a glyph needs to be shifted to produce the best appearance. Set to 0 for non-slanted fonts */
    int16_t mReserved[4];         /**< Reserved, set to 0 */
    int16_t mMetricDataFormat;    /**< 0 for current format. */
    uint16_t mNumberOfHMetrics;   /**< Number of hMetric entries in 'hmtx' table */
  });
  
  class TableHhea : public Table{
  public:
    TableHhea(void);
    ~TableHhea(void);

    void load(pdfout::InputStream *stream);
    void save(pdfout::OutputStream *stream) const;

    uint16_t getNumberOfHMetrics(void) const;
    int16_t getAscender(void) const;
    int16_t getDescender(void) const;
    int16_t getLineGap(void) const;

    std::unique_ptr<TableHhea> createSubset(std::vector<uint16_t> const &gids, FontInfo const &fontInfo) const;
    std::unique_ptr<TableHhea> createSuperset(TableHhea const *hhea) const;

  private:
    //TableHhea(TableHhea const &) = delete;
    TableHhea &operator=(TableHhea const &) = delete;

  public:
    TableHheaData mData;
  };

}}}
