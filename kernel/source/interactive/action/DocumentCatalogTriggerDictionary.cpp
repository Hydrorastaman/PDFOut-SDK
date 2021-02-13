#include <interactive/action/DocumentCatalogTriggerDictionary.h>

namespace{

  std::string const keyWC = "WC";
  std::string const keyWS = "WS";
  std::string const keyDS = "DS";
  std::string const keyWP = "WP";
  std::string const keyDP = "DP";

}

namespace kernel{ namespace interactive{

  std::unordered_map<DocumentCatalogTriggerDictionaryKey, std::pair<std::string, uint32_t>> DocumentCatalogTriggerDictionary::mDocumentCatalogTriggerDictionaryMap{
    {DocumentCatalogTriggerDictionaryKeyWC, {keyWC, ObjectTypeDictionary}},
    {DocumentCatalogTriggerDictionaryKeyWS, {keyWS, ObjectTypeDictionary}},
    {DocumentCatalogTriggerDictionaryKeyDS, {keyDS, ObjectTypeDictionary}},
    {DocumentCatalogTriggerDictionaryKeyWP, {keyWP, ObjectTypeDictionary}},
    {DocumentCatalogTriggerDictionaryKeyDP, {keyDP, ObjectTypeDictionary}}
  };

  DocumentCatalogTriggerDictionary::DocumentCatalogTriggerDictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){
  }

  DocumentCatalogTriggerDictionary::~DocumentCatalogTriggerDictionary(void) {}

  void DocumentCatalogTriggerDictionary::addKey(DocumentCatalogTriggerDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mDocumentCatalogTriggerDictionaryMap, key, std::move(value));
  }

}}
