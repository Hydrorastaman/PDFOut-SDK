#include <document/DocumentInformationDictionary.h>

#include <string>

namespace{
  std::string const keyTitle = "Title";
  std::string const keyAuthor = "Author";
  std::string const keySubject = "Subject";
  std::string const keyKeywords = "Keywords";
  std::string const keyCreator = "Creator";
  std::string const keyProducer = "Producer";
  std::string const keyCreationDate = "CreationDate";
  std::string const keyModDate = "ModDate";
  std::string const keyTrapped = "Trapped";
}

namespace kernel{

  std::unordered_map<InformationDictionaryKey, std::pair<std::string, uint32_t>> DocumentInformationDictionary::mDocumentInformationDictionaryMap{
    {InformationDictionaryKeyTitle,        {keyTitle,        OBJECT_TYPE_TEXT_STRING}},
    {InformationDictionaryKeyAuthor,       {keyAuthor,       OBJECT_TYPE_TEXT_STRING}},
    {InformationDictionaryKeySubject,      {keySubject,      OBJECT_TYPE_TEXT_STRING}},
    {InformationDictionaryKeyKeywords,     {keyKeywords,     OBJECT_TYPE_TEXT_STRING}},
    {InformationDictionaryKeyCreator,      {keyCreator,      OBJECT_TYPE_TEXT_STRING}},
    {InformationDictionaryKeyProducer,     {keyProducer,     OBJECT_TYPE_TEXT_STRING}},
    {InformationDictionaryKeyCreationDate, {keyCreationDate, ObjectTypeDate}},
    {InformationDictionaryKeyModDate,      {keyModDate,      ObjectTypeDate}},
    {InformationDictionaryKeyTrapped,      {keyTrapped,      ObjectTypeName}}
  };

  void DocumentInformationDictionary::addKey(InformationDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mDocumentInformationDictionaryMap, key, std::move(value));
  }
}
