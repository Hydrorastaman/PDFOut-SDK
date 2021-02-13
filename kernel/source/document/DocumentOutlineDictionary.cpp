#include <document/DocumentOutlineDictionary.h>

#include <string>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "Outlines";

  std::string const keyFirst = "First";
  std::string const keyLast = "Last";
  std::string const keyCount = "Count";
}

namespace kernel{

  std::unordered_map<OutlineDictionaryKey, std::pair<std::string, uint32_t>> DocumentOutlineDictionary::mDocumentOutlineDictionaryMap{
    {OutlineDictionaryKeyFirst, {keyFirst, ObjectTypeIndirectReference}},
    {OutlineDictionaryKeyLast,  {keyLast,  ObjectTypeIndirectReference}},
    {OutlineDictionaryKeyCount, {keyCount, ObjectTypeInteger}}
  };

  DocumentOutlineDictionary::DocumentOutlineDictionary(void)
    : ObjectDictionary(){
    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
  }

  void DocumentOutlineDictionary::addKey(OutlineDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mDocumentOutlineDictionaryMap, key, std::move(value));
  }
}
