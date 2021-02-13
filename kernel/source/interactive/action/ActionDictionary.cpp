#include <interactive/action/ActionDictionary.h>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "Action";

  std::string const keyS = "S";
  std::string const keyNext = "Next";
}

namespace kernel{ namespace interactive{

  std::unordered_map<ActionDictionaryKey, std::pair<std::string, uint32_t>> ActionDictionary::mActionDictionaryMap{
    {ActionDictionaryKeyS,    {keyS,    ObjectTypeName}},
    {ActionDictionaryKeyNext, {keyNext, ObjectTypeDictionary | ObjectTypeArray}}
  };

  ActionDictionary::ActionDictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){

    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
  }

  ActionDictionary::~ActionDictionary(void) {}

  void ActionDictionary::addKey(ActionDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mActionDictionaryMap, key, std::move(value));
  }

}}
