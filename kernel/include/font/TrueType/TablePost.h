#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include <InternalTools.h>
#include <font/TrueType/Table.h>
#include <font/AdobeGlyphList.h>

namespace pdfout{

  class InputStream;
  class OutputStream;

}

namespace kernel{ namespace font{ namespace truetype{

  /**
   * @brief TrueType font PostScript header, "post"
   */
  PACK(struct TablePostData{
    int32_t mVersion;            /**< 0x00010000 for version 1.0, this TrueType-based font file contains exactly the 258 glyphs in the standard Macintosh TrueType font file 
                                      0x00020000 for version 2.0, this is the version required by TrueType-based fonts to be used on Windows
                                      0x00025000 for version 2.5 (deprecated), this version provides a space saving table for TrueType-based fonts which contain a pure subset of, or a simple reordering of, the standard Macintosh glyph set
                                      0x00030000 for version 3.0, this version is used by OpenType fonts with TrueType or CFF data */
    int32_t mItalicAngle;        /**< Italic angle in counter-clockwise degrees from the vertical. Zero for upright text, negative for text that leans to the right (forward). */
    int16_t mUnderlinePosition;  /**< This is the suggested distance of the top of the underline from the baseline (negative values indicate below baseline).
                                      The PostScript definition of this FontInfo dictionary key (the y coordinate of the center of the stroke) is not used for historical reasons.
                                      The value of the PostScript key may be calculated by subtracting half the underlineThickness from the value of this field. */
    int16_t mUnderlineThickness; /**< Suggested values for the underline thickness. */
    uint32_t mIsFixedPitch;      /**< Set to 0 if the font is proportionally spaced, non-zero if the font is not proportionally spaced (i.e. monospaced). */
    uint32_t mMinMemType42;      /**< Minimum memory usage when an OpenType font is downloaded. */
    uint32_t mMaxMemType42;      /**< Maximum memory usage when an OpenType font is downloaded. */
    uint32_t mMinMemType1;       /**< Minimum memory usage when an OpenType font is downloaded as a Type 1 font. */
    uint32_t mMaxMemType1;       /**< Maximum memory usage when an OpenType font is downloaded as a Type 1 font. */

    /** additional entries for version 2.0 and 2.5 */
    uint16_t mNumberOfGlyphs;              /**< Number of glyphs (this should be the same as numGlyphs in 'maxp' table), version 2.0, 2.5 */
    std::vector<uint16_t> mGlyphNameIndex; /**< Ordinal number of this glyph in 'post' string tables. This is not an offset, version 2.0 */
    std::vector<std::string> mNames;       /**< Glyph names, version 2.0 */
    std::vector<int8_t> mOffset;           /**< Difference between graphic index and standard order of glyph, version 2.5 */
  });

  class TablePost : public Table{
  public:
    TablePost(void);
    ~TablePost(void);

    void load(pdfout::InputStream *stream);
    void save(pdfout::OutputStream *stream) const;

    int32_t getItalicAngle(void) const;
    bool isFixedPitch(void) const;

    void getUnicode(uint16_t glyphId, UnicodeInfo &uniInfo) const;

    std::unique_ptr<TablePost> createSubset(std::vector<uint16_t> const &gids) const;

  private:
    //TablePost(TablePost const &) = delete;
    TablePost &operator=(TablePost const &) = delete;

  public:
    TablePostData mData;
  };

}}}
