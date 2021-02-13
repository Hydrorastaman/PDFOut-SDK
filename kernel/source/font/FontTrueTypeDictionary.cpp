#include <font/FontTrueTypeDictionary.h>

#include <object/ObjectName.h>
#include <object/ObjectArray.h>
#include <object/ObjectStream.h>
#include <object/ObjectInteger.h>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "Font";

  std::string const keySubtype = "Subtype";
  std::string const valueSubtype = "TrueType";

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

  std::unordered_map<FontTrueTypeDictionaryKey, std::pair<std::string, uint32_t>> FontTrueTypeDictionary::mFontTrueTypeDictonary{
    {FontTrueTypeDictionaryKeyName,           {keyName,           ObjectTypeName}},
    {FontTrueTypeDictionaryKeyBaseFont,       {keyBaseFont,       ObjectTypeName}},
    {FontTrueTypeDictionaryKeyFirstChar,      {keyFirstChar,      ObjectTypeInteger}},
    {FontTrueTypeDictionaryKeyLastChar,       {keyLastChar,       ObjectTypeInteger}},
    {FontTrueTypeDictionaryKeyWidths,         {keyWidths,         ObjectTypeArray}},
    {FontTrueTypeDictionaryKeyFontDescriptor, {keyFontDescriptor, ObjectTypeDictionary}},
    {FontTrueTypeDictionaryKeyEncoding,       {keyEncoding,       ObjectTypeName | ObjectTypeDictionary}},
    {FontTrueTypeDictionaryKeyToUnicode,      {keyToUnicode,      ObjectTypeStream}}
  };

  FontTrueTypeDictionary::FontTrueTypeDictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){

    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
    ObjectDictionary::insert(keySubtype, std::make_unique<ObjectName>(valueSubtype));
  }

  void FontTrueTypeDictionary::addKey(FontTrueTypeDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mFontTrueTypeDictonary, key, std::move(value));
  }
}}
