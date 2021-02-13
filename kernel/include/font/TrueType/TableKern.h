#pragma once

#include <memory>
#include <cstdint>
#include <unordered_map>

#include <InternalTools.h>
#include <font/TrueType/Table.h>

namespace pdfout{

  class InputStream;
  class OutputStream;

}

namespace kernel{ namespace font { namespace truetype {

  /**
  * @brief TrueType kerning table for cross-platform fonts, "kern". Not compatible with Apple extension.
  */
  PACK(struct TableKernHeader{
    uint16_t mVersion; /**< The version number of the kerning table, 0x0000 */
    uint16_t mTables;  /**< Number of subtables in the kerning table. */
  });

  enum TableKernVersion{
    TableKernVersion0 = 0
  };

  PACK(struct TableKernSubHeader{
    uint16_t mVersion;  /**< Kern subtable version number */
    uint16_t mLength;   /**< Length of the subtable, in bytes (including this header). */
    uint16_t mCoverage; /**< What type of information is contained in this table. */
  });

  enum TableKernFormat{
    TableKernFormat0 = 0,
    TableKernFormat1 = 1,
    TableKernFormat2 = 2,
    TableKernFormat3 = 3
  };

  enum TableKernCoverage{
    TableKernCoverageHorizontal  = 0x0001, /**< 1 if table has horizontal data, 0 if vertical. */
    TableKernCoverageMinimum     = 0x0002, /**< If this bit is set to 1, the table has minimum values. If set to 0, the table has kerning values. */
    TableKernCoverageCrossStream = 0x0004, /**< If set to 1, kerning is perpendicular to the flow of the text. 

                                                If the text is normally written horizontally, kerning will be done in the up and down directions. If kerning
                                                values are positive, the text will be kerned upwards; if they are negative, the text will be kerned downwards.

                                                If the text is normally written vertically, kerning will be done in the left and right directions. If kerning
                                                values are positive, the text will be kerned to the right; if they are negative, the text will be kerned to the left. 

                                                The value 0x8000 in the kerning data resets the cross-stream kerning back to 0. */
    TableKernCoverageOverride    = 0x0008, /**< If this bit is set to 1 the value in this table should replace the value currently being accumulated. */
    TableKernCoverageFormat      = 0xFF00  /**< Format of the subtable. Only formats 0 and 2 have been defined. Formats 1 and 3 through 255 are reserved for future use. */
  };

  PACK(struct TableKernSubTable0{
    uint16_t mNPairs;        /**< The number of kerning pairs in this subtable. */
    uint16_t mSearchRange;   /**< The largest power of two less than or equal to the value of nPairs, multiplied by the size in bytes of an entry in the subtable. */
    uint16_t mEntrySelector; /**< This is calculated as log2 of the largest power of two less than or equal to the value of nPairs. This value indicates how many iterations of the search loop have to be made. For example, in a list of eight items, there would be three iterations of the loop. */
    uint16_t mRangeShift;    /**< The value of nPairs minus the largest power of two less than or equal to nPairs. This is multiplied by the size in bytes of an entry in the table. */
  });

  PACK(struct TableKernDataFormat0{
    uint16_t mLeft;  /**< The glyph index for the left-hand glyph in the kerning pair. */
    uint16_t mRight; /**< The glyph index for the right-hand glyph in the kerning pair. */
    int16_t  mValue; /**< The kerning value for the above pair, in FUnits. If this value is greater than zero, the characters will be moved apart. 
                          If this value is less than zero, the character will be moved closer together. */
  });

  class TableKern : public Table {
  public:
    TableKern(void);
    ~TableKern(void);

    void load(pdfout::InputStream *stream);
    void save(pdfout::OutputStream *stream) const;

    int16_t getKerningWidth(uint16_t gidLhs, uint16_t gidRhs) const;

    std::unique_ptr<TableKern> createSubset(std::vector<uint16_t> const &gids) const;
    std::unique_ptr<TableKern> createSuperset(TableKern const *kern) const;

  private:
    TableKern(TableKern const &) = delete;
    TableKern &operator=(TableKern const &) = delete;

  public:
    std::unordered_map<uint16_t, std::unordered_map<uint16_t, int16_t>> mData; // {left glyph ID : {right glyph ID : kerning}}
  };

}}}
