#pragma once

#include <cstdint>
#include <Types.h>

namespace kernel{

  struct ExtGraphicsState{
    ExtGraphicsState(void);

    float mLineWidth;                         /**< The thickness, in user space units, of paths to be stroked */
    pdfout::LineCap mLineCap;                 /**< A code specifying the shape of the endpoints for any open path that is stroked */
    pdfout::LineJoin mLineJoin;               /**< A code specifying the shape of joints between connected segments of a stroked path */
    float mMiterLimit;                        /**< The maximum length of mitered line joins for stroked paths */
    //pdfout::DashPattern mDashPattern;         /**< A description of the dash pattern to be used when paths are stroked */
    pdfout::RenderingIntent mRenderingIntent; /**< The rendering intent to be used when converting CIE-based colors to device colors */
    bool mStrokingOverprint; 
    bool mOverprint;                      /**< (PDF 1.2) A flag specifying (on output devices that support the overprint control feature) whether
                                               painting in one set of colorants should cause the corresponding areas of other colorants to be erased (false)
                                               or left unchanged (true) */
    pdfout::OverprintMode mOverprintMode; /**< (PDF 1.3) A code specifying whether a color component value of 0 in a DeviceCMYK color space should erase
                                               that component (0) or leave it unchanged (1) when overprinting */
    float mFlatness;                      /**< The precision with which curves are to be rendered on the output device. */
    float mSmoothness;                    /**< (PDF 1.3) The precision with which color gradients are to be rendered on the output device */
    bool mStrokeAdjustment;               /**< (PDF 1.2) A flag specifying whether to compensate for possible rasterization effects when stroking
                                               a path with a line width that is small relative to the pixel resolution of the output device */

    pdfout::BlendMode mBlendMode;         /**< (PDF 1.4) The current blend mode to be used in the transparent imaging model */

  //struct{
  //  Font *mFont;
  //  int32_t mFontSize;
  //} mFontInfo; // Font

  // BG
  // BG2
  // UCR
  // UCR2
  // TR
  // TR2
  // HT
  // SMask
  // CA
  // ca
  // AIS
  // TK
  };
}
