#pragma once

#include "PdfOutConfig.h"

namespace pdfout{

  /**
   RGB color model
   */
  struct ColorRGB{
    float mRed;   /**< Red component */
    float mGreen; /**< Green component */
    float mBlue;  /**< Blue component */
  };

  /**
   CMYK color model
   */
  struct ColorCMYK{
    float mCyan;    /**< Cyan component */
    float mMagenta; /**< Magenta component */
    float mYellow;  /**< Yellow component */
    float mBlack;   /**< Black component */
  };

  /**
   Gray color model
   */
  struct ColorGray{
    float mGray; /**< Gray component */
  };

  /**
   Index color model
   */
  struct ColorIndex{
    uint32_t mIndex; /**< Index */
  };

  /**
   Color types
   */
  enum ColorType{
    ColorTypeRGB,  /**< RGB color model */
    ColorTypeCMYK, /**< CMYK color model */
    ColorTypeGray, /**< Gray color model */
    ColorTypeIndex /**< Index color model */
  };

  enum ColorName;

  struct PDFOUT_EXPORTS Color{
    ColorType mType;
    union{
      ColorGray mGray;
      ColorRGB mRGB;
      ColorCMYK mCMYK;
      ColorIndex mIndex;
    } mValue;

    /**
     Creates RGB color

     @param red Red component;
     @param green Green component;
     @param blue Blue component;
     @return Color object.

     @code {.cpp}
       Color const woodColor = Color::createRGB(0xDE, 0xB8, 0x87);
     @endcode
     */
    static Color createRGB(float red, float green, float blue);

    /**
     Creates RGB color from pre-defined color name

     @param colorName Pre-defined color name
     @return Color object.

     @code {.cpp}
       Color const woodColor = Color::createRGB(ColorNameBurlyWood);
     @endcode
     */
    static Color createRGB(ColorName colorName);

    /**
     Creates CMYK color

     @param cyan Cyan component;
     @param magenta Magenta component;
     @param yellow Yellow component;
     @param black Black component;
     @return Color object.

     @code {.cpp}
       Color const woodColor = Color::createCMYK(0, 17, 39, 13);
     @endcode
     */
    static Color createCMYK(float cyan, float magenta, float yellow, float black);

    /**
     Creates CMYK color from pre-defined color name

     @param colorName Pre-defined color name
     @return Color object.

     @code {.cpp}
       Color const woodColor = Color::createCMYK(ColorNameBurlyWood);
     @endcode
     */
    static Color createCMYK(ColorName colorName);

    /**
     Creates Gray color

     @param gray Gray component;
     @return Color object.

     @code {.cpp}
       Color const woodColor = Color::createGray(188.55f);
     @endcode
     */
    static Color createGray(float gray);

    /**
     Creates Gray color from pre-defined color name with luminosity conversion algorithm

     @param colorName Pre-defined color name
     @return Color object.

     @code {.cpp}
       Color const woodColor = Color::createGray(ColorNameBurlyWood);
     @endcode
     */
    static Color createGray(ColorName colorName); // The luminosity algorithm, gray = 0.21f * R + 0.72f * G + 0.07f * B;

    /**
     Creates Index color

     @param i Index;
     @return Color object.

     @code {.cpp}
       Color const index = Color::createIndex(17);
     @endcode
     */
    static Color createIndex(uint32_t i);
  };

  /**
   Pre-defined HTML color names
   */
  enum ColorName{
    ColorNameAliceBlue = 0,         /**< {0xF0, 0xF8, 0xFF} */
    ColorNameAntiqueWhite,          /**< {0xFA, 0xEB, 0xD7} */
    ColorNameAqua,                  /**< {0x00, 0xFF, 0xFF} */
    ColorNameAquamarine,            /**< {0x7F, 0xFF, 0xD4} */
    ColorNameAzure,                 /**< {0xF0, 0xFF, 0xFF} */
    ColorNameBeige,                 /**< {0xF5, 0xF5, 0xDC} */
    ColorNameBisque,                /**< {0xFF, 0xE4, 0xC4} */
    ColorNameBlack,                 /**< {0x00, 0x00, 0x00} */
    ColorNameBlanchedAlmond,        /**< {0xFF, 0xEB, 0xCD} */
    ColorNameBlue,                  /**< {0x00, 0x00, 0xFF} */
    ColorNameBlueViolet,            /**< {0x8A, 0x2B, 0xE2} */
    ColorNameBrown,                 /**< {0xA5, 0x2A, 0x2A} */
    ColorNameBurlyWood,             /**< {0xDE, 0xB8, 0x87} */
    ColorNameCadetBlue,             /**< {0x5F, 0x9E, 0xA0} */
    ColorNameChartreuse,            /**< {0x7F, 0xFF, 0x00} */
    ColorNameChocolate,             /**< {0xD2, 0x69, 0x1E} */
    ColorNameCoral,                 /**< {0xFF, 0x7F, 0x50} */
    ColorNameCornflowerBlue,        /**< {0x64, 0x95, 0xED} */
    ColorNameCornsilk,              /**< {0xFF, 0xF8, 0xDC} */
    ColorNameCrimson,               /**< {0xDC, 0x14, 0x3C} */
    ColorNameCyan,                  /**< {0x00, 0xFF, 0xFF} */
    ColorNameDarkBlue,              /**< {0x00, 0x00, 0x8B} */
    ColorNameDarkCyan,              /**< {0x00, 0x8B, 0x8B} */
    ColorNameDarkGoldenRod,         /**< {0xB8, 0x86, 0x0B} */
    ColorNameDarkGray,              /**< {0xA9, 0xA9, 0xA9} */
    ColorNameDarkGrey,              /**< {0xA9, 0xA9, 0xA9} */
    ColorNameDarkGreen,             /**< {0x00, 0x64, 0x00} */
    ColorNameDarkKhaki,             /**< {0xBD, 0xB7, 0x6B} */
    ColorNameDarkMagenta,           /**< {0x8B, 0x00, 0x8B} */
    ColorNameDarkOliveGreen,        /**< {0x55, 0x6B, 0x2F} */
    ColorNameDarkOrange,            /**< {0xFF, 0x8C, 0x00} */
    ColorNameDarkOrchid,            /**< {0x99, 0x32, 0xCC} */
    ColorNameDarkRed,               /**< {0x8B, 0x00, 0x00} */
    ColorNameDarkSalmon,            /**< {0xE9, 0x96, 0x7A} */
    ColorNameDarkSeaGreen,          /**< {0x8F, 0xBC, 0x8F} */
    ColorNameDarkSlateBlue,         /**< {0x48, 0x3D, 0x8B} */
    ColorNameDarkSlateGray,         /**< {0x2F, 0x4F, 0x4F} */
    ColorNameDarkSlateGrey,         /**< {0x2F, 0x4F, 0x4F} */
    ColorNameDarkTurquoise,         /**< {0x00, 0xCE, 0xD1} */
    ColorNameDarkViolet,            /**< {0x94, 0x00, 0xD3} */
    ColorNameDeepPink,              /**< {0xFF, 0x14, 0x93} */
    ColorNameDeepSkyBlue,           /**< {0x00, 0xBF, 0xFF} */
    ColorNameDimGray,               /**< {0x69, 0x69, 0x69} */
    ColorNameDimGrey,               /**< {0x69, 0x69, 0x69} */
    ColorNameDodgerBlue,            /**< {0x1E, 0x90, 0xFF} */
    ColorNameFireBrick,             /**< {0xB2, 0x22, 0x22} */
    ColorNameFloralWhite,           /**< {0xFF, 0xFA, 0xF0} */
    ColorNameForestGreen,           /**< {0x22, 0x8B, 0x22} */
    ColorNameFuchsia,               /**< {0xFF, 0x00, 0xFF} */
    ColorNameGainsboro,             /**< {0xDC, 0xDC, 0xDC} */
    ColorNameGhostWhite,            /**< {0xF8, 0xF8, 0xFF} */
    ColorNameGold,                  /**< {0xFF, 0xD7, 0x00} */
    ColorNameGoldenRod,             /**< {0xDA, 0xA5, 0x20} */
    ColorNameGray,                  /**< {0x80, 0x80, 0x80} */
    ColorNameGrey,                  /**< {0x80, 0x80, 0x80} */
    ColorNameGreen,                 /**< {0x00, 0x80, 0x00} */
    ColorNameGreenYellow,           /**< {0xAD, 0xFF, 0x2F} */
    ColorNameHoneyDew,              /**< {0xF0, 0xFF, 0xF0} */
    ColorNameHotPink,               /**< {0xFF, 0x69, 0xB4} */
    ColorNameIndianRed,             /**< {0xCD, 0x5C, 0x5C} */
    ColorNameIndigo,                /**< {0x4B, 0x00, 0x82} */
    ColorNameIvory,                 /**< {0xFF, 0xFF, 0xF0} */
    ColorNameKhaki,                 /**< {0xF0, 0xE6, 0x8C} */
    ColorNameLavender,              /**< {0xE6, 0xE6, 0xFA} */
    ColorNameLavenderBlush,         /**< {0xFF, 0xF0, 0xF5} */
    ColorNameLawnGreen,             /**< {0x7C, 0xFC, 0x00} */
    ColorNameLemonChiffon,          /**< {0xFF, 0xFA, 0xCD} */
    ColorNameLightBlue,             /**< {0xAD, 0xD8, 0xE6} */
    ColorNameLightCoral,            /**< {0xF0, 0x80, 0x80} */
    ColorNameLightCyan,             /**< {0xE0, 0xFF, 0xFF} */
    ColorNameLightGoldenRodYellow,  /**< {0xFA, 0xFA, 0xD2} */
    ColorNameLightGray,             /**< {0xD3, 0xD3, 0xD3} */
    ColorNameLightGrey,             /**< {0xD3, 0xD3, 0xD3} */
    ColorNameLightGreen,            /**< {0x90, 0xEE, 0x90} */
    ColorNameLightPink,             /**< {0xFF, 0xB6, 0xC1} */
    ColorNameLightSalmon,           /**< {0xFF, 0xA0, 0x7A} */
    ColorNameLightSeaGreen,         /**< {0x20, 0xB2, 0xAA} */
    ColorNameLightSkyBlue,          /**< {0x87, 0xCE, 0xFA} */
    ColorNameLightSlateGray,        /**< {0x77, 0x88, 0x99} */
    ColorNameLightSlateGrey,        /**< {0x77, 0x88, 0x99} */
    ColorNameLightSteelBlue,        /**< {0xB0, 0xC4, 0xDE} */
    ColorNameLightYellow,           /**< {0xFF, 0xFF, 0xE0} */
    ColorNameLime,                  /**< {0x00, 0xFF, 0x00} */
    ColorNameLimeGreen,             /**< {0x32, 0xCD, 0x32} */
    ColorNameLinen,                 /**< {0xFA, 0xF0, 0xE6} */
    ColorNameMagenta,               /**< {0xFF, 0x00, 0xFF} */
    ColorNameMaroon,                /**< {0x80, 0x00, 0x00} */
    ColorNameMediumAquaMarine,      /**< {0x66, 0xCD, 0xAA} */
    ColorNameMediumBlue,            /**< {0x00, 0x00, 0xCD} */
    ColorNameMediumOrchid,          /**< {0xBA, 0x55, 0xD3} */
    ColorNameMediumPurple,          /**< {0x93, 0x70, 0xDB} */
    ColorNameMediumSeaGreen,        /**< {0x3C, 0xB3, 0x71} */
    ColorNameMediumSlateBlue,       /**< {0x7B, 0x68, 0xEE} */
    ColorNameMediumSpringGreen,     /**< {0x00, 0xFA, 0x9A} */
    ColorNameMediumTurquoise,       /**< {0x48, 0xD1, 0xCC} */
    ColorNameMediumVioletRed,       /**< {0xC7, 0x15, 0x85} */
    ColorNameMidnightBlue,          /**< {0x19, 0x19, 0x70} */
    ColorNameMintCream,             /**< {0xF5, 0xFF, 0xFA} */
    ColorNameMistyRose,             /**< {0xFF, 0xE4, 0xE1} */
    ColorNameMoccasin,              /**< {0xFF, 0xE4, 0xB5} */
    ColorNameNavajoWhite,           /**< {0xFF, 0xDE, 0xAD} */
    ColorNameNavy,                  /**< {0x00, 0x00, 0x80} */
    ColorNameOldLace,               /**< {0xFD, 0xF5, 0xE6} */
    ColorNameOlive,                 /**< {0x80, 0x80, 0x00} */
    ColorNameOliveDrab,             /**< {0x6B, 0x8E, 0x23} */
    ColorNameOrange,                /**< {0xFF, 0xA5, 0x00} */
    ColorNameOrangeRed,             /**< {0xFF, 0x45, 0x00} */
    ColorNameOrchid,                /**< {0xDA, 0x70, 0xD6} */
    ColorNamePaleGoldenRod,         /**< {0xEE, 0xE8, 0xAA} */
    ColorNamePaleGreen,             /**< {0x98, 0xFB, 0x98} */
    ColorNamePaleTurquoise,         /**< {0xAF, 0xEE, 0xEE} */
    ColorNamePaleVioletRed,         /**< {0xDB, 0x70, 0x93} */
    ColorNamePapayaWhip,            /**< {0xFF, 0xEF, 0xD5} */
    ColorNamePeachPuff,             /**< {0xFF, 0xDA, 0xB9} */
    ColorNamePeru,                  /**< {0xCD, 0x85, 0x3F} */
    ColorNamePink,                  /**< {0xFF, 0xC0, 0xCB} */
    ColorNamePlum,                  /**< {0xDD, 0xA0, 0xDD} */
    ColorNamePowderBlue,            /**< {0xB0, 0xE0, 0xE6} */
    ColorNamePurple,                /**< {0x80, 0x00, 0x80} */
    ColorNameRebeccaPurple,         /**< {0x66, 0x33, 0x99} */
    ColorNameRed,                   /**< {0xFF, 0x00, 0x00} */
    ColorNameRosyBrown,             /**< {0xBC, 0x8F, 0x8F} */
    ColorNameRoyalBlue,             /**< {0x41, 0x69, 0xE1} */
    ColorNameSaddleBrown,           /**< {0x8B, 0x45, 0x13} */
    ColorNameSalmon,                /**< {0xFA, 0x80, 0x72} */
    ColorNameSandyBrown,            /**< {0xF4, 0xA4, 0x60} */
    ColorNameSeaGreen,              /**< {0x2E, 0x8B, 0x57} */
    ColorNameSeaShell,              /**< {0xFF, 0xF5, 0xEE} */
    ColorNameSienna,                /**< {0xA0, 0x52, 0x2D} */
    ColorNameSilver,                /**< {0xC0, 0xC0, 0xC0} */
    ColorNameSkyBlue,               /**< {0x87, 0xCE, 0xEB} */
    ColorNameSlateBlue,             /**< {0x6A, 0x5A, 0xCD} */
    ColorNameSlateGray,             /**< {0x70, 0x80, 0x90} */
    ColorNameSlateGrey,             /**< {0x70, 0x80, 0x90} */
    ColorNameSnow,                  /**< {0xFF, 0xFA, 0xFA} */
    ColorNameSpringGreen,           /**< {0x00, 0xFF, 0x7F} */
    ColorNameSteelBlue,             /**< {0x46, 0x82, 0xB4} */
    ColorNameTan,                   /**< {0xD2, 0xB4, 0x8C} */
    ColorNameTeal,                  /**< {0x00, 0x80, 0x80} */
    ColorNameThistle,               /**< {0xD8, 0xBF, 0xD8} */
    ColorNameTomato,                /**< {0xFF, 0x63, 0x47} */
    ColorNameTurquoise,             /**< {0x40, 0xE0, 0xD0} */
    ColorNameViolet,                /**< {0xEE, 0x82, 0xEE} */
    ColorNameWheat,                 /**< {0xF5, 0xDE, 0xB3} */
    ColorNameWhite,                 /**< {0xFF, 0xFF, 0xFF} */
    ColorNameWhiteSmoke,            /**< {0xF5, 0xF5, 0xF5} */
    ColorNameYellow,                /**< {0xFF, 0xFF, 0x00} */
    ColorNameYellowGreen            /**< {0x9A, 0xCD, 0x32} */
  };
}
