#pragma once

#include <unordered_map>

#include <stream/InputStream.h>
#include <stream/OutputStream.h>

namespace kernel{ namespace font{ namespace truetype{

  enum TrueTypeTable{
    TrueTypeTableAcnt = 0, // (accent attachment) table
    TrueTypeTableAnkr, // (anchor point) table
    TrueTypeTableAvar, // (axis variation) table
    TrueTypeTableBdat, // (bitmap data) table
    TrueTypeTableBhed, // (bitmap font header) table
    TrueTypeTableBloc, // (bitmap location) table
    TrueTypeTableBsln, // (baseline) table
    TrueTypeTableCmap, // (character code mapping) table
    TrueTypeTableCvar, // (CVT variation) table
    TrueTypeTableCvt , // (control value) table
    TrueTypeTableEBSC, // (embedded bitmap scaling control) table
    TrueTypeTableFdsc, // (font descriptor) table
    TrueTypeTableFeat, // (layout feature) table
    TrueTypeTableFmtx, // (font metrics) table
    TrueTypeTableFond, // (font family compatibility) table
    TrueTypeTableFpgm, // (font program) table
    TrueTypeTableFvar, // (font variation) table
    TrueTypeTableGasp, // (grid-fitting and scan-conversion procedure) table
    TrueTypeTableGlyf, // (glyph outline) table
    TrueTypeTableGvar, // (glyph variation) table
    TrueTypeTableHdmx, // (horizontal device metrics) table
    TrueTypeTableHead, // (font header) table
    TrueTypeTableHhea, // (horizontal header) table
    TrueTypeTableHmtx, // (horizontal metrics) table
    TrueTypeTableJust, // (justification) table
    TrueTypeTableKern, // (kerning) table
    TrueTypeTableKerx, // (extended kerning) table
    TrueTypeTableLcar, // (ligature caret) table
    TrueTypeTableLoca, // (glyph location) table
    TrueTypeTableLtag, // (language tag) table
    TrueTypeTableMaxp, // (maximum profile) table
    TrueTypeTableMeta, // (metadata) table
    TrueTypeTableMort, // (metamorphosis) table (deprecated)
    TrueTypeTableMorx, // (extended metamorphosis) table
    TrueTypeTableName, // (name) table
    TrueTypeTableOpbd, // (optical bounds) table
    TrueTypeTableOS2,  // (compatibility) table
    TrueTypeTablePost, // (glyph name and PostScript compatibility) table
    TrueTypeTablePrep, // (control value program) table
    TrueTypeTableProp, // (properties) table
    TrueTypeTableSbix, // (extended bitmaps) table
    TrueTypeTableTrak, // (tracking) table
    TrueTypeTableVhea, // (vertical header) table
    TrueTypeTableVmtx, // (vertical metrics) table
    TrueTypeTableXref, // (cross-reference) table
    TrueTypeTableZapf, // (glyph reference) table
    TrueTypeTableCFF,  // PostScript font program (compact font format)
    TrueTypeTableVORG, // Vertical Origin
    TrueTypeTableEBDT, // Embedded bitmap data
    TrueTypeTableEBLC, // Embedded bitmap location data
    TrueTypeTableBASE, // Baseline data
    TrueTypeTableGDEF, // Glyph definition data
    TrueTypeTableGPOS, // Glyph positioning data
    TrueTypeTableGSUB, // Glyph substitution data
    TrueTypeTableJSTF, // Justification data
    TrueTypeTableDSIG, // Digital signature
    TrueTypeTableLTSH, // Linear threshold data
    TrueTypeTablePCLT, // PCL 5 data
    TrueTypeTableVDMX, // Vertical device metrics
    TrueTypeTableDttf, // Delta TrueType Font Table (Microsoft subset fonts)
    TrueTypeTableInvalid // should be the last element in the enum
  };

  TrueTypeTable getTableId(char tag[4]);
  void tableIdToTag(TrueTypeTable id, char *tag);

  struct GlyphInfo{
    // from TableGlyf
    int16_t mXMax;
    int16_t mXMin;
    int16_t mYMax;
    int16_t mYMin;
    uint16_t mPoints;
    int16_t mContours;
    uint16_t mCompositePoints;
    uint16_t mCompositeContours;
    uint16_t mComponentElements;
    uint16_t mComponentDepth;
    uint16_t mSizeOfInstructions;

    // from TableHmtx
    int16_t mLSB;
    int16_t mRSB;
    int16_t mAdvWidth;
    int16_t mXExtent;
  };

  typedef std::unordered_map<uint16_t, GlyphInfo> FontInfo;

  class Table{
  public:
    Table(void);
    virtual ~Table(void);

    virtual void load(pdfout::InputStream *stream) = 0;
    virtual void save(pdfout::OutputStream *stream) const = 0;

  private:
    //Table(Table const &) = delete;
    Table &operator=(Table const &) = delete;
  };

  /**
   * @brief TrueType table version info
   */
  enum TableVersion{
    TableVersion10 = 0x00010000,  /**< for version 1.0 */
    TableVersion20 = 0x00020000,  /**< for version 2.0 */
    TableVersion25 = 0x00025000,  /**< for version 2.5 */
    TableVersion30 = 0x00030000,  /**< for version 3.0 */
    TableVersion40 = 0x00040000,  /**< for version 4.0 */
    TableVersionOTTO = 0x4F54544F /**< for OpenType with CFF v1 or v2 */
  };

}}}
