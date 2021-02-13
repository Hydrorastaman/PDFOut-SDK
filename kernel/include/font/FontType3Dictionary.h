#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace font{

  enum FontType3DictionaryKey{
    FontType3DictionaryKeyName,           // Required in PDF 1.0; optional otherwise
    FontType3DictionaryKeyFontBBox,       // Required
    FontType3DictionaryKeyFontMatrix,     // Required
    FontType3DictionaryKeyCharProcs,      // Required
    FontType3DictionaryKeyEncoding,       // Required
    FontType3DictionaryKeyFirstChar,      // Required
    FontType3DictionaryKeyLastChar,       // Required
    FontType3DictionaryKeyWidths,         // Required
    FontType3DictionaryKeyFontDescriptor, // Required in Tagged PDF documents; must be an indirect reference
    FontType3DictionaryKeyResources,      // Optional but strongly recommended; PDF 1.2
    FontType3DictionaryKeyToUnicode       // Optional; PDF 1.2
  };

  class FontType3Dictionary : public ObjectDictionary{
  public:
    FontType3Dictionary(void);
    ~FontType3Dictionary(void) {}

    void addKey(FontType3DictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<FontType3DictionaryKey, std::pair<std::string, uint32_t>> mFontType3DictionaryMap;

  private:
    FontType3Dictionary(FontType3Dictionary const &) = delete;
    FontType3Dictionary &operator=(FontType3Dictionary const &) = delete;
  };
}}
