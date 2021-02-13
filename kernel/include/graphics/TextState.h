#pragma once

#include <string>
#include <cstdint>
#include <Types.h>
#include <FontImpl.h>
#include <TransformationMatrix.h>

namespace kernel{

  /**
   * @brief Graphics state parameters that only affect text.
   */
  struct TextState{
    TextState(void)
      : mCharacterSpacing(0),
      mWordSpacing(0),
      mHorizontalScaling(100),
      mLeading(0),
      mTextFont(""),
      mTextFontSize(0),
      mTextRenderingMode(pdfout::TextRenderingModeFill),
      mTextRise(0),
      mTextKnockout(true),
      mFont(nullptr),
      mCTM(){
    }

    float mCharacterSpacing;       /**< Specified in unscaled text space units. */
    float mWordSpacing;            /**< Works the same way as character spacing but applies only to the space character, code 32. */
    float mHorizontalScaling;      /**< Adjusts the width of glyphs by stretching or compressing them in the horizontal direction. */
    float mLeading;                /**< Specifies the vertical distance between the baselines of adjacent lines of text. */
    std::string mTextFont;         /**< A name of a font resource in the Font subdictionary of the current resource dictionary */
    int32_t mTextFontSize;         /**< Representing a scale factor. */
    pdfout::TextRenderingMode mTextRenderingMode; /**< Determines whether showing text causes glyph outlines to be stroked, filled, used as a clipping boundary,
                                                       or some combination of the three. */
    int32_t mTextRise;             /**< Specifies the distance, in unscaled text space units, to move the baseline up or down from its default location. */
    bool mTextKnockout;            /**< A boolean flag that determines what text elements are considered elementary objects for purposes of color 
                                       compositing in the transparent imaging model. */
    pdfout::impl::FontImpl *mFont;
    pdfout::TransformationMatrix mCTM;     /**< Current Text transformation matrix */
  };
}
