#pragma once

#include <vector>
#include <memory>
#include <cstdint>

#include <Tools.h>

namespace pdfout{
  class InputStream;
  enum WritingMode;
}

namespace kernel{ namespace font{ namespace truetype{

  class FontTrueType;

  class FontTrueTypeCollection{
  public:
    FontTrueTypeCollection(pdfout::InputStream *stream);
    ~FontTrueTypeCollection(void);

    uint32_t getNumFonts(void) const;

    /**
     * Creates TrueType font from TrueType Collection
     * @param index [in, required] zero-based index in range [0 .. numFonts)
     */
    std::unique_ptr<FontTrueType> createFont(uint32_t index) const;

  private:
    FontTrueTypeCollection(FontTrueTypeCollection const &) = delete;
    FontTrueTypeCollection &operator=(FontTrueTypeCollection const &) = delete;

  private:
    mutable pdfout::InputStream *mStream;
    std::vector<uint32_t> mOffsetTable;
  };

}}}
