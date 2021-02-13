#include <font/FontType3Dictionary.h>

#include <string>

#include <object/ObjectName.h>
#include <object/ObjectArray.h>
#include <object/ObjectStream.h>
#include <object/ObjectInteger.h>
#include <object/ObjectRectangle.h>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "Font";

  std::string const keySubtype = "Subtype";
  std::string const valueSubtype = "Type3";

  std::string const keyName = "Name";
  std::string const keyFontBBox = "FontBBox";
  std::string const keyFontMatrix = "FontMatrix";
  std::string const keyCharProcs = "CharProcs";
  std::string const keyEncoding = "Encoding";
  std::string const keyFirstChar = "FirstChar";
  std::string const keyLastChar = "LastChar";
  std::string const keyWidths = "Widths";
  std::string const keyFontDescriptor = "FontDescriptor";
  std::string const keyResources = "Resources";
  std::string const keyToUnicode = "ToUnicode";
}

namespace kernel{ namespace font{

  std::unordered_map<FontType3DictionaryKey, std::pair<std::string, uint32_t>> FontType3Dictionary::mFontType3DictionaryMap{
    {FontType3DictionaryKeyName,           {keyName,           ObjectTypeName}},
    {FontType3DictionaryKeyFontBBox,       {keyFontBBox,       ObjectTypeRectangle}},
    {FontType3DictionaryKeyFontMatrix,     {keyFontMatrix,     ObjectTypeArray}},
    {FontType3DictionaryKeyCharProcs,      {keyCharProcs,      ObjectTypeDictionary}},
    {FontType3DictionaryKeyEncoding,       {keyEncoding,       ObjectTypeName | ObjectTypeDictionary}},
    {FontType3DictionaryKeyFirstChar,      {keyFirstChar,      ObjectTypeInteger}},
    {FontType3DictionaryKeyLastChar,       {keyLastChar,       ObjectTypeInteger}},
    {FontType3DictionaryKeyWidths,         {keyWidths,         ObjectTypeArray}},
    {FontType3DictionaryKeyFontDescriptor, {keyFontDescriptor, ObjectTypeDictionary}},
    {FontType3DictionaryKeyResources,      {keyResources,      ObjectTypeDictionary}},
    {FontType3DictionaryKeyToUnicode,      {keyToUnicode,      ObjectTypeStream}}
  };

  FontType3Dictionary::FontType3Dictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){

    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
    ObjectDictionary::insert(keySubtype, std::make_unique<ObjectName>(valueSubtype));
  }

  void FontType3Dictionary::addKey(FontType3DictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mFontType3DictionaryMap, key, std::move(value));
  }
}}
