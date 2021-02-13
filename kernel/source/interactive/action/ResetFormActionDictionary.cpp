#include <interactive/action/ResetFormActionDictionary.h>

namespace{
  std::string const valueS = "ResetForm";

  std::string const keyFields = "Fields";
  std::string const keyFlags = "Flags";
}

namespace kernel{ namespace interactive{

  std::unordered_map<ResetFormActionDictionaryKey, std::pair<std::string, uint32_t>> ResetFormActionDictionary::mResetFormActionDictionaryMap{
    {ResetFormActionDictionaryKeyFields, {keyFields, ObjectTypeArray}},
    {ResetFormActionDictionaryKeyFlags,  {keyFlags,  ObjectTypeInteger}}
 };

  ResetFormActionDictionary::ResetFormActionDictionary(void)
    : ActionDictionary(){

    ActionDictionary::addKey(ActionDictionaryKeyS, std::make_unique<ObjectName>(valueS));
  }

  ResetFormActionDictionary::~ResetFormActionDictionary(void) {}

  void ResetFormActionDictionary::addKey(ResetFormActionDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mResetFormActionDictionaryMap, key, std::move(value));
  }

}}
