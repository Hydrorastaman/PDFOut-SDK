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

  enum MacStyle{
    MacStyleBold      = 0x1,  /**< Bold */
    MacStyleItalic    = 0x2,  /**< Italic */
    MacStyleUnderline = 0x4,  /**< Underline */
    MacStyleOutline   = 0x8,  /**< Outline */
    MacStyleShadow    = 0x10, /**< Shadow */
    MacStyleCondensed = 0x20, /**< Condensed */
    MacStyleExtended  = 0x40, /**< Extended */
  };

  /**
   * @brief TrueType font header, "head"
   */

  PACK(struct TableHeadData{
    int32_t mVersion;             /**< Table version number 0x00010000 for version 1.0 */
    int32_t mFontRevision;        /**< Set by font manufacturer */
    uint32_t mCheckSumAdjustment; /**< To compute: set it to 0, sum the entire font as ULONG, then store 0xB1B0AFBA - sum */
    uint32_t mMagicNumber;        /**< Set to 0x5F0F3CF5 */
    uint16_t mFlags;              /**< Bit 0: Baseline for font at y=0;
                                    *  Bit 1: Left sidebearing point at x=0;
                                    *  Bit 2: Instructions may depend on point size;
                                    *  Bit 3: Force ppem to integer values for all internal scaler math; may use fractional ppem sizes if this bit is clear;
                                    *  Bit 4: Instructions may alter advance width (the advance widths might not scale linearly);
                                    *  Bits 5-10: These should be set according to Apple's specification . However, they are not implemented in OpenType.
                                    *  Bit 11: Font data is 'lossless,' as a result of having been compressed and decompressed with the Agfa MicroType Express engine.
                                    *  Bit 12: Font converted (produce compatible metrics)
                                    *  Bit 13: Font optimized for ClearType™. Note, fonts that rely on embedded bitmaps (EBDT) for rendering should not be considered optimized for ClearType, and therefore should keep this bit cleared.
                                    *  Bit 14: Last Resort font. If set, indicates that the glyphs encoded in the cmap subtables are simply generic symbolic representations of code point ranges and don't truly represent support for those code points. If unset, indicates that the glyphs encoded in the cmap subtables represent proper support for those code points.
                                    *  Bit 15: Reserved, set to 0
                                    */
    uint16_t mUnitsPerEm;         /**< Valid range is from 16 to 16384. This value should be a power of 2 for fonts that have TrueType outlines. */
    int64_t mCreated;             /**< Number of seconds since 12:00 midnight, January 1, 1904. 64-bit integer */
    int64_t mModified;            /**< Number of seconds since 12:00 midnight, January 1, 1904. 64-bit integer */
    int16_t mXMin;                /**< For all glyph bounding boxes. */
    int16_t mYMin;                /**< For all glyph bounding boxes. */
    int16_t mXMax;                /**< For all glyph bounding boxes. */
    int16_t mYMax;                /**< For all glyph bounding boxes. */
    uint16_t mMacStyle;           /**< Bit 0: Bold (if set to 1);
                                    *  Bit 1: Italic (if set to 1)
                                    *  Bit 2: Underline (if set to 1)
                                    *  Bit 3: Outline (if set to 1)
                                    *  Bit 4: Shadow (if set to 1)
                                    *  Bit 5: Condensed (if set to 1)
                                    *  Bit 6: Extended (if set to 1)
                                    *  Bits 7-15: Reserved (set to 0).
                                    */
    uint16_t mLowestRecPPEM;      /**< Smallest readable size in pixels. */
    int16_t mFontDirectionHint;   /**< Deprecated (Set to 2).
                                    *  0: Fully mixed directional glyphs;
                                    *  1: Only strongly left to right;
                                    *  2: Like 1 but also contains neutrals;
                                    *  -1: Only strongly right to left;
                                    *  -2: Like -1 but also contains neutrals. 1
                                    */
    int16_t mIndexToLocFormat;    /**< 0 for short offsets, 1 for long. */
    int16_t mGlyphDataFormat;     /**< 0 for current format. */
  });

  class TableHead : public Table{
  public:
    TableHead(void);
    ~TableHead(void);

    void load(pdfout::InputStream *stream);
    void save(pdfout::OutputStream *stream) const;

    uint16_t getMacStyle(void) const;
    int16_t getXMax(void) const;
    int16_t getXMin(void) const;

    struct pdfout::Rectangle getBBox(void) const;
    uint16_t getUnitsPerEm(void) const;

    std::unique_ptr<TableHead> createSubset(FontInfo const &fontInfo) const;
    std::unique_ptr<TableHead> createSuperset(TableHead const *head) const;

  private:
    //TableHead(TableHead const &) = delete;
    TableHead &operator=(TableHead const &) = delete;

  public:
    TableHeadData mData;
  };

}}}
