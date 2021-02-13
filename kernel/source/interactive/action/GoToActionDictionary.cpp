#include <interactive/action/GoToActionDictionary.h>

namespace{
  std::string const valueS = "GoTo";
  std::string const keyD = "D";
}

namespace kernel{ namespace interactive{

  std::unordered_map<GoToActionDictionaryKey, std::pair<std::string, uint32_t>> GoToActionDictionary::mGoToActionDictionaryMap{
    {GoToActionDictionaryKeyD, {keyD, ObjectTypeName || ObjectTypeArray || ObjectTypeByteString}},
 };

  GoToActionDictionary::GoToActionDictionary(void)
    : ActionDictionary(){

    ActionDictionary::addKey(ActionDictionaryKeyS, std::make_unique<ObjectName>(valueS));
  }

  GoToActionDictionary::~GoToActionDictionary(void) {}

  void GoToActionDictionary::addKey(GoToActionDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mGoToActionDictionaryMap, key, std::move(value));
  }

}}
