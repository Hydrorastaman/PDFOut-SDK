#include <font/FontEncodingDictionary.h>

#include <string>

#include <object/ObjectName.h>
#include <object/ObjectArray.h>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "Encoding";

  std::string const keyBaseEncoding = "BaseEncoding";
  std::string const keyDifferences = "Differences";
}

namespace kernel{ namespace font{

  std::unordered_map<FontEncodingDictionaryKey, std::pair<std::string,  uint32_t>> FontEncodingDictionary::mFontEncodingDictionaryMap{
    {FontEncodingDictionaryKeyBaseEncoding, {keyBaseEncoding, ObjectTypeName}},
    {FontEncodingDictionaryKeyDifferences,  {keyDifferences,  ObjectTypeArray}}
  };

  FontEncodingDictionary::FontEncodingDictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){

    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
  }

  void FontEncodingDictionary::addKey(FontEncodingDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mFontEncodingDictionaryMap, key, std::move(value));
  }
}}
