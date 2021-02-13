#pragma once

#include <deque>
#include <vector>
#include <cstdint>
#include <unordered_map>

#include <InternalTools.h>
#include <object/ObjectArray.h>
#include <object/ObjectStream.h>

#include <font/CMapName.h>
#include <font/SimpleFont.h>
#include <font/TrueType/Table.h>
#include <font/FontCIDDictionary.h>
#include <font/FontType0Dictionary.h>
#include <font/ToUnicodeCMapStream.h>
#include <font/FontDescriptorDictionary.h>

namespace pdfout{
  class InputStream;
  class OutputStream;
  class MemoryOStream;
}

namespace kernel{ namespace font{ namespace truetype{

  class Cmap;
  class TableHead;
  class TableHhea;
  class TableCmap;
  class TableGlyf;
  class TableHmtx;
  class TableLoca;
  class TableMaxp;
  class TableName;
  class TableOS2 ;
  class TablePost;
  class TableDttf;
  class TableKern;

  using pdfout::UString;

  /**
   * Implementation for physical TrueType font
   */
  class FontTrueType : public SimpleFont{
  private:
    /**
     * @brief TrueType table record entry
     */
    PACK(struct TrueTypeTableRecord{
      char mTag[4];       /**< 4-byte identifier */
      uint32_t mCheckSum; /**< CheckSum for this table */
      uint32_t mOffset;   /**< Offset from beginning of TrueType font file */
      uint32_t mLength;   /**< Length of this table in bytes */
    });

  public:
    FontTrueType(pdfout::InputStream *stream);
    ~FontTrueType(void);

    /**
     * FontMetrics interface implementation
     */

    UString getRevision(void) const;
    UString getPostScriptName(void) const;
    UString getFamilyName(void) const;
    UString getSubFamilyName(void) const;

    void saveToStream(pdfout::OutputStream *stream) const;

    void toCharCodes(void const *glyphIndices, uint16_t elemSize, uint64_t count, pdfout::UString &charCodes, uint32_t defaultValue) const;
    void toText(void const *glyphIndices, uint16_t elemSize, uint64_t count, pdfout::UString &text, uint32_t defaultValue) const;

    int32_t getTextWidth(pdfout::UString const &text, std::vector<int32_t> &widths) const;
    int32_t getTextWidth(pdfout::UString const &text) const;
    int32_t getCharWidth(uint16_t ch) const;
    void getCIDMetrics(uint16_t cid, pdfout::CIDMetrics &metrics) const;
    int32_t getKerningWidth(uint16_t cidLhs, uint16_t cidRhs) const;

    int32_t getLineGap(void) const;
    int32_t getAscent(void) const;
    int32_t getDescent(void) const;
    int32_t getCapHeight(void) const;
    int32_t getUnitsPerEm(void) const;

    int32_t getWinAscent(void) const;
    int32_t getWinDescent(void) const;
    int32_t getWinInternalLeading(void) const;
    int32_t getWinExternalLeading(void) const;
    int32_t getBreakChar(void) const;
    int32_t getDefaultChar(void) const;
    int32_t getUnderlineYSize(void) const;

    bool isBold(void) const;
    bool isItalic(void) const;
    bool isUnderline(void) const;
    bool isShadow(void) const;
    bool isCondensed(void) const;
    bool isExtended(void) const;

    std::unique_ptr<SimpleFont> createSubset(std::vector<uint32_t> const &cids) const;
    std::unique_ptr<SimpleFont> createSuperset(SimpleFont const *font) const;

    std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, SerializeParams *params) const;

    // For CID-based font serialization
    std::unique_ptr<ObjectStream> cid2gid(void) const;
    std::unique_ptr<ObjectArray> toW(void) const;
    std::unique_ptr<ToUnicodeCMapStream> toUnicode(void) const;
    std::unique_ptr<FontDescriptorDictionary> makeFontDescriptor(void) const;

  private:
    FontTrueType(void);
    FontTrueType(FontTrueType const &) = delete;
    FontTrueType &operator=(FontTrueType const &) = delete;

  private:
    bool enumTables(pdfout::InputStream *stream);
    TableHead *getTableHead(void) const;
    TableHhea *getTableHhea(void) const;
    TableCmap *getTableCmap(void) const;
    TableGlyf *getTableGlyf(void) const;
    TableHmtx *getTableHmtx(void) const;
    TableLoca *getTableLoca(void) const;
    TableMaxp *getTableMaxp(void) const;
    TableName *getTableName(void) const;
    TableOS2  *getTableOS2(void) const;
    TablePost *getTablePost(void) const;
    TableDttf *getTableDttf(void) const;
    TableKern *getTableKern(void) const;

    void setTableHead(std::unique_ptr<TableHead> table);
    void setTableHhea(std::unique_ptr<TableHhea> table);
    void setTableCmap(std::unique_ptr<TableCmap> table);
    void setTableGlyf(std::unique_ptr<TableGlyf> table);
    void setTableHmtx(std::unique_ptr<TableHmtx> table);
    void setTableLoca(std::unique_ptr<TableLoca> table);
    void setTableMaxp(std::unique_ptr<TableMaxp> table);
    void setTableName(std::unique_ptr<TableName> table);
    void setTableOS2(std::unique_ptr<TableOS2> table);
    void setTablePost(std::unique_ptr<TablePost> table);
    void setTableDttf(std::unique_ptr<TableDttf> table);
    void setTableKern(std::unique_ptr<TableKern> table);

    uint16_t getMacStyle(void) const;

    bool loadFontInfo(pdfout::InputStream *stream);
    uint32_t calculateChecksum(uint32_t *table, uint32_t length) const;

    template <typename T>
    bool loadTable(pdfout::InputStream *stream, TrueTypeTable tableTag){
      auto tableRecord = mTablesOffset.find(tableTag);
      if (tableRecord == std::end(mTablesOffset))
        return false;

      stream->seekg(tableRecord->second.mOffset);
      std::unique_ptr<T> table = std::make_unique<T>();
      table->load(stream);
      mTables[tableTag] = std::move(table);
      return true;
    }

    pdfout::MemoryOStream *saveTable(TrueTypeTable tableTag, TrueTypeTableRecord &record) const;

    template <typename T>
    void toGlyphIndices(Cmap *cmap, uint16_t firstChar, uint16_t lastChar, std::vector<uint32_t> &glyphs) const{
      std::size_t const size = lastChar - firstChar + 1;
      std::vector<T> charCodes(size);
      for (int i = 0; i < size; ++i)
        charCodes[i] = firstChar + i;

      cmap->toGlyphIndices(charCodes.data(), charCodes.size(), glyphs);
    }

    void makeCIDMetrics(void);

  private:
    std::unordered_map<int, TrueTypeTableRecord> mTablesOffset;
    std::vector<std::unique_ptr<Table>> mTables;
    std::unordered_map<uint16_t, pdfout::CIDMetrics> mCIDMetrics;
  };

}}}
