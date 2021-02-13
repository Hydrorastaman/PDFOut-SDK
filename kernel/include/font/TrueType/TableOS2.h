#pragma once

#include <memory>
#include <cstdint>

#include <InternalTools.h>
#include <font/TrueType/Table.h>

namespace pdfout{

  class InputStream;
  class OutputStream;

}

namespace kernel{ namespace font{ namespace truetype{

  /**
   * @brief Indicates the visual weight (degree of blackness or thickness of strokes) of the characters in the font.
   */
  enum WeightClass{
    WeightClassThin = 100,
    WeightClassExtraLight = 200, /**< Ultra Light */
    WeightClassLight = 300,
    WeightClassNormal = 400,     /**< Regular */
    WeightClassMedium = 500,
    WeightClassSemiBold = 600,   /**< Demi Bold */
    WeightClassBold = 700,
    WeightClassExtraBold = 800,  /**< Ultra Bold */
    WeightClassBlack = 900,      /**< Heavy */
  };

  /**
   * @brief Indicates a relative change from the normal aspect ratio (width to height ratio) as specified by a font designer for the glyphs in a font.
   */
  enum WidthClass{
    WidthClassUltraCondensed = 1, /**< 50% of normal */
    WidthClassExtraCondensed = 2, /**< 62.5% of normal */
    WidthClassCondensed = 3,      /**< 75% of normal */
    WidthClassSemiCondensed = 4,  /**< 87.5% of normal */
    WidthClassMedium = 5,         /**< Normal, 100% of normal */
    WidthClassSemiExpanded = 6,   /**< 112.5% of normal */
    WidthClassExpanded = 7,       /**< 125% of normal */
    WidthClassExtraExpanded = 8,  /**< 150% of normal */
    WidthClassUltraExpanded = 9   /**< 200% of normal */
  };

  /**
   * @brief Indicates font embedding licensing rights for the font. Embeddable fonts may be stored in a document.
            When a document with embedded fonts is opened on a system that does not have the font installed (the remote system),
            the embedded font may be loaded for temporary (and in some cases, permanent) use on that system by an embedding-aware application.
            Embedding licensing rights are granted by the vendor of the font. 
   */
  enum FsType{

    FsTypeInstallableEmbedding = 0,    /**< No fsType bit is set. Thus fsType is zero. Fonts with this setting indicate that they may be embedded and 
                                            permanently installed on the remote system by an application. The user of the remote system acquires the identical rights,
                                            obligations and licenses for that font as the original purchaser of the font, and is subject to the same end-user license agreement,
                                            copyright, design patent, and/or trademark as was the original purchaser. */

    FsTypeRestrictedLicense = 0x0002,  /**< Fonts that have only this bit set must not be modified, embedded or exchanged in any manner without first obtaining permission of the legal owner.
                                            Caution: For Restricted License embedding to take effect, it must be the only level of embedding selected. */

    FsTypePreviewAndPrint = 0x0004,    /**< The font may be embedded, and temporarily loaded on the remote system. Documents containing Preview & Print fonts must be opened “read-only;”
                                            no edits can be applied to the document. */

    FsTypeEditableEmbedding = 0x0008,  /**< The font may be embedded but must only be installed temporarily on other systems. In contrast to Preview & Print fonts, documents containing
                                            Editable fonts may be opened for reading, editing is permitted, and changes may be saved. */

    FsTypeNoSubsetting = 0x0100,       /**< The font may not be subsetted prior to embedding. Other embedding restrictions specified in bits 0-3 and 9 also apply. */

    FsTypeBitmapEmbeddingOnly = 0x0200 /**< Only bitmaps contained in the font may be embedded. No outline data may be embedded. If there are no bitmaps available in the font,
                                            then the font is considered unembeddable and the embedding services will fail. Other embedding restrictions specified in bits 0-3 and 8 also apply. */
  };

  /**
   * @brief PANOSE classification number
   */
  PACK(struct Panose{
    int8_t bFamilyType;      /**<  */
    int8_t bSerifStyle;      /**<  */
    int8_t bWeight;          /**<  */
    int8_t bProportion;      /**<  */
    int8_t bContrast;        /**<  */
    int8_t bStrokeVariation; /**<  */
    int8_t bArmStyle;        /**<  */
    int8_t bLetterform;      /**<  */
    int8_t bMidline;         /**<  */
    int8_t bXHeight;         /**<  */
  });

  /**
   * @brief Font selection flags
   */
  enum FsSelection{
    FsSelectionItalic = 0x1,            /**< Font contains italic or oblique characters, otherwise they are upright. */
    FsSelectionUnderscore = 0x2,        /**< Characters are underscored. */
    FsSelectionNegative = 0x4,          /**< Characters have their foreground and background reversed. */
    FsSelectionOutlined = 0x8,          /**< Outline (hollow) characters, otherwise they are solid. */
    FsSelectionStrikeout = 0x10,        /**< Characters are overstruck. */
    FsSelectionBold = 0x20,             /**< Characters are emboldened. */
    FsSelectionRegular = 0x40,          /**< Characters are in the standard weight/style for the font. */
    FsSelectionUse_typo_metrics = 0x80, /**< If set, it is strongly recommended to use OS/2.sTypoAscender - OS/2.sTypoDescender+ OS/2.sTypoLineGap as a value for default line spacing for this font. */
    FsSelectionWws = 0x100,             /**< The font has ‘name' table strings consistent with a weight/width/slope family without requiring use of ‘name' IDs 21 and 22. */
    FsSelectionOblique = 0x200          /**< Font contains oblique characters. */
  };

  /**
   * @brief TrueType font OS/2 and Windows metrics, "OS/2"
   */
  PACK(struct TableOS2Data{
    /** version 0 */
    uint16_t mVersion;                 /**< The version number for this OS/2 table, 0x0005 */
    int16_t  mXAvgCharWidth;           /**< The Average Character Width parameter specifies the arithmetic average of the escapement (width) of all non-zero width glyphs in the font */
    uint16_t mUsWeightClass;           /**< Indicates the visual weight (degree of blackness or thickness of strokes) of the characters in the font, see enum WeightClass */
    uint16_t mUsWidthClass;            /**< Indicates a relative change from the normal aspect ratio (width to height ratio) as specified by a font designer for the glyphs in a font, see enum WidthClass*/
    uint16_t mFsType;                  /**< Indicates font embedding licensing rights for the font, see enum FsType */
    int16_t  mYSubscriptXSize;         /**< The recommended horizontal size in font design units for subscripts for this font */
    int16_t  mYSubscriptYSize;         /**< The recommended vertical size in font design units for subscripts for this font */
    int16_t  mYSubscriptXOffset;       /**< The recommended horizontal offset in font design untis for subscripts for this font */
    int16_t  mYSubscriptYOffset;       /**< The recommended vertical offset in font design units from the baseline for subscripts for this font */
    int16_t  mYSuperscriptXSize;       /**< The recommended horizontal size in font design units for superscripts for this font */
    int16_t  mYSuperscriptYSize;       /**< The recommended vertical size in font design units for superscripts for this font */
    int16_t  mYSuperscriptXOffset;     /**< The recommended horizontal offset in font design units for superscripts for this font */
    int16_t  mYSuperscriptYOffset;     /**< The recommended vertical offset in font design units from the baseline for superscripts for this font */
    int16_t  mYStrikeoutSize;          /**< Width of the strikeout stroke in font design units */
    int16_t  mYStrikeoutPosition;      /**< The position of the top of the strikeout stroke relative to the baseline in font design units */
    int16_t  mSFamilyClass;            /**< This parameter is a classification of font-family design */
    Panose   mPanose;                  /**< PANOSE classification number, see struct Panose */
    uint32_t mUlUnicodeRange1;         /**< Unicode Character Range, Bits 0-31 */
    uint32_t mUlUnicodeRange2;         /**< Unicode Character Range, Bits 32-63 */
    uint32_t mUlUnicodeRange3;         /**< Unicode Character Range, Bits 64-95 */
    uint32_t mUlUnicodeRange4;         /**< Unicode Character Range, Bits 96-127 */
    uint8_t  mAchVendID[4];            /**< The four character identifier for the vendor of the given type face */
    uint16_t mFsSelection;             /**< Contains information concerning the nature of the font patterns, see FsSelection enum */
    uint16_t mUsFirstCharIndex;        /**< The minimum Unicode index (character code) in this font, according to the cmap subtable for platform ID 3 and platform- specific encoding ID 0 or 1 */
    uint16_t mUsLastCharIndex;         /**< The maximum Unicode index (character code) in this font, according to the cmap subtable for platform ID 3 and encoding ID 0 or 1 */
    int16_t  mTypoAscender;            /**< The typographic ascender for this font. Remember that this is not the same as the Ascender value in the 'hhea' table, which Apple defines in a far different manner */
    int16_t  mTypoDescender;           /**< The typographic descender for this font. Remember that this is not the same as the Descender value in the 'hhea' table, which Apple defines in a far different manner */
    int16_t  mTypoLineGap;             /**< The typographic line gap for this font. Remember that this is not the same as the LineGap value in the 'hhea' table, which Apple defines in a far different manner */
    uint16_t mUsWinAscent;             /**< The ascender metric for Windows. This, too, is distinct from Apple's Ascender value and from the usTypoAscender values */
    uint16_t mUsWinDescent;            /**< The descender metric for Windows. This, too, is distinct from Apple's Descender value and from the usTypoDescender values */

    /** version 1 */
    uint32_t mUlCodePageRange1;        /**< Code Page Character Range, Bits 0-31 */
    uint32_t mUlCodePageRange2;        /**< Code Page Character Range, Bits 32-63 */

    /** version 2, 3, 4 */
    int16_t  mXHeight;                 /**< This metric specifies the distance between the baseline and the approximate height of non-ascending lowercase letters measured in FUnits */
    int16_t  mCapHeight;               /**< This metric specifies the distance between the baseline and the approximate height of uppercase letters measured in FUnits */
    uint16_t mUsDefaultChar;           /**< Whenever a request is made for a character that is not in the font, Windows provides this default character */
    uint16_t mUsBreakChar;             /**< This is the Unicode encoding of the glyph that Windows uses as the break character */
    uint16_t mUsMaxContext;            /**< The maximum length of a target glyph context for any feature in this font */

    /** version 5 */
    uint16_t mUsLowerOpticalPointSize; /**< This field is used for fonts with multiple optical styles */
    uint16_t mUsUpperOpticalPointSize; /**< This field is used for fonts with multiple optical styles */
  });

  enum TableOS2Version{
    TableOS2Version0 = 0,
    TableOS2Version1 = 1,
    TableOS2Version2 = 2,
    TableOS2Version3 = 3,
    TableOS2Version4 = 4,
    TableOS2Version5 = 5
  };

  enum TableOS2VerSize{
    TableOS2VerSize0 = 78,
    TableOS2VerSize1 = 86,
    TableOS2VerSize2 = 96,
    TableOS2VerSize3 = 96,
    TableOS2VerSize4 = 96,
    TableOS2VerSize5 = 100
  };

  class TableOS2 : public Table{
  public:
    TableOS2(void);
    ~TableOS2(void);

    void load(pdfout::InputStream *stream);
    void save(pdfout::OutputStream *stream) const;

    std::string getWidthClass(void) const;
    uint16_t getWeightClass(void) const;
    int16_t getAscent(void) const;
    int16_t getWinAscent(void) const;
    int16_t getDescent(void) const;
    int16_t getWinDescent(void) const;
    int16_t getCapHeight(void) const;
    int16_t getLineGap(void) const;
    uint8_t getFontClassId(void) const;
    uint8_t getFontSubclassId(void) const;
    uint16_t getFirstCharIndex(void) const;
    uint16_t getLastCharIndex(void) const;
    uint16_t getBreakChar(void) const;
    uint16_t getDefaultChar(void) const;
    uint16_t getUnderlineYSize(void) const;
    bool isSerif(void) const;
    bool isScript(void) const;
    bool isSymbolic(void) const;

    std::unique_ptr<TableOS2> createSubset(FontInfo const &fontInfo, uint16_t defaultChar, uint16_t breakChar) const;

  private:
    //TableOS2(TableOS2 const &) = delete;
    TableOS2 &operator=(TableOS2 const &) = delete;

  public:
    TableOS2Data mData;
  };

}}}
