#pragma once

#include <memory>
#include <cstdint>

#include <object/ObjectIndirectReference.h>

namespace pdfout{
  class OutputStream;
}

namespace kernel{
  struct SerializeParams;
}

namespace kernel{ namespace graphics{ namespace image{

  class SoftMaskImage{
  public:
    SoftMaskImage(uint32_t width, uint32_t height, uint8_t transparency);
    ~SoftMaskImage(void);

    std::unique_ptr<ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

  private:
    SoftMaskImage(SoftMaskImage const &) = delete;
    SoftMaskImage &operator=(SoftMaskImage const &) = delete;

  private:
    uint32_t mWidth;
    uint32_t mHeight;
    uint8_t mTransparency;
  };

}}}
