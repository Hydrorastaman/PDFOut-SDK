#pragma once

#include <TransformationMatrix.h>
#include <ColorSpace.h>
#include <graphics/TextState.h>
#include <graphics/ExtGraphicsState.h>
#include <Function.h>
#include <Halftone.h>
#include <Types.h>
#include <Color.h>

namespace kernel{
  struct GraphicsState{
    GraphicsState(void);
    GraphicsState *clone(void) const;

    pdfout::TransformationMatrix mCTM;        /**< Current transformation matrix */
    float mLineWidth;                         /**< The thickness, in user space units, of paths to be stroked */
    pdfout::LineCap mLineCap;                 /**< A code specifying the shape of the endpoints for any open path that is stroked */
    pdfout::LineJoin mLineJoin;               /**< A code specifying the shape of joints between connected segments of a stroked path */
    float mMiterLimit;                        /**< The maximum length of mitered line joins for stroked paths */
    // mDashPattern;                          /**< A description of the dash pattern to be used when paths are stroked */
    pdfout::RenderingIntent mRenderingIntent; /**< The rendering intent to be used when converting CIE-based colors to device colors */
    float mFlatness;                          /**< The precision with which curves are to be rendered on the output device */


    pdfout::ColorSpace *mStrokingColorSpace;  /**< Current stroking color space */
    pdfout::ColorSpace *mColorSpace;          /**< Current color space */
                                    
    pdfout::Color mStrokingColor;             /**< Current stroking color */
    pdfout::Color mColor;                     /**< Current color */
    pdfout::Color mColorRGB;                  /**< Current RGB color */
    pdfout::Color mColorCMYK;                 /**< Current CMYK color */
    pdfout::Color mColorGray;                 /**< Current Gray color */

    TextState mTextState;                     /**< A set of nine graphics state parameters that pertain only to the painting of text */
    ExtGraphicsState mExtGState;              /**< A set the specified parameters in the graphics state */
  };
}
