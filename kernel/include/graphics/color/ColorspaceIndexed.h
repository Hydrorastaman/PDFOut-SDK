#pragma once

#include <vector>

#include <ColorSpaceImpl.h>

using namespace pdfout;

namespace kernel{ namespace graphics{ namespace color{

  class ColorSpaceIndexed : public pdfout::impl::ColorSpaceImpl{
  public:
    ColorSpaceIndexed(ColorSpaceFamily base, int32_t hival, uint8_t const *color, uint32_t components);
    ~ColorSpaceIndexed(void);

    std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

  private:
    ColorSpaceIndexed(ColorSpaceIndexed const &) = delete;
    ColorSpaceIndexed &operator=(ColorSpaceIndexed const &) = delete;

  private:
    ColorSpaceFamily mBase;
    int32_t mHival;
    std::vector<uint8_t> mColor;
  };

}}}
