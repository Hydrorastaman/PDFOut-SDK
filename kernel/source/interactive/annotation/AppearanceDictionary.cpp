#include <interactive/annotation/AppearanceDictionary.h>

namespace{
  std::string const keyN = "N";
  std::string const keyR = "R";
  std::string const keyD = "D";
}

namespace kernel{ namespace interactive{

  std::unordered_map<AppearanceDictionaryKey, std::pair<std::string, uint32_t>> AppearanceDictionary::mAppearanceDictionaryMap{
    {AppearanceDictionaryKeyN, {keyN, ObjectTypeStream | ObjectTypeDictionary}},
    {AppearanceDictionaryKeyR, {keyR, ObjectTypeStream | ObjectTypeDictionary}},
    {AppearanceDictionaryKeyD, {keyD, ObjectTypeStream | ObjectTypeDictionary}}
  };

  AppearanceDictionary::AppearanceDictionary(void)
    : ObjectDictionary(IndirectTypeNonIndirectable){
  }

  AppearanceDictionary::~AppearanceDictionary(void) {}

  void AppearanceDictionary::addKey(AppearanceDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mAppearanceDictionaryMap, key, std::move(value));
  }

}}
