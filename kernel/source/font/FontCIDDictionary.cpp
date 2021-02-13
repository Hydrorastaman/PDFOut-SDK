#include <font/FontCIDDictionary.h>

#include <string>

#include <object/ObjectName.h>
#include <object/ObjectInteger.h>
#include <object/ObjectArray.h>
#include <object/ObjectStream.h>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "Font";

  std::string const keySubtype = "Subtype";
  std::string const valueSubtype0 = "CIDFontType0";
  std::string const valueSubtype2 = "CIDFontType2";

  std::string const keyBaseFont = "BaseFont";
  std::string const keyCIDSystemInfo = "CIDSystemInfo";
  std::string const keyFontDescriptor = "FontDescriptor";
  std::string const keyDW = "DW";
  std::string const keyW = "W";
  std::string const keyDW2 = "DW2";
  std::string const keyW2 = "W2";
  std::string const keyCIDToGIDMap = "CIDToGIDMap";
}

namespace kernel{ namespace font{

  std::unordered_map<FontCIDDictionaryKey, std::pair<std::string, uint32_t>> FontCIDDictionary::mFontCIDDictionaryMap{
    {FontCIDDictionaryKeyBaseFont,       {keyBaseFont,       ObjectTypeName}},
    {FontCIDDictionaryKeyCIDSystemInfo,  {keyCIDSystemInfo,  ObjectTypeDictionary}},
    {FontCIDDictionaryKeyFontDescriptor, {keyFontDescriptor, ObjectTypeDictionary}},
    {FontCIDDictionaryKeyDW,             {keyDW,             ObjectTypeInteger}},
    {FontCIDDictionaryKeyW,              {keyW,              ObjectTypeArray}},
    {FontCIDDictionaryKeyDW2,            {keyDW2,            ObjectTypeArray}},
    {FontCIDDictionaryKeyW2,             {keyW2,             ObjectTypeArray}},
    {FontCIDDictionaryKeyCIDToGIDMap,    {keyCIDToGIDMap,    ObjectTypeStream | ObjectTypeName}}
  };

  FontCIDDictionary::FontCIDDictionary(CIDFontType type)
    : ObjectDictionary(IndirectTypeIndirectable){

    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));

    switch (type){
      case CIDFontType0:
        ObjectDictionary::insert(keySubtype, std::make_unique<ObjectName>(valueSubtype0));
        break;

      case CIDFontType2:
        ObjectDictionary::insert(keySubtype, std::make_unique<ObjectName>(valueSubtype2));
        break;
    };
  }

  void FontCIDDictionary::addKey(FontCIDDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mFontCIDDictionaryMap, key, std::move(value));
  }
}}
