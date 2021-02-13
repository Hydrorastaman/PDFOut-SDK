#include <interactive/action/PageTriggerDictionary.h>

namespace{

  std::string const keyO = "O";
  std::string const keyC = "C";

}

namespace kernel{ namespace interactive{

  std::unordered_map<PageTriggerDictionaryKey, std::pair<std::string, uint32_t>> PageTriggerDictionary::mPageTriggerDictionaryMap{
    {PageTriggerDictionaryKeyO, {keyO, ObjectTypeDictionary}},
    {PageTriggerDictionaryKeyC, {keyC, ObjectTypeDictionary}}
  };

  PageTriggerDictionary::PageTriggerDictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){
  }

  PageTriggerDictionary::~PageTriggerDictionary(void) {}

  void PageTriggerDictionary::addKey(PageTriggerDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mPageTriggerDictionaryMap, key, std::move(value));
  }

}}
