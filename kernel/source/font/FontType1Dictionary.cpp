#include <font/FontType1Dictionary.h>

#include <object/ObjectName.h>
#include <object/ObjectArray.h>
#include <object/ObjectStream.h>
#include <object/ObjectInteger.h>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "Font";

  std::string const keySubtype = "Subtype";
  std::string const valueSubtype = "Type1";

  std::string const keyName = "Name";
  std::string const keyBaseFont = "BaseFont";
  std::string const keyFirstChar = "FirstChar";
  std::string const keyLastChar = "LastChar";
  std::string const keyWidths = "Widths";
  std::string const keyFontDescriptor = "FontDescriptor";
  std::string const keyEncoding = "Encoding";
  std::string const keyToUnicode = "ToUnicode";
}

namespace kernel{ namespace font{

  std::unordered_map<FontType1DictionaryKey, std::pair<std::string, uint32_t>> FontType1Dictionary::mFontType1Dictionary{
    {FontType1DictionaryKeyName,           {keyName,           ObjectTypeName}},
    {FontType1DictionaryKeyBaseFont,       {keyBaseFont,       ObjectTypeName}},
    {FontType1DictionaryKeyFirstChar,      {keyFirstChar,      ObjectTypeInteger}},
    {FontType1DictionaryKeyLastChar,       {keyLastChar,       ObjectTypeInteger}},
    {FontType1DictionaryKeyWidths,         {keyWidths,         ObjectTypeArray}},
    {FontType1DictionaryKeyFontDescriptor, {keyFontDescriptor, ObjectTypeDictionary}},
    {FontType1DictionaryKeyEncoding,       {keyEncoding,       ObjectTypeName | ObjectTypeDictionary}},
    {FontType1DictionaryKeyToUnicode,      {keyToUnicode,      ObjectTypeStream}}
  };

  FontType1Dictionary::FontType1Dictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){

    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
    ObjectDictionary::insert(keySubtype, std::make_unique<ObjectName>(valueSubtype));
  }

  void FontType1Dictionary::addKey(FontType1DictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mFontType1Dictionary, key, std::move(value));
  }
}}
