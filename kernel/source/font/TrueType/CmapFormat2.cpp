#include <font/TrueType/CmapFormat2.h>

#include <InternalTools.h>

namespace{
  PACK(struct Table2{
    uint16_t mFormat;   /**< Format number is set to 2 */
    uint16_t mLength;   /**< This is the length in bytes of the subtable */
    uint16_t mLanguage; /**< Please see “Note on the language field in 'cmap' subtables“ in https://www.microsoft.com/typography/otspec/cmap.htm#language */
  });
}

namespace kernel{ namespace font{ namespace truetype{

  CmapFormat2::CmapFormat2(uint16_t platformId, uint16_t encodingId)
    : Cmap(Format2, platformId, encodingId){
  }

  CmapFormat2::~CmapFormat2(void) {}

  void CmapFormat2::load(pdfout::InputStream *stream){
    NOT_IMPLEMENTED();
  }

  void CmapFormat2::save(pdfout::OutputStream *stream) const{
    NOT_IMPLEMENTED();
  }

  void CmapFormat2::toGlyphIndices(void const *charCodes, uint64_t count, std::vector<uint16_t> &glyphIndices, uint16_t defaultValue) const{
    NOT_IMPLEMENTED();
  }

  void CmapFormat2::toCharCodes(void const *glyphIndices, uint64_t count, pdfout::UString &charCodes, uint32_t defaultValue) const{
    NOT_IMPLEMENTED();
  }

  void CmapFormat2::toText(void const *glyphIndices, uint64_t count, pdfout::UString &text, uint32_t defaultValue) const{
    NOT_IMPLEMENTED();
  }

  std::unique_ptr<Cmap> CmapFormat2::createSubset(std::vector<uint32_t> const &cids) const{
    NOT_IMPLEMENTED();
  }

  std::unique_ptr<Cmap> CmapFormat2::createSuperset(Cmap const *cmap) const{
    NOT_IMPLEMENTED();
  }

  std::unique_ptr<ToUnicodeCMapStream> CmapFormat2::toUnicodeCMap(TablePost *post) const{
    NOT_IMPLEMENTED();
  }

  std::unique_ptr<ObjectStream> CmapFormat2::CIDtoGID(void) const{
    NOT_IMPLEMENTED();
  }

  std::unique_ptr<ObjectArray> CmapFormat2::toW(std::unordered_map<uint16_t, pdfout::CIDMetrics> const &cidMetrics, float scale){
    NOT_IMPLEMENTED();
  }

  void CmapFormat2::getCIDMetrics(TableHmtx *hmtx, TableGlyf *glyf, std::unordered_map<uint16_t, pdfout::CIDMetrics> &cidMetrics, uint16_t defaultCID) const{
    NOT_IMPLEMENTED();
  }

  std::unique_ptr<Cmap> CmapFormat2::clone(void) const{
    NOT_IMPLEMENTED();
  }

}}}
