#pragma once

#include <font/CompositeFont.h>

namespace kernel{ namespace font{ namespace cff{
  class FontCFF;
}}}

namespace kernel{ namespace font{ namespace cid{

  using kernel::font::cff::FontCFF;

  /**
   * CID font based on CFF font
   */
  class FontCIDType0 : public CompositeFont{
  public:
    FontCIDType0(FontCFF const *descendandFont, pdfout::WritingMode writingMode);
    ~FontCIDType0(void) noexcept;

    std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, SerializeParams *params) const;

  private:
    FontCIDType0(FontCIDType0 const &) = delete;
    FontCIDType0 &operator=(FontCIDType0 const &) = delete;
  };

}}}
