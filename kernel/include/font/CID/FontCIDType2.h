#pragma once

#include <font/CompositeFont.h>
#include <font/FontCIDDictionary.h>
#include <font/FontType0Dictionary.h>

namespace kernel{ namespace font{ namespace truetype{
  class FontTrueType;
}}}

namespace kernel{ namespace font{ namespace cid{

  using kernel::font::truetype::FontTrueType;

  /**
   * CID font based on TrueType font
   */
  class FontCIDType2 : public CompositeFont{
  public:
    FontCIDType2(FontTrueType const *descendandFont, pdfout::WritingMode writingMode);
    ~FontCIDType2(void) noexcept;

    std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, SerializeParams *params) const;

  private:
    std::unique_ptr<FontType0Dictionary> makeFontDictionary(void) const;
    std::unique_ptr<FontCIDDictionary> makeCIDFontDictionary(void) const;

  private:
    FontCIDType2(FontCIDType2 const &) = delete;
    FontCIDType2 &operator=(FontCIDType2 const &) = delete;
  };

}}}
