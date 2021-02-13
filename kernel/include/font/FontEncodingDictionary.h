#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace font{

  enum FontEncodingDictionaryKey{
    FontEncodingDictionaryKeyBaseEncoding, // Optional
    FontEncodingDictionaryKeyDifferences   // Optional; not recommended with TrueType fonts
  };

  class FontEncodingDictionary : public ObjectDictionary{
  public:
    FontEncodingDictionary(void);
    ~FontEncodingDictionary(void) {}

    void addKey(FontEncodingDictionaryKey key, std::unique_ptr<Object> value);

    FontEncodingDictionary *clone(void) const {return nullptr;}

  private:
    static std::unordered_map<FontEncodingDictionaryKey, std::pair<std::string,  uint32_t>> mFontEncodingDictionaryMap;

  private:
    FontEncodingDictionary(FontEncodingDictionary const &) = delete;
    FontEncodingDictionary &operator=(FontEncodingDictionary const &) = delete;
  };
}}
