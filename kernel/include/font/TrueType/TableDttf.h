#pragma once

#include <cstdint>
#include <vector>

#include <InternalTools.h>
#include <font/TrueType/Table.h>

namespace pdfout{

  class InputStream;
  class OutputStream;

}

namespace kernel{ namespace font { namespace truetype {

  /**
  * @brief TrueType delta table, "dttf".
  * Affects the interpretation of: loca, hmtx, vmtx, hdmx and LTSH tables.
  */
  PACK(struct TableDttfData {
    int32_t mVersion;             /**< Table version number  0x00010000 for version 1.0. */
    int32_t mCheckSum;            /**< Checksum of table */
    uint16_t mOriginalNumGlyphs;  /**< Original number of glyphs in the font */
    uint16_t mMaxGlyphIndexUsed;  /**< Maximal glyph index in the table */
    uint16_t mFormat;             /**< Table format. */
    uint16_t mFFlags;             /**< unknown. */
    uint16_t mGlyphCount;         /**< Glyphs count in the table. */
    //uint16_t mGlyphIndexArray[mGlyphCount]; /**< The GlyphIndexArray indirection.  */
  });

  class TableDttf : public Table {
  public:
    TableDttf(void);
    ~TableDttf(void);

    void load(pdfout::InputStream *stream);
    void save(pdfout::OutputStream *stream) const;

  private:
    TableDttf(TableDttf const &) = delete;
    TableDttf &operator=(TableDttf const &) = delete;

  public:
    TableDttfData mData;
    std::vector<uint16_t> mGlyphIndexArray;
  };

}}}
