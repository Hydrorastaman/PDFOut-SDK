#pragma once

#include <vector>

#include "Types.h"
#include "UString.h"
#include "Encoding.h"
#include "PdfOutConfig.h"
#include "FontCollection.h"
#include "stream/InputStream.h"
#include "stream/OutputStream.h"

namespace pdfout{

  enum StandardType1Fonts{
    StandardType1FontsTimesRoman,
    StandardType1FontsTimesBold,
    StandardType1FontsTimesItalic,
    StandardType1FontsTimesBoldItalic,
    StandardType1FontsHelvetica,
    StandardType1FontsHelveticaBold,
    StandardType1FontsHelveticaOblique,
    StandardType1FontsHelveticaBoldOblique,
    StandardType1FontsCourier,
    StandardType1FontsCourierBold,
    StandardType1FontsCourierOblique,
    StandardType1FontsCourierBoldOblique,
    StandardType1FontsSymbol,
    StandardType1FontsZapfDingbats
  };

  class PDFOUT_EXPORTS FontMetrics{
  protected:
    FontMetrics(void) {}

  public:
    virtual ~FontMetrics(void) {}

    /**
     * Gets font revision string
     */
    virtual UString getRevision(void) const = 0;

    /**
     * Gets PostScript name.
     */
    virtual UString getPostScriptName(void) const = 0;

    /**
     * Gets family name.
     */
    virtual UString getFamilyName(void) const = 0;

    /**
     * Gets sub-family name.
     */
    virtual UString getSubFamilyName(void) const = 0;

    /**
     * Saves font to stream as a font program.
     */
    virtual void saveToStream(pdfout::OutputStream *stream) const = 0;

    /**
     * Converts glyphs sequence to char codes, may contain unreadable characters.
     */
    virtual void toCharCodes(void const *glyphIndices, uint16_t elemSize, uint64_t count, pdfout::UString &charCodes, uint32_t defaultValue = 0) const = 0;

    /**
     * Converts glyphs sequence to readable text.
     */
    virtual void toText(void const *glyphIndices, uint16_t elemSize, uint64_t count, pdfout::UString &text, uint32_t defaultValue = 0) const = 0;

    /**
     * Gets text widths in design units.
     * DeviceUnits = (DesignUnits/unitsPerEm) * (PointSize/72) * DeviceResolution
     * @param text [in] The text string
     * @return The total width of the text string
     */
    virtual int32_t getTextWidth(pdfout::UString const &text) const = 0;

    /**
     * Gets character width in design units.
     * DeviceUnits = (DesignUnits/unitsPerEm) * (PointSize/72) * DeviceResolution
     * @param cid [in] CID
     * @return The CID width
     */
    virtual int32_t getCharWidth(uint16_t cid) const = 0;
    virtual void getCIDMetrics(uint16_t cid, CIDMetrics &metrics) const = 0;

    /**
     * Gets kerning widths between two CIDs in design units.
     * DeviceUnits = (DesignUnits/unitsPerEm) * (PointSize/72) * DeviceResolution
     * @param cidLhs [in] The lefthand CID in the kerning pair
     * @param cidRhs [in] The righthand CID in the kerning pair
     * @return The kerning width
     */
    virtual int32_t getKerningWidth(uint16_t cidLhs, uint16_t cidRhs) const = 0;

    /**
     * Gets font line gap value
     */
    virtual int32_t getLineGap(void) const = 0;

    /**
     * Gets font ascent value
     */
    virtual int32_t getAscent(void) const = 0;

    /**
     * Gets font descent value
     */
    virtual int32_t getDescent(void) const = 0;

    /**
     * Gets font cap height
     */
    virtual int32_t getCapHeight(void) const = 0;

    /**
     * Gets units per Em
     */
    virtual int32_t getUnitsPerEm(void) const = 0;

    virtual int32_t getWinAscent(void) const = 0;
    virtual int32_t getWinDescent(void) const = 0;
    virtual int32_t getWinInternalLeading(void) const = 0;
    virtual int32_t getWinExternalLeading(void) const = 0;
    virtual int32_t getBreakChar(void) const = 0;
    virtual int32_t getDefaultChar(void) const = 0;
    virtual int32_t getUnderlineYSize(void) const = 0;

    virtual bool isBold(void) const = 0;
    virtual bool isItalic(void) const = 0;
    virtual bool isUnderline(void) const = 0;
    virtual bool isShadow(void) const = 0;
    virtual bool isCondensed(void) const = 0;
    virtual bool isExtended(void) const = 0;

  private:
    FontMetrics(FontMetrics const &);
    FontMetrics &operator=(FontMetrics const &);
  };

  class PDFOUT_EXPORTS Font : public FontMetrics{
  protected:
    Font(void) : FontMetrics() {}

  public:
    ~Font(void) {}

    /**
     * Appends font path to the current font path.
     */
    static void addFontPath(pdfout::UString const &fontPath);

    /**
     * Sets font path. Removes previous changes.
     */
    static void setFontPath(pdfout::UString const &fontPath);

    /**
     * Creates font from file.
     */
    static Font *createFont(pdfout::UString const &fileName, WritingMode writingMode = WritingModeHorizontal);

    /**
     * Creates font from memory.
     */
    static Font *createFont(void const *fontData, uint64_t size, WritingMode writingMode = WritingModeHorizontal);

    /**
     * Creates font from stream.
     */
    static Font *createFont(InputStream *istream, WritingMode writingMode = WritingModeHorizontal);

    static FontCollection *createFontCollection(pdfout::UString const &fileName, WritingMode writingMode = WritingModeHorizontal);
    static FontCollection *createFontCollection(void const *fontData, uint64_t size, WritingMode writingMode = WritingModeHorizontal);
    static FontCollection *createFontCollection(InputStream *istream, WritingMode writingMode = WritingModeHorizontal);

    /**
     * Gets font object version.
     */
    virtual uint16_t getVersion(void) const = 0;

    /**
     * Gets writing mode.
     */
    virtual WritingMode getWritingMode(void) const = 0;

    /**
     * Merges delta font with existing one.
     */
    virtual Font *merge(Font const *delta) const = 0;

  private:
    Font(Font const &);
    Font &operator=(Font const &);
  };
}
