#include <document/DocumentMarkInformationDictionary.h>

#include <string>

namespace{
  std::string const keyMarked = "Marked";
  std::string const keyUserProperties = "UserProperties";
  std::string const keySuspects = "Suspects";
}

namespace kernel{

  std::unordered_map<MarkInformationDictionaryKey, std::pair<std::string, uint32_t>> DocumentMarkInformationDictionary::mDocumentMarkInformationDictionaryMap{
    {MarkInformationDictionaryKeyMarked,              {keyMarked,         ObjectTypeBoolean}},
    {MarkInformationDictionaryKeyUserProperties,      {keyUserProperties, ObjectTypeBoolean}},
    {MarkInformationDictionaryKeySuspects,            {keySuspects,       ObjectTypeBoolean}}
  };

  void DocumentMarkInformationDictionary::addKey(MarkInformationDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mDocumentMarkInformationDictionaryMap, key, std::move(value));
  }
}
