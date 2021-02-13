#pragma once

#include <memory>

#include <Font.h>

namespace pdfout{
  class InputStream;
  class OutputStream;
}

namespace kernel{
  class ObjectIndirectReference;
  struct SerializeParams;
}

namespace kernel{ namespace font{

  class SimpleFont : public pdfout::FontMetrics{
  protected:
    SimpleFont(void);

  public:
    virtual ~SimpleFont(void) noexcept;

    FontMetrics::getTextWidth;
    virtual int32_t getTextWidth(pdfout::UString const &text, std::vector<int32_t> &widths) const = 0;

    /**
     * Serializes object to stream as a PDF object.
     */
    virtual std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const = 0;

    /**
     * Serialize object to stream as a font program.
     */
    virtual void saveToStream(pdfout::OutputStream *stream) const = 0;

    /**
     * Creates new subset font with selected CIDs.
     */
    virtual std::unique_ptr<SimpleFont> createSubset(std::vector<uint32_t> const &cids) const = 0;

    /**
     * Creates new superset font with selected font.
     */
    virtual std::unique_ptr<SimpleFont> createSuperset(SimpleFont const *font) const = 0;

    /**
     * Creates new simple font from stream.
     */
    static std::unique_ptr<SimpleFont> createSimpleFont(pdfout::InputStream *stream);

  private:
    SimpleFont(SimpleFont const &) = delete;
    SimpleFont &operator=(SimpleFont const &) = delete;
  };

}}
