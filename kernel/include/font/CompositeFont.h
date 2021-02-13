#pragma once

#include <memory>
#include <string>

#include <Types.h>
#include <font/CMapName.h>

namespace pdfout{
  class OutputStream;
}

namespace kernel{
  class ObjectIndirectReference;
  struct SerializeParams;
}

namespace kernel{ namespace font{

  class SimpleFont;

  class CompositeFont{
  protected:
    CompositeFont(SimpleFont const *descendantFont, pdfout::WritingMode writingMode);

  public:
    virtual ~CompositeFont(void) noexcept;

    /**
     * Gets descendant font.
     */
    SimpleFont const *getDescendantFont(void) const;

    /**
     * Gets font writing mode.
     */
    pdfout::WritingMode getWritingMode(void) const;

    /**
     * Serializes object to stream as a PDF object.
     */
    virtual std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const = 0;

    /**
     * Creates new composite font.
     */
    static std::unique_ptr<CompositeFont> createCompositeFont(SimpleFont const *descendantFont, pdfout::WritingMode writingMode);

  protected:
    CMapName getCMapName(void) const;

  private:
    CompositeFont(CompositeFont const &) = delete;
    CompositeFont &operator=(CompositeFont const &) = delete;

  private:
    std::string mUID;
    SimpleFont const *mDescendantFont;
    pdfout::WritingMode mWritingMode;
  };

}}
