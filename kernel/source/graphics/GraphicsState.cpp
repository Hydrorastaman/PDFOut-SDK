#include <graphics/GraphicsState.h>

using namespace pdfout;

namespace kernel{

  GraphicsState::GraphicsState(void)
    : mCTM(),
    mLineWidth(1.f),
    mLineCap(LineCapButt),
    mLineJoin(LineJoinMiter),
    mMiterLimit(10.f),
    //mDashPattern(),
    mRenderingIntent(RenderingIntentRelativeColorimetric),
    mFlatness(1.f),
    mStrokingColorSpace(ColorSpace::createDeviceGray()),
    mColorSpace(ColorSpace::createDeviceGray()),
    mStrokingColor(Color::createGray(ColorNameBlack)),
    mColor(Color::createGray(ColorNameBlack)),
    mColorRGB(Color::createGray(ColorNameBlack)),
    mColorCMYK(Color::createGray(ColorNameBlack)),
    mColorGray(Color::createGray(ColorNameBlack)),
    mTextState(),
    mExtGState(){
  }

  GraphicsState *GraphicsState::clone(void) const{
    return new GraphicsState(*this);
  }
}
