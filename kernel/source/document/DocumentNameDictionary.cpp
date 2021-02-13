#include <document/DocumentNameDictionary.h>

namespace{
  std::string const keyDests = "Dests";
  std::string const keyAP = "AP";
  std::string const keyJavaScript = "JavaScript";
  std::string const keyPages = "Pages";
  std::string const keyTemplates = "Templates";
  std::string const keyIDS = "IDS";
  std::string const keyURLS = "URLS";
  std::string const keyEmbeddedFiles = "EmbeddedFiles";
  std::string const keyAlternatePresentations = "AlternatePresentations";
  std::string const keyRenditions = "Renditions";
}

namespace kernel{
  std::unordered_map<NameDictionaryKey, std::pair<std::string, uint32_t>> DocumentNameDictionary::mDocumentNameDictionaryMap{
    {NameDictionaryKeyDests,                  {keyDests,                  ObjectTypeNameTree}},
    {NameDictionaryKeyAP,                     {keyAP,                     ObjectTypeNameTree}},
    {NameDictionaryKeyJavaScript,             {keyJavaScript,             ObjectTypeNameTree}},
    {NameDictionaryKeyPages,                  {keyPages,                  ObjectTypeNameTree}},
    {NameDictionaryKeyTemplates,              {keyTemplates,              ObjectTypeNameTree}},
    {NameDictionaryKeyIDS,                    {keyIDS,                    ObjectTypeNameTree}},
    {NameDictionaryKeyURLS,                   {keyURLS,                   ObjectTypeNameTree}},
    {NameDictionaryKeyEmbeddedFiles,          {keyEmbeddedFiles,          ObjectTypeNameTree}},
    {NameDictionaryKeyAlternatePresentations, {keyAlternatePresentations, ObjectTypeNameTree}},
    {NameDictionaryKeyRenditions,             {keyRenditions,             ObjectTypeNameTree}}
  };


  DocumentNameDictionary::DocumentNameDictionary(void)
    : ObjectDictionary(){
  }

  void DocumentNameDictionary::addKey(NameDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mDocumentNameDictionaryMap, key, std::move(value));
  }
}
