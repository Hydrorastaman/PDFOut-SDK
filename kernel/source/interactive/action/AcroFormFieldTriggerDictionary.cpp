#include <interactive/action/AcroFormFieldTriggerDictionary.h>

namespace{

  std::string const keyK = "K";
  std::string const keyF = "F";
  std::string const keyV = "V";
  std::string const keyC = "C";

}

namespace kernel{ namespace interactive{

  std::unordered_map<AcroFormFieldTriggerDictionaryKey, std::pair<std::string, uint32_t>> AcroFormFieldTriggerDictionary::mAcroFormFieldTriggerDictionaryMap{
    {AcroFormFieldTriggerDictionaryKeyK, {keyK, ObjectTypeDictionary}},
    {AcroFormFieldTriggerDictionaryKeyF, {keyF, ObjectTypeDictionary}},
    {AcroFormFieldTriggerDictionaryKeyV, {keyV, ObjectTypeDictionary}},
    {AcroFormFieldTriggerDictionaryKeyC, {keyC, ObjectTypeDictionary}}
  };

  AcroFormFieldTriggerDictionary::AcroFormFieldTriggerDictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){
  }

  AcroFormFieldTriggerDictionary::~AcroFormFieldTriggerDictionary(void) {}

  void AcroFormFieldTriggerDictionary::addKey(AcroFormFieldTriggerDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mAcroFormFieldTriggerDictionaryMap, key, std::move(value));
  }

}}
