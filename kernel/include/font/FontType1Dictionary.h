#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace font{

  enum FontType1DictionaryKey{
    FontType1DictionaryKeyName,           // Required in PDF 1.0; optional otherwise
    FontType1DictionaryKeyBaseFont,       // Required
    FontType1DictionaryKeyFirstChar,      // Required except for the standard 14 fonts
    FontType1DictionaryKeyLastChar,       // Required except for the standard 14 fonts
    FontType1DictionaryKeyWidths,         // Required except for the standard 14 fonts; indirect reference preferred
    FontType1DictionaryKeyFontDescriptor, // Required except for the standard 14 fonts; must be an indirect reference
    FontType1DictionaryKeyEncoding,       // Optional
    FontType1DictionaryKeyToUnicode       // Optional; PDF 1.2
  };

  class FontType1Dictionary : public ObjectDictionary{
  public:
    FontType1Dictionary(void);
    ~FontType1Dictionary(void) {}

    void addKey(FontType1DictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<FontType1DictionaryKey, std::pair<std::string, uint32_t>> mFontType1Dictionary;

  private:
    FontType1Dictionary(FontType1Dictionary const &) = delete;
    FontType1Dictionary &operator=(FontType1Dictionary const &) = delete;
  };
}}
