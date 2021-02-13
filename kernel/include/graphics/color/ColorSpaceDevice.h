#pragma once

#include <memory>
#include <ColorSpaceImpl.h>

namespace kernel{ namespace graphics{ namespace color{

  class ColorSpaceDeviceGray : public pdfout::impl::ColorSpaceImpl{
  public:
    ColorSpaceDeviceGray(void);
    ~ColorSpaceDeviceGray(void);

    std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

  private:
    ColorSpaceDeviceGray(ColorSpaceDeviceGray const &) = delete;
    ColorSpaceDeviceGray &operator=(ColorSpaceDeviceGray const &) = delete;
  };

  class ColorSpaceDeviceRGB : public pdfout::impl::ColorSpaceImpl{
  public:
    ColorSpaceDeviceRGB(void);
    ~ColorSpaceDeviceRGB(void);

    std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

  private:
    ColorSpaceDeviceRGB(ColorSpaceDeviceRGB const &) = delete;
    ColorSpaceDeviceRGB &operator=(ColorSpaceDeviceRGB const &) = delete;
  };

  class ColorSpaceDeviceCMYK : public pdfout::impl::ColorSpaceImpl{
  public:
    ColorSpaceDeviceCMYK(void);
    ~ColorSpaceDeviceCMYK(void);

    std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

  private:
    ColorSpaceDeviceCMYK(ColorSpaceDeviceCMYK const &) = delete;
    ColorSpaceDeviceCMYK &operator=(ColorSpaceDeviceCMYK const &) = delete;
  };

}}}

