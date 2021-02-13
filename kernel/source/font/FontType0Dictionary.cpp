#include <font/FontType0Dictionary.h>

#include <object/ObjectName.h>
#include <object/ObjectArray.h>
#include <object/ObjectStream.h>
#include <object/ObjectInteger.h>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "Font";

  std::string const keySubtype = "Subtype";
  std::string const valueSubtype = "Type0";

  std::string const keyBaseFont = "BaseFont";
  std::string const keyEncoding = "Encoding";
  std::string const keyDescendantFonts = "DescendantFonts";
  std::string const keyToUnicode = "ToUnicode";
}

namespace kernel{ namespace font{
  std::unordered_map<FontType0DictionaryKey, std::pair<std::string, uint32_t>> FontType0Dictionary::mFontType0DictionaryMap{
    {FontType0DictionaryKeyBaseFont,        {keyBaseFont,        ObjectTypeName}},
    {FontType0DictionaryKeyEncoding,        {keyEncoding,        ObjectTypeName | ObjectTypeStream}},
    {FontType0DictionaryKeyDescendantFonts, {keyDescendantFonts, ObjectTypeArray}},
    {FontType0DictionaryKeyToUnicode,       {keyToUnicode,       ObjectTypeStream}}
  };

  FontType0Dictionary::FontType0Dictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){

    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
    ObjectDictionary::insert(keySubtype, std::make_unique<ObjectName>(valueSubtype));
  }

  void FontType0Dictionary::addKey(FontType0DictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mFontType0DictionaryMap, key, std::move(value));
  }
}}
