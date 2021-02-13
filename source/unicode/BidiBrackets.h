#pragma once

#include <stdint.h>
#include <unordered_map>

namespace pdfout{ namespace unicode{

  enum class ParenthesisDirection{
    Open,
    Close,
    None
  };

  struct ParethesisInfo {
    uint32_t mInPair;
    ParenthesisDirection mDirection;
  };

  inline ParethesisInfo getParethesisInfo(uint32_t symbol) {
    static std::unordered_map<uint32_t, ParethesisInfo> const bidiBrackets = {
      {0x0028, {0x0029, ParenthesisDirection::Open}}, /**< LEFT PARENTHESIS */
      {0x0029, {0x0028, ParenthesisDirection::Close}}, /**< RIGHT PARENTHESIS */
      {0x005B, {0x005D, ParenthesisDirection::Open}}, /**< LEFT SQUARE BRACKET */
      {0x005D, {0x005B, ParenthesisDirection::Close}}, /**< RIGHT SQUARE BRACKET */
      {0x007B, {0x007D, ParenthesisDirection::Open}}, /**< LEFT CURLY BRACKET */
      {0x007D, {0x007B, ParenthesisDirection::Close}}, /**< RIGHT CURLY BRACKET */
      {0x0F3A, {0x0F3B, ParenthesisDirection::Open}}, /**< TIBETAN MARK GUG RTAGS GYON */
      {0x0F3B, {0x0F3A, ParenthesisDirection::Close}}, /**< TIBETAN MARK GUG RTAGS GYAS */
      {0x0F3C, {0x0F3D, ParenthesisDirection::Open}}, /**< TIBETAN MARK ANG KHANG GYON */
      {0x0F3D, {0x0F3C, ParenthesisDirection::Close}}, /**< TIBETAN MARK ANG KHANG GYAS */
      {0x169B, {0x169C, ParenthesisDirection::Open}}, /**< OGHAM FEATHER MARK */
      {0x169C, {0x169B, ParenthesisDirection::Close}}, /**< OGHAM REVERSED FEATHER MARK */
      {0x2045, {0x2046, ParenthesisDirection::Open}}, /**< LEFT SQUARE BRACKET WITH QUILL */
      {0x2046, {0x2045, ParenthesisDirection::Close}}, /**< RIGHT SQUARE BRACKET WITH QUILL */
      {0x207D, {0x207E, ParenthesisDirection::Open}}, /**< SUPERSCRIPT LEFT PARENTHESIS */
      {0x207E, {0x207D, ParenthesisDirection::Close}}, /**< SUPERSCRIPT RIGHT PARENTHESIS */
      {0x208D, {0x208E, ParenthesisDirection::Open}}, /**< SUBSCRIPT LEFT PARENTHESIS */
      {0x208E, {0x208D, ParenthesisDirection::Close}}, /**< SUBSCRIPT RIGHT PARENTHESIS */
      {0x2308, {0x2309, ParenthesisDirection::Open}}, /**< LEFT CEILING */
      {0x2309, {0x2308, ParenthesisDirection::Close}}, /**< RIGHT CEILING */
      {0x230A, {0x230B, ParenthesisDirection::Open}}, /**< LEFT FLOOR */
      {0x230B, {0x230A, ParenthesisDirection::Close}}, /**< RIGHT FLOOR */
      {0x2329, {0x232A, ParenthesisDirection::Open}}, /**< LEFT-POINTING ANGLE BRACKET */
      {0x232A, {0x2329, ParenthesisDirection::Close}}, /**< RIGHT-POINTING ANGLE BRACKET */
      {0x2768, {0x2769, ParenthesisDirection::Open}}, /**< MEDIUM LEFT PARENTHESIS ORNAMENT */
      {0x2769, {0x2768, ParenthesisDirection::Close}}, /**< MEDIUM RIGHT PARENTHESIS ORNAMENT */
      {0x276A, {0x276B, ParenthesisDirection::Open}}, /**< MEDIUM FLATTENED LEFT PARENTHESIS ORNAMENT */
      {0x276B, {0x276A, ParenthesisDirection::Close}}, /**< MEDIUM FLATTENED RIGHT PARENTHESIS ORNAMENT */
      {0x276C, {0x276D, ParenthesisDirection::Open}}, /**< MEDIUM LEFT-POINTING ANGLE BRACKET ORNAMENT */
      {0x276D, {0x276C, ParenthesisDirection::Close}}, /**< MEDIUM RIGHT-POINTING ANGLE BRACKET ORNAMENT */
      {0x276E, {0x276F, ParenthesisDirection::Open}}, /**< HEAVY LEFT-POINTING ANGLE QUOTATION MARK ORNAMEN */
      {0x276F, {0x276E, ParenthesisDirection::Close}}, /**< HEAVY RIGHT-POINTING ANGLE QUOTATION MARK ORNAMENT */
      {0x2770, {0x2771, ParenthesisDirection::Open}}, /**< HEAVY LEFT-POINTING ANGLE BRACKET ORNAMENT */
      {0x2771, {0x2770, ParenthesisDirection::Close}}, /**< HEAVY RIGHT-POINTING ANGLE BRACKET ORNAMENT */
      {0x2772, {0x2773, ParenthesisDirection::Open}}, /**< LIGHT LEFT TORTOISE SHELL BRACKET ORNAMENT */
      {0x2773, {0x2772, ParenthesisDirection::Close}}, /**< LIGHT RIGHT TORTOISE SHELL BRACKET ORNAMENT */
      {0x2774, {0x2775, ParenthesisDirection::Open}}, /**< MEDIUM LEFT CURLY BRACKET ORNAMENT */
      {0x2775, {0x2774, ParenthesisDirection::Close}}, /**< MEDIUM RIGHT CURLY BRACKET ORNAMENT */
      {0x27C5, {0x27C6, ParenthesisDirection::Open}}, /**< LEFT S-SHAPED BAG DELIMITER */
      {0x27C6, {0x27C5, ParenthesisDirection::Close}}, /**< RIGHT S-SHAPED BAG DELIMITER */
      {0x27E6, {0x27E7, ParenthesisDirection::Open}}, /**< MATHEMATICAL LEFT WHITE SQUARE BRACKET */
      {0x27E7, {0x27E6, ParenthesisDirection::Close}}, /**< MATHEMATICAL RIGHT WHITE SQUARE BRACKET */
      {0x27E8, {0x27E9, ParenthesisDirection::Open}}, /**< MATHEMATICAL LEFT ANGLE BRACKET */
      {0x27E9, {0x27E8, ParenthesisDirection::Close}}, /**< MATHEMATICAL RIGHT ANGLE BRACKET */
      {0x27EA, {0x27EB, ParenthesisDirection::Open}}, /**< MATHEMATICAL LEFT DOUBLE ANGLE BRACKET */
      {0x27EB, {0x27EA, ParenthesisDirection::Close}}, /**< MATHEMATICAL RIGHT DOUBLE ANGLE BRACKET */
      {0x27EC, {0x27ED, ParenthesisDirection::Open}}, /**< MATHEMATICAL LEFT WHITE TORTOISE SHELL BRACKET */
      {0x27ED, {0x27EC, ParenthesisDirection::Close}}, /**< MATHEMATICAL RIGHT WHITE TORTOISE SHELL BRACKET */
      {0x27EE, {0x27EF, ParenthesisDirection::Open}}, /**< MATHEMATICAL LEFT FLATTENED PARENTHESIS */
      {0x27EF, {0x27EE, ParenthesisDirection::Close}}, /**< MATHEMATICAL RIGHT FLATTENED PARENTHESIS */
      {0x2983, {0x2984, ParenthesisDirection::Open}}, /**< LEFT WHITE CURLY BRACKET */
      {0x2984, {0x2983, ParenthesisDirection::Close}}, /**< RIGHT WHITE CURLY BRACKET */
      {0x2985, {0x2986, ParenthesisDirection::Open}}, /**< LEFT WHITE PARENTHESIS */
      {0x2986, {0x2985, ParenthesisDirection::Close}}, /**< RIGHT WHITE PARENTHESIS */
      {0x2987, {0x2988, ParenthesisDirection::Open}}, /**< Z NOTATION LEFT IMAGE BRACKET */
      {0x2988, {0x2987, ParenthesisDirection::Close}}, /**< Z NOTATION RIGHT IMAGE BRACKET */
      {0x2989, {0x298A, ParenthesisDirection::Open}}, /**< Z NOTATION LEFT BINDING BRACKET */
      {0x298A, {0x2989, ParenthesisDirection::Close}}, /**< Z NOTATION RIGHT BINDING BRACKET */
      {0x298B, {0x298C, ParenthesisDirection::Open}}, /**< LEFT SQUARE BRACKET WITH UNDERBAR */
      {0x298C, {0x298B, ParenthesisDirection::Close}}, /**< RIGHT SQUARE BRACKET WITH UNDERBAR */
      {0x298D, {0x2990, ParenthesisDirection::Open}}, /**< LEFT SQUARE BRACKET WITH TICK IN TOP CORNER */
      {0x298E, {0x298F, ParenthesisDirection::Close}}, /**< RIGHT SQUARE BRACKET WITH TICK IN BOTTOM CORNER */
      {0x298F, {0x298E, ParenthesisDirection::Open}}, /**< LEFT SQUARE BRACKET WITH TICK IN BOTTOM CORNER */
      {0x2990, {0x298D, ParenthesisDirection::Close}}, /**< RIGHT SQUARE BRACKET WITH TICK IN TOP CORNER */
      {0x2991, {0x2992, ParenthesisDirection::Open}}, /**< LEFT ANGLE BRACKET WITH DOT */
      {0x2992, {0x2991, ParenthesisDirection::Close}}, /**< RIGHT ANGLE BRACKET WITH DOT */
      {0x2993, {0x2994, ParenthesisDirection::Open}}, /**< LEFT ARC LESS-THAN BRACKET */
      {0x2994, {0x2993, ParenthesisDirection::Close}}, /**< RIGHT ARC GREATER-THAN BRACKET */
      {0x2995, {0x2996, ParenthesisDirection::Open}}, /**< DOUBLE LEFT ARC GREATER-THAN BRACKET */
      {0x2996, {0x2995, ParenthesisDirection::Close}}, /**< DOUBLE RIGHT ARC LESS-THAN BRACKET */
      {0x2997, {0x2998, ParenthesisDirection::Open}}, /**< LEFT BLACK TORTOISE SHELL BRACKET */
      {0x2998, {0x2997, ParenthesisDirection::Close}}, /**< RIGHT BLACK TORTOISE SHELL BRACKET */
      {0x29D8, {0x29D9, ParenthesisDirection::Open}}, /**< LEFT WIGGLY FENCE */
      {0x29D9, {0x29D8, ParenthesisDirection::Close}}, /**< RIGHT WIGGLY FENCE */
      {0x29DA, {0x29DB, ParenthesisDirection::Open}}, /**< LEFT DOUBLE WIGGLY FENCE */
      {0x29DB, {0x29DA, ParenthesisDirection::Close}}, /**< RIGHT DOUBLE WIGGLY FENCE */
      {0x29FC, {0x29FD, ParenthesisDirection::Open}}, /**< LEFT-POINTING CURVED ANGLE BRACKET */
      {0x29FD, {0x29FC, ParenthesisDirection::Close}}, /**< RIGHT-POINTING CURVED ANGLE BRACKET */
      {0x2E22, {0x2E23, ParenthesisDirection::Open}}, /**< TOP LEFT HALF BRACKET */
      {0x2E23, {0x2E22, ParenthesisDirection::Close}}, /**< TOP RIGHT HALF BRACKET */
      {0x2E24, {0x2E25, ParenthesisDirection::Open}}, /**< BOTTOM LEFT HALF BRACKET */
      {0x2E25, {0x2E24, ParenthesisDirection::Close}}, /**< BOTTOM RIGHT HALF BRACKET */
      {0x2E26, {0x2E27, ParenthesisDirection::Open}}, /**< LEFT SIDEWAYS U BRACKET */
      {0x2E27, {0x2E26, ParenthesisDirection::Close}}, /**< RIGHT SIDEWAYS U BRACKET */
      {0x2E28, {0x2E29, ParenthesisDirection::Open}}, /**< LEFT DOUBLE PARENTHESIS */
      {0x2E29, {0x2E28, ParenthesisDirection::Close}}, /**< RIGHT DOUBLE PARENTHESIS */
      {0x3008, {0x3009, ParenthesisDirection::Open}}, /**< LEFT ANGLE BRACKET */
      {0x3009, {0x3008, ParenthesisDirection::Close}}, /**< RIGHT ANGLE BRACKET */
      {0x300A, {0x300B, ParenthesisDirection::Open}}, /**< LEFT DOUBLE ANGLE BRACKET */
      {0x300B, {0x300A, ParenthesisDirection::Close}}, /**< RIGHT DOUBLE ANGLE BRACKET */
      {0x300C, {0x300D, ParenthesisDirection::Open}}, /**< LEFT CORNER BRACKET */
      {0x300D, {0x300C, ParenthesisDirection::Close}}, /**< RIGHT CORNER BRACKET */
      {0x300E, {0x300F, ParenthesisDirection::Open}}, /**< LEFT WHITE CORNER BRACKET */
      {0x300F, {0x300E, ParenthesisDirection::Close}}, /**< RIGHT WHITE CORNER BRACKET */
      {0x3010, {0x3011, ParenthesisDirection::Open}}, /**< LEFT BLACK LENTICULAR BRACKET */
      {0x3011, {0x3010, ParenthesisDirection::Close}}, /**< RIGHT BLACK LENTICULAR BRACKET */
      {0x3014, {0x3015, ParenthesisDirection::Open}}, /**< LEFT TORTOISE SHELL BRACKET */
      {0x3015, {0x3014, ParenthesisDirection::Close}}, /**< RIGHT TORTOISE SHELL BRACKET */
      {0x3016, {0x3017, ParenthesisDirection::Open}}, /**< LEFT WHITE LENTICULAR BRACKET */
      {0x3017, {0x3016, ParenthesisDirection::Close}}, /**< RIGHT WHITE LENTICULAR BRACKET */
      {0x3018, {0x3019, ParenthesisDirection::Open}}, /**< LEFT WHITE TORTOISE SHELL BRACKET */
      {0x3019, {0x3018, ParenthesisDirection::Close}}, /**< RIGHT WHITE TORTOISE SHELL BRACKET */
      {0x301A, {0x301B, ParenthesisDirection::Open}}, /**< LEFT WHITE SQUARE BRACKET */
      {0x301B, {0x301A, ParenthesisDirection::Close}}, /**< RIGHT WHITE SQUARE BRACKET */
      {0xFE59, {0xFE5A, ParenthesisDirection::Open}}, /**< SMALL LEFT PARENTHESIS */
      {0xFE5A, {0xFE59, ParenthesisDirection::Close}}, /**< SMALL RIGHT PARENTHESIS */
      {0xFE5B, {0xFE5C, ParenthesisDirection::Open}}, /**< SMALL LEFT CURLY BRACKET */
      {0xFE5C, {0xFE5B, ParenthesisDirection::Close}}, /**< SMALL RIGHT CURLY BRACKET */
      {0xFE5D, {0xFE5E, ParenthesisDirection::Open}}, /**< SMALL LEFT TORTOISE SHELL BRACKET */
      {0xFE5E, {0xFE5D, ParenthesisDirection::Close}}, /**< SMALL RIGHT TORTOISE SHELL BRACKET */
      {0xFF08, {0xFF09, ParenthesisDirection::Open}}, /**< FULLWIDTH LEFT PARENTHESIS */
      {0xFF09, {0xFF08, ParenthesisDirection::Close}}, /**< FULLWIDTH RIGHT PARENTHESIS */
      {0xFF3B, {0xFF3D, ParenthesisDirection::Open}}, /**< FULLWIDTH LEFT SQUARE BRACKET */
      {0xFF3D, {0xFF3B, ParenthesisDirection::Close}}, /**< FULLWIDTH RIGHT SQUARE BRACKET */
      {0xFF5B, {0xFF5D, ParenthesisDirection::Open}}, /**< FULLWIDTH LEFT CURLY BRACKET */
      {0xFF5D, {0xFF5B, ParenthesisDirection::Close}}, /**< FULLWIDTH RIGHT CURLY BRACKET */
      {0xFF5F, {0xFF60, ParenthesisDirection::Open}}, /**< FULLWIDTH LEFT WHITE PARENTHESIS */
      {0xFF60, {0xFF5F, ParenthesisDirection::Close}}, /**< FULLWIDTH RIGHT WHITE PARENTHESIS */
      {0xFF62, {0xFF63, ParenthesisDirection::Open}}, /**< HALFWIDTH LEFT CORNER BRACKET */
      {0xFF63, {0xFF62, ParenthesisDirection::Close}}, /**< HALFWIDTH RIGHT CORNER BRACKET */
    };

    auto iter = bidiBrackets.find(symbol);
    if (iter != std::end(bidiBrackets))
      return iter->second;

    return {0, ParenthesisDirection::None};
  }
}}
