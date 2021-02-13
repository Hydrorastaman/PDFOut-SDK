#include <interactive/action/JavaScriptActionDictionary.h>

namespace{
  std::string const valueS = "JavaScript";

  std::string const keyJS = "JS";
}

namespace kernel{ namespace interactive{

  std::unordered_map<JavaScriptActionDictionaryKey, std::pair<std::string, uint32_t>> JavaScriptActionDictionary::mJavaScriptActionDictionaryMap{
    {JavaScriptActionDictionaryKeyJS, {keyJS, OBJECT_TYPE_TEXT_STRING}}
  };

  JavaScriptActionDictionary::JavaScriptActionDictionary(void)
    : ActionDictionary(){

    ActionDictionary::addKey(ActionDictionaryKeyS, std::make_unique<ObjectName>(valueS));
  }

  JavaScriptActionDictionary::~JavaScriptActionDictionary(void) {}

  void JavaScriptActionDictionary::addKey(JavaScriptActionDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mJavaScriptActionDictionaryMap, key, std::move(value));
  }

}}
