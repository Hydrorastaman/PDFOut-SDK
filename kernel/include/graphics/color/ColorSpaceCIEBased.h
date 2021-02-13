#pragma once

#include <ColorSpaceImpl.h>
#include <graphics/color/ColorICCStream.h>
#include <memory>

using namespace pdfout;

namespace kernel{ namespace graphics{ namespace color{

  class ColorSpaceCalGray : public pdfout::impl::ColorSpaceImpl{
  public:
    ColorSpaceCalGray(ColorWhitePoint const &whitePoint);
    ColorSpaceCalGray(ColorWhitePoint const &whitePoint, ColorBlackPoint const &blackPoint, float gamma);
    ~ColorSpaceCalGray(void) {}

    ColorWhitePoint const &getWhitePoint(void) const {return mWhitePoint;}
    ColorBlackPoint const &getBlackPoint(void) const {return mBlackPoint;}
    float getGamma(void) const {return mGamma;}

    std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

  private:
    ColorSpaceCalGray(ColorSpaceCalGray const &) = delete;
    ColorSpaceCalGray &operator=(ColorSpaceCalGray const &) = delete;

  private:
    ColorWhitePoint mWhitePoint;
    ColorBlackPoint mBlackPoint;
    float mGamma;
  };

  class ColorSpaceCalRGB : public pdfout::impl::ColorSpaceImpl{
  public:
    ColorSpaceCalRGB(ColorWhitePoint const &whitePoint);
    ColorSpaceCalRGB(ColorWhitePoint const &whitePoint, ColorBlackPoint const &blackPoint, ColorGamma const &gamma, ColorCalMatrix const &matrix);
    ~ColorSpaceCalRGB(void) {}

    ColorWhitePoint getWhitePoint(void) const {return mWhitePoint;}
    ColorBlackPoint getBlackPoint(void) const {return mBlackPoint;}
    ColorGamma getGamma(void) const {return mGamma;}
    ColorCalMatrix getMatrix(void) const {return mMatrix;}

    std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

  private:
    ColorSpaceCalRGB(ColorSpaceCalRGB const &) = delete;
    ColorSpaceCalRGB &operator=(ColorSpaceCalRGB const &) = delete;

  private:
    ColorWhitePoint mWhitePoint;
    ColorBlackPoint mBlackPoint;
    ColorGamma mGamma;
    ColorCalMatrix mMatrix;
  };

  class ColorSpaceLab : public pdfout::impl::ColorSpaceImpl{
  public:
    ColorSpaceLab(ColorWhitePoint const &whitePoint);
    ColorSpaceLab(ColorWhitePoint const &whitePoint, ColorBlackPoint const &blackPoint, ColorLabRange const &range);
    ~ColorSpaceLab(void) {}

    ColorWhitePoint getWhitePoint(void) const {return mWhitePoint;}
    ColorBlackPoint getBlackPoint(void) const {return mBlackPoint;}
    ColorLabRange getRange(void) const {return mRange;}

    std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

  private:
    ColorSpaceLab(ColorSpaceLab const &) = delete;
    ColorSpaceLab &operator=(ColorSpaceLab const &) = delete;

  private:
    ColorWhitePoint mWhitePoint;
    ColorBlackPoint mBlackPoint;
    ColorLabRange mRange;
  };

  class ColorSpaceICC : public pdfout::impl::ColorSpaceImpl{
  public:
    ColorSpaceICC(ColorICCComponents components);
    ~ColorSpaceICC(void) {}

    std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

  private:
    ColorSpaceICC(ColorSpaceICC const &obj) = delete;
    ColorSpaceICC &operator=(ColorSpaceICC const &) = delete;

  private:
    std::unique_ptr<ColorICCStream> mStream;
  };

}}}
