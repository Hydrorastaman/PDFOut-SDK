#include <graphics/ExtGraphicsState.h>

using namespace pdfout;

namespace kernel{

  ExtGraphicsState::ExtGraphicsState(void)
    : mLineWidth(1.f),
    mLineCap(LineCapButt),
    mLineJoin(LineJoinMiter),
    mMiterLimit(10.f),
    //mDashPattern(),
    mRenderingIntent(RenderingIntentRelativeColorimetric),
    mStrokingOverprint(false),
    mOverprint(false),
    mOverprintMode(OverprintModeZero),
    mFlatness(1.f),
    mSmoothness(1.f),
    mStrokeAdjustment(false),
    mBlendMode(BlendModeNormal){
  }
}
