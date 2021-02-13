#include <ColorSpace.h>

#include <memory>

#include <UserObjectManager.h>
#include <graphics/color/ColorSpaceCIEBased.h>
#include <graphics/color/ColorSpaceDevice.h>
#include <graphics/color/ColorspaceIndexed.h>

namespace{
  kernel::graphics::color::ColorSpaceDeviceGray defaultGray;
  kernel::graphics::color::ColorSpaceDeviceRGB defaultRGB;
  kernel::graphics::color::ColorSpaceDeviceCMYK defaultCMYK;
}

namespace kernel{
  typedef UserObjectManager<pdfout::impl::ColorSpaceImpl> ColorSpaceManager;
}

using namespace pdfout;
using namespace pdfout::impl;
using namespace kernel;
using namespace kernel::graphics::color;

namespace pdfout{

  ColorSpace *ColorSpace::createDeviceGray(void){
    return &defaultGray;
  }

  ColorSpace *ColorSpace::createDeviceRGB(void){
    return &defaultRGB;
  }

  ColorSpace *ColorSpace::createDeviceCMYK(void){
    return &defaultCMYK;
  }

  ColorSpace *ColorSpace::createCalGray(ColorWhitePoint const &whitePoint, ColorBlackPoint const &blackPoint, float gamma){
    return ColorSpaceManager::getInstance().insert(std::unique_ptr<ColorSpaceImpl>(new ColorSpaceCalGray(whitePoint, blackPoint, gamma)));
  }

  ColorSpace *ColorSpace::createCalRGB(ColorWhitePoint const &whitePoint, ColorBlackPoint const &blackPoint, ColorGamma const &gamma, ColorCalMatrix const &matrix){
    return ColorSpaceManager::getInstance().insert(std::unique_ptr<ColorSpaceImpl>(new ColorSpaceCalRGB(whitePoint, blackPoint, gamma, matrix)));
  }

  ColorSpace *ColorSpace::createLab(ColorWhitePoint const &whitePoint, ColorBlackPoint const &blackPoint, ColorLabRange const &range){
    return ColorSpaceManager::getInstance().insert(std::unique_ptr<ColorSpaceImpl>(new ColorSpaceLab(whitePoint, blackPoint, range)));
  }

  ColorSpace *ColorSpace::createICC(ColorICCComponents components){
    return ColorSpaceManager::getInstance().insert(std::unique_ptr<ColorSpaceImpl>(new ColorSpaceICC(components)));
  }

  ColorSpace *ColorSpace::createIndexed(ColorSpaceFamily base, int32_t hival, uint8_t const *color, uint32_t components){
    return ColorSpaceManager::getInstance().insert(std::unique_ptr<ColorSpaceImpl>(new ColorSpaceIndexed(base, hival, color, components)));
  }

}
