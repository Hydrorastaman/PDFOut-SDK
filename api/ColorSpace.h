#pragma once

#include "PdfOutConfig.h"

namespace pdfout{

  enum ColorSpaceFamily{
    ColorSpaceFamilyDeviceGray = 0, // PDF 1.1
    ColorSpaceFamilyDeviceRGB,      // PDF 1.1
    ColorSpaceFamilyDeviceCMYK,     // PDF 1.1
    ColorSpaceFamilyCalGray,        // PDF 1.1
    ColorSpaceFamilyCalRGB,         // PDF 1.1
    ColorSpaceFamilyLab,            // PDF 1.1
    ColorSpaceFamilyICCBased,       // PDF 1.3
    ColorSpaceFamilyPattern,        // PDF 1.2
    ColorSpaceFamilyIndexed,        // PDF 1.1
    ColorSpaceFamilySeparation,     // PDF 1.2
    ColorSpaceFamilyDeviceN         // PDF 1.3
  };

  struct PDFOUT_EXPORTS ColorWhitePoint{
    ColorWhitePoint(void) : mX(1.0), mY(1.0), mZ(1.0) {}
    ColorWhitePoint(float x, float z) : mX(x), mY(1.0), mZ(z) {}

    float mX;
    float mY;
    float mZ;
  };

  struct PDFOUT_EXPORTS ColorBlackPoint{
    ColorBlackPoint(void) : mX(0.0), mY(0.0), mZ(0.0) {}
    ColorBlackPoint(float x, float y, float z) : mX(x), mY(y), mZ(z) {}

    float mX;
    float mY;
    float mZ;
  };

  struct PDFOUT_EXPORTS ColorGamma{
    ColorGamma(void) : mX(1.0), mY(1.0), mZ(1.0) {}
    ColorGamma(float x, float y, float z) : mX(x), mY(y), mZ(z) {}

    float mX;
    float mY;
    float mZ;
  };

  struct PDFOUT_EXPORTS ColorCalMatrix{
    ColorCalMatrix(void) : xA(1.0), yA(0.0), zA(0.0), xB(0.0), yB(1.0), zB(0.0), xC(0.0), yC(0.0), zC(1.0) {}
    float xA; float yA; float zA;
    float xB; float yB; float zB;
    float xC; float yC; float zC; 
  };

  struct PDFOUT_EXPORTS ColorLabRange{
    ColorLabRange(void) : aMin(-100.0), aMax(100.0), bMin(-100.0), bMax(100.0) {}
    float aMin;
    float aMax;
    float bMin;
    float bMax;
  };

  enum ColorICCComponents{
    ColorICCComponents1 = 1,
    ColorICCComponents3 = 3,
    ColorICCComponents4 = 4
  };

  class PDFOUT_EXPORTS ColorSpace{
  protected:
    ColorSpace(void) {}
    virtual ~ColorSpace(void) {}

  public:
    virtual ColorSpaceFamily getFamily(void) const = 0;

    static ColorSpace *createDeviceGray(void);
    static ColorSpace *createDeviceRGB(void);
    static ColorSpace *createDeviceCMYK(void);
    static ColorSpace *createCalGray(ColorWhitePoint const &whitePoint, ColorBlackPoint const &blackPoint = ColorBlackPoint(), float gamma = 1.f);
    static ColorSpace *createCalRGB(ColorWhitePoint const &whitePoint, ColorBlackPoint const &blackPoint = ColorBlackPoint(), ColorGamma const &gamma = ColorGamma(), ColorCalMatrix const &matrix = ColorCalMatrix());
    static ColorSpace *createLab(ColorWhitePoint const &whitePoint, ColorBlackPoint const &blackPoint = ColorBlackPoint(), ColorLabRange const &range = ColorLabRange());
    static ColorSpace *createICC(ColorICCComponents components);
    static ColorSpace *createIndexed(ColorSpaceFamily base, int32_t hival, uint8_t const *color, uint32_t components);

  private:
    ColorSpace(ColorSpace const &);
    ColorSpace &operator=(ColorSpace const &);
  };

}
