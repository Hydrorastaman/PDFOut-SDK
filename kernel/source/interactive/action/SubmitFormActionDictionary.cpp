#include <interactive/action/SubmitFormActionDictionary.h>

namespace{
  std::string const valueS = "SubmitForm";

  std::string const keyF = "F";
  std::string const keyFields = "Fields";
  std::string const keyFlags = "Flags";
}

namespace kernel{ namespace interactive{

  std::unordered_map<SubmitFormActionDictionaryKey, std::pair<std::string, uint32_t>> SubmitFormActionDictionary::mSubmitFormActionDictionaryMap{
    {SubmitFormActionDictionaryKeyF,      {keyF,      ObjectTypeFileSpecification}},
    {SubmitFormActionDictionaryKeyFields, {keyFields, ObjectTypeArray}},
    {SubmitFormActionDictionaryKeyFlags,  {keyFlags,  ObjectTypeInteger}}
  };

  SubmitFormActionDictionary::SubmitFormActionDictionary(void)
    : ActionDictionary(){

    ActionDictionary::addKey(ActionDictionaryKeyS, std::make_unique<ObjectName>(valueS));
  }

  SubmitFormActionDictionary::~SubmitFormActionDictionary(void) {}

  void SubmitFormActionDictionary::addKey(SubmitFormActionDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mSubmitFormActionDictionaryMap, key, std::move(value));
  }

}}
