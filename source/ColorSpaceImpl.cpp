#include <ColorSpaceImpl.h>

#include <ResourceUid.h>

namespace pdfout{ namespace impl{

  ColorSpaceImpl::ColorSpaceImpl(ColorSpaceFamily family)
    : mColorUID(generateUID(family)),
    mFamily(family){
  }

  ColorSpaceImpl::~ColorSpaceImpl(void) {}

  std::string const &ColorSpaceImpl::getUID(void) const{
    return mColorUID;
  }

  ColorSpaceFamily ColorSpaceImpl::getFamily(void) const{
    return mFamily;
  }

  std::string ColorSpaceImpl::generateUID(ColorSpaceFamily family){
    switch (family){
      case ColorSpaceFamilyDeviceGray: return "CS_DeviceGray";
      case ColorSpaceFamilyDeviceRGB:  return "CS_DeviceRGB";
      case ColorSpaceFamilyDeviceCMYK: return "CS_DeviceCMYK";
    }

    return kernel::ResourceUid::getResourceUid(kernel::ResourceTypeColorSpace);
  }

}}
