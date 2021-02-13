#pragma once

#include <cstdint>
#include <vector>

#include <UString.h>
#include <object/ObjectArray.h>
#include <font/ToUnicodeCMapStream.h>

namespace pdfout{
  class InputStream;
  class OutputStream;
}

namespace kernel{ namespace font{ namespace truetype{

  enum Format{
    Format0 = 0,   /**< Byte encoding table */
    Format2 = 2,   /**< High-byte mapping through table */
    Format4 = 4,   /**< Segment mapping to delta values */
    Format6 = 6,   /**< Trimmed table mapping */
    Format8 = 8,   /**< Mixed 16-bit and 32-bit coverage */
    Format10 = 10, /**< Trimmed array */
    Format12 = 12, /**< Segmented coverage */
    Format13 = 13, /**< Many-to-one range mappings */
    Format14 = 14, /**< Unicode Variation Sequences */
  };

  class TableHmtx;
  class TablePost;
  class TableGlyf;

  class Cmap{
  public:
    explicit Cmap(Format format, uint16_t platformId, uint16_t encodingId);
    virtual ~Cmap(void);

    Format getFormat(void) const;
    uint16_t getLanguage(void) const;
    uint16_t getPlatformId(void) const;
    uint16_t getEncodingId(void) const;

    virtual void load(pdfout::InputStream *stream) = 0;
    virtual void save(pdfout::OutputStream *stream) const = 0;
    virtual void toGlyphIndices(void const *charCodes, uint64_t count, std::vector<uint16_t> &glyphIndices, uint16_t defaultValue = 0) const = 0;
    virtual void toCharCodes(void const *glyphIndices, uint64_t count, pdfout::UString &charCodes, uint32_t defaultValue = 0) const = 0;
    virtual void toText(void const *glyphIndices, uint64_t count, pdfout::UString &text, uint32_t defaultValue = 0) const = 0;

    virtual std::unique_ptr<ToUnicodeCMapStream> toUnicodeCMap(TablePost *post) const = 0;
    virtual std::unique_ptr<ObjectStream> CIDtoGID(void) const = 0;
    virtual std::unique_ptr<ObjectArray> toW(std::unordered_map<uint16_t, pdfout::CIDMetrics> const &cidMetrics, float scale) = 0;
    virtual void getCIDMetrics(TableHmtx *hmtx, TableGlyf *glyf, std::unordered_map<uint16_t, pdfout::CIDMetrics> &cidMetrics, uint16_t defaultCID) const = 0;

    /**
     * Creates cmap table for incoming set of charCodes.
     * @param cids [in] character ID for subset
     * @return returns pointer to Cmap object for incoming subset
     */
    virtual std::unique_ptr<Cmap> createSubset(std::vector<uint32_t> const &cids) const = 0;

    /**
     * Merge cmap table with incoming cmap
     * @param cmap [in] pointer to Cmap object for merging
     */
    virtual std::unique_ptr<Cmap> createSuperset(Cmap const *cmap) const = 0;

    virtual std::unique_ptr<Cmap> clone(void) const = 0;
  private:
    //Cmap(Cmap const &) = delete;
    Cmap &operator=(Cmap const &) = delete;

  private:
    Format mFormat;

  protected:
    uint16_t mLanguage;
    uint16_t mPlatformId;
    uint16_t mEncodingId;
  };

}}}
