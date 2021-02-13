#pragma once

#include <vector>
#include <memory>
#include <cstdint>

#include <font/TrueType/Glyph.h>
#include <font/TrueType/Table.h>
#include <font/TrueType/TableLoca.h>

namespace pdfout{

  class InputStream;
  class OutputStream;

}

namespace kernel{ namespace font{ namespace truetype{

  class TableGlyf : public Table{
  private:
    struct TableGlyfInfo{
      uint32_t mOffset;
      uint32_t mLength;
      std::unique_ptr<Glyph> mGlyph;
    };

  public:
    TableGlyf(TableLoca const *tableLoca);
    TableGlyf(TableGlyf const &obj);
    ~TableGlyf(void);

    void load(pdfout::InputStream *stream);
    void save(pdfout::OutputStream *stream) const;

    std::unique_ptr<TableGlyf> createSubset(std::vector<uint16_t> const &gids) const;
    std::unique_ptr<TableGlyf> createSuperset(TableGlyf const *glyf, TableLoca const *loca) const;

    void extractCompoundGids(std::vector<uint16_t> const &gids, std::vector<uint16_t> &extraGids) const;
    void updateFontInfo(FontInfo &fontInfo) const;

    Glyph const *getGlyph(uint16_t gid) const;

  private:
    uint16_t getCompoundMaxPointsNum(uint16_t gid) const;
    uint16_t getCompoundMaxContours(uint16_t gid) const;
    uint16_t getComponentDepth(uint16_t gid) const;

  private:
    TableGlyf &operator=(TableGlyf const &) = delete;

  private:
    TableLoca const *mTableLoca;
    std::vector<TableGlyfInfo> mGlyphs;
  };

}}}
