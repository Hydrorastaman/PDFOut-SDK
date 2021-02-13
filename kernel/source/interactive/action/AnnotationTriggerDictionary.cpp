#include <interactive/action/AnnotationTriggerDictionary.h>

namespace{

  std::string const keyE = "E";
  std::string const keyX = "X";
  std::string const keyD = "D";
  std::string const keyU = "U";
  std::string const keyFo = "Fo";
  std::string const keyBl = "Bl";
  std::string const keyPO = "PO";
  std::string const keyPC = "PC";
  std::string const keyPV = "PV";
  std::string const keyPI = "PI";

}

namespace kernel{ namespace interactive{

  std::unordered_map<AnnotationTriggerDictionaryKey, std::pair<std::string, uint32_t>> AnnotationTriggerDictionary::mAnnotationTriggerDictionaryMap{
    {AnnotationTriggerDictionaryKeyE,  {keyE,  ObjectTypeDictionary}},
    {AnnotationTriggerDictionaryKeyX,  {keyX,  ObjectTypeDictionary}},
    {AnnotationTriggerDictionaryKeyD,  {keyD,  ObjectTypeDictionary}},
    {AnnotationTriggerDictionaryKeyU,  {keyU,  ObjectTypeDictionary}},
    {AnnotationTriggerDictionaryKeyFo, {keyFo, ObjectTypeDictionary}},
    {AnnotationTriggerDictionaryKeyBl, {keyBl, ObjectTypeDictionary}},
    {AnnotationTriggerDictionaryKeyPO, {keyPO, ObjectTypeDictionary}},
    {AnnotationTriggerDictionaryKeyPC, {keyPC, ObjectTypeDictionary}},
    {AnnotationTriggerDictionaryKeyPV, {keyPV, ObjectTypeDictionary}},
    {AnnotationTriggerDictionaryKeyPI, {keyPI, ObjectTypeDictionary}}
  };

  AnnotationTriggerDictionary::AnnotationTriggerDictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){
  }

  AnnotationTriggerDictionary::~AnnotationTriggerDictionary(void) {}

  void AnnotationTriggerDictionary::addKey(AnnotationTriggerDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mAnnotationTriggerDictionaryMap, key, std::move(value));
  }

}}
