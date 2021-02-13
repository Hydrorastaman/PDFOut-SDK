#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace font{

  enum FontTrueTypeDictionaryKey{
    FontTrueTypeDictionaryKeyName,           // Required in PDF 1.0; optional otherwise
    FontTrueTypeDictionaryKeyBaseFont,       // Required
    FontTrueTypeDictionaryKeyFirstChar,      // Required except for the standard 14 fonts
    FontTrueTypeDictionaryKeyLastChar,       // Required except for the standard 14 fonts
    FontTrueTypeDictionaryKeyWidths,         // Required except for the standard 14 fonts; indirect reference preferred
    FontTrueTypeDictionaryKeyFontDescriptor, // Required except for the standard 14 fonts; must be an indirect reference
    FontTrueTypeDictionaryKeyEncoding,       // Optional
    FontTrueTypeDictionaryKeyToUnicode       // Optional; PDF 1.2
  };

  class FontTrueTypeDictionary : public ObjectDictionary{
  public:
    FontTrueTypeDictionary(void);
    ~FontTrueTypeDictionary(void) {}

    void addKey(FontTrueTypeDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<FontTrueTypeDictionaryKey, std::pair<std::string, uint32_t>> mFontTrueTypeDictonary;

  private:
    FontTrueTypeDictionary(FontTrueTypeDictionary const &) = delete;
    FontTrueTypeDictionary &operator=(FontTrueTypeDictionary const &) = delete;
  };
}}
