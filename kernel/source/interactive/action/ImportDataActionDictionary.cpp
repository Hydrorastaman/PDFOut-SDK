#include <interactive/action/ImportDataActionDictionary.h>

namespace{
  std::string const valueS = "ImportData";

  std::string const keyF = "F";
}

namespace kernel{ namespace interactive{

  std::unordered_map<ImportDataActionDictionaryKey, std::pair<std::string, uint32_t>> ImportDataActionDictionary::mImportDataActionDictionaryMap{
    {ImportDataActionDictionaryKeyF,      {keyF, ObjectTypeFileSpecification}},
  };

  ImportDataActionDictionary::ImportDataActionDictionary(void)
    : ActionDictionary(){

    ActionDictionary::addKey(ActionDictionaryKeyS, std::make_unique<ObjectName>(valueS));
  }

  ImportDataActionDictionary::~ImportDataActionDictionary(void) {}

  void ImportDataActionDictionary::addKey(ImportDataActionDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mImportDataActionDictionaryMap, key, std::move(value));
  }

}}
