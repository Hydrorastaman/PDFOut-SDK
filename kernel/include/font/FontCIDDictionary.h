#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace font{

  enum FontCIDDictionaryKey{
    FontCIDDictionaryKeyBaseFont,       // Required
    FontCIDDictionaryKeyCIDSystemInfo,  // Required
    FontCIDDictionaryKeyFontDescriptor, // Required; must be an indirect reference
    FontCIDDictionaryKeyDW,             // Optional
    FontCIDDictionaryKeyW,              // Optional
    FontCIDDictionaryKeyDW2,            // Optional; applies only to CIDFonts used for vertical writing
    FontCIDDictionaryKeyW2,             // Optional; applies only to CIDFonts used for vertical writing
    FontCIDDictionaryKeyCIDToGIDMap     // Optional; Type 2 CIDFonts only
  };

  enum CIDFontType{
    CIDFontType0,
    CIDFontType2
  };

  class FontCIDDictionary : public ObjectDictionary{
  public:
    FontCIDDictionary(CIDFontType type);
    ~FontCIDDictionary(void) {}

    void addKey(FontCIDDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<FontCIDDictionaryKey, std::pair<std::string, uint32_t>> mFontCIDDictionaryMap;

  private:
    FontCIDDictionary(FontCIDDictionary const &) = delete;
    FontCIDDictionary &operator=(FontCIDDictionary const &) = delete;
  };
}}
