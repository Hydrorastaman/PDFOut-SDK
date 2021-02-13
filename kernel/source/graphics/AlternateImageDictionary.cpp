#include <graphics/AlternateImageDictionary.h>

#include <string>

namespace{
  std::string const keyImage = "Image";
  std::string const keyDefaultForPrinting = "DefaultForPrinting";
  std::string const keyOC = "OC";
}

namespace kernel{

  std::unordered_map<AlternateImageDictionaryKey, std::pair<std::string, uint32_t>> AlternateImageDictionary::mAlternateImageDictionaryMap{
    {AlternateImageDictionaryKeyImage,              {keyImage,              ObjectTypeStream}},
    {AlternateImageDictionaryKeyDefaultForPrinting, {keyDefaultForPrinting, ObjectTypeBoolean}},
    {AlternateImageDictionaryKeyOC,                 {keyOC,                 ObjectTypeDictionary}}
  };

  AlternateImageDictionary::AlternateImageDictionary(void)
    : ObjectDictionary(){
  }

  void AlternateImageDictionary::addKey(AlternateImageDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mAlternateImageDictionaryMap, key, std::move(value));
  }
}

