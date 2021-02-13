#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace font{

  enum FontDescriptorDictionaryKey{
    FontDescriptorDictionaryKeyFontName,     // Required
    FontDescriptorDictionaryKeyFontFamily,   // Optional; PDF 1.5; strongly recommended for Type 3 fonts in Tagged PDF documents
    FontDescriptorDictionaryKeyFontStretch,  // Optional; PDF 1.5; strongly recommended for Type 3 fonts in Tagged PDF documents
    FontDescriptorDictionaryKeyFontWeight,   // Optional; PDF 1.5; strongly recommended for Type 3 fonts in Tagged PDF documents
    FontDescriptorDictionaryKeyFlags,        // Required
    FontDescriptorDictionaryKeyFontBBox,     // Required, except for Type 3 fonts
    FontDescriptorDictionaryKeyItalicAngle,  // Required
    FontDescriptorDictionaryKeyAscent,       // Required, except for Type 3 fonts
    FontDescriptorDictionaryKeyDescent,      // Required, except for Type 3 fonts
    FontDescriptorDictionaryKeyLeading,      // Optional
    FontDescriptorDictionaryKeyCapHeight,    // Required for fonts that have Latin characters, except for Type 3 fonts
    FontDescriptorDictionaryKeyXHeight,      // Optional
    FontDescriptorDictionaryKeyStemV,        // Required, except for Type 3 fonts
    FontDescriptorDictionaryKeyStemH,        // Optional
    FontDescriptorDictionaryKeyAvgWidth,     // Optional
    FontDescriptorDictionaryKeyMaxWidth,     // Optional
    FontDescriptorDictionaryKeyMissingWidth, // Optional
    FontDescriptorDictionaryKeyFontFile,     // Optional
    FontDescriptorDictionaryKeyFontFile2,    // Optional; PDF 1.1
    FontDescriptorDictionaryKeyFontFile3,    // Optional; PDF 1.2
    FontDescriptorDictionaryKeyCharSet,      // Optional; meaningful only in Type 1 fonts; PDF 1.1
    FontDescriptorDictionaryKeyStyle,        // Optional; CIDFonts only
    FontDescriptorDictionaryKeyLang,         // Optional; CIDFonts only
    FontDescriptorDictionaryKeyFD,           // Optional; CIDFonts only
    FontDescriptorDictionaryKeyCIDSet        // Optional; CIDFonts only
  };

    /**
     * @brief Font Descriptor flags
     */
    enum FontFlags{
      FontFlagsFixedPitch = 1 << 0,  /**< All glyphs have the same width (as opposed to proportional or variable-pitch fonts, which have different widths). */
      FontFlagsSerif = 1 << 1,       /**< Glyphs have serifs, which are short strokes drawn at an angle on the top and bottom of glyph stems. (Sans serif fonts do not have serifs.) */
      FontFlagsSymbolic = 1 << 2,    /**< Font contains glyphs outside the Adobe standard Latin character set. This flag and the Nonsymbolic flag cannot both be set or both be clear (see below). */
      FontFlagsScript = 1 << 3,      /**< Glyphs resemble cursive handwriting. */
      FontFlagsNonsymbolic = 1 << 5, /**< Font uses the Adobe standard Latin character set or a subset of it (see below). */
      FontFlagsItalic = 1 << 6,      /**< Glyphs have dominant vertical strokes that are slanted. */
      FontFlagsAllCap = 1 << 16,     /**< Font contains no lowercase letters; typically used for display purposes, such as for titles or headlines. */
      FontFlagsSmallCap = 1 << 17,   /**< Font contains both uppercase and lowercase letters. The uppercase letters are similar to those in the regular version of the same typeface family.
                                          The glyphs for the lowercase letters have the same shapes as the corresponding uppercase letters, but they are sized and their proportions adjusted
                                          so that they have the same size and stroke weight as lowercase glyphs in the same typeface family. */
      FontFlagsForceBold = 1 << 18   /**< Determines whether bold glyphs are painted with extra pixels even at very small text sizes. */
    };

  class FontDescriptorDictionary : public ObjectDictionary{
  public:
    FontDescriptorDictionary(void);
    ~FontDescriptorDictionary(void) {}

    void addKey(FontDescriptorDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<FontDescriptorDictionaryKey, std::pair<std::string, uint32_t>> mFontDescriptorDictionaryMap;

  private:
    FontDescriptorDictionary(FontDescriptorDictionary const &) = delete;
    FontDescriptorDictionary &operator=(FontDescriptorDictionary const &) = delete;
  };
}}
