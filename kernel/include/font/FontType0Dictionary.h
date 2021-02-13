#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace font{

  enum FontType0DictionaryKey{
    FontType0DictionaryKeyBaseFont,        // Required
    FontType0DictionaryKeyEncoding,        // Required
    FontType0DictionaryKeyDescendantFonts, // Required
    FontType0DictionaryKeyToUnicode        // Optional
  };

  class FontType0Dictionary : public ObjectDictionary{
  public:
    FontType0Dictionary(void);
    ~FontType0Dictionary(void) {}

    void addKey(FontType0DictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<FontType0DictionaryKey, std::pair<std::string, uint32_t>> mFontType0DictionaryMap;

  private:
    FontType0Dictionary(FontType0Dictionary const &) = delete;
    FontType0Dictionary &operator=(FontType0Dictionary const &) = delete;
  };
}}
