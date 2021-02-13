#include <graphics/color/ColorSpaceDevice.h>
#include <stream/OutputStream.h>
#include <object/ObjectIndirectReference.h>

using namespace pdfout;

namespace kernel{ namespace graphics{ namespace color{

  ColorSpaceDeviceGray::ColorSpaceDeviceGray(void)
    : ColorSpaceImpl(ColorSpaceFamilyDeviceGray){}

  ColorSpaceDeviceGray::~ColorSpaceDeviceGray(void) {}

  std::unique_ptr<kernel::ObjectIndirectReference> ColorSpaceDeviceGray::serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const{
    return nullptr;
  }


  ColorSpaceDeviceRGB::ColorSpaceDeviceRGB(void)
    : ColorSpaceImpl(ColorSpaceFamilyDeviceRGB){
  }

  ColorSpaceDeviceRGB::~ColorSpaceDeviceRGB(void) {}

 std::unique_ptr<kernel::ObjectIndirectReference> ColorSpaceDeviceRGB::serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const{
    return nullptr;
  }

  ColorSpaceDeviceCMYK::ColorSpaceDeviceCMYK(void)
    : ColorSpaceImpl(ColorSpaceFamilyDeviceCMYK){
  }

  ColorSpaceDeviceCMYK::~ColorSpaceDeviceCMYK(void) {}

  std::unique_ptr<kernel::ObjectIndirectReference> ColorSpaceDeviceCMYK::serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const{
    return nullptr;
  }

}}}
