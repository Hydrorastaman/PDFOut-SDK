#include <encryption/EncryptionDictionary.h>

#include <string>

namespace{

  std::string const keyFilter = "Filter";
  std::string const keySubFilter = "SubFilter";
  std::string const keyV = "V";
  std::string const keyLength = "Length";
  std::string const keyCF = "CF";
  std::string const keyStmF = "StmF";
  std::string const keyStrF = "StrF";
  std::string const keyEFF = "EFF";

}

namespace kernel{ namespace encryption{

  std::unordered_map<EncryptionDictionaryKey, std::pair<std::string, uint32_t>> EncryptionDictionary::mEncryptionDictionaryMap{
    {EncryptionDictionaryKeyFilter,    {keyFilter,    ObjectTypeName}},
    {EncryptionDictionaryKeySubFilter, {keySubFilter, ObjectTypeName}},
    {EncryptionDictionaryKeyV,         {keyV,         ObjectTypeInteger}},
    {EncryptionDictionaryKeyLength,    {keyLength,    ObjectTypeInteger}},
    {EncryptionDictionaryKeyCF,        {keyCF,        ObjectTypeDictionary}},
    {EncryptionDictionaryKeyStmF,      {keyStmF,      ObjectTypeName}},
    {EncryptionDictionaryKeyStrF,      {keyStrF,      ObjectTypeName}},
    {EncryptionDictionaryKeyEFF,       {keyEFF,       ObjectTypeName}}
  };

  EncryptionDictionary::EncryptionDictionary(void)
    : ObjectDictionary(){
  }

  EncryptionDictionary::~EncryptionDictionary(void) {}

  void EncryptionDictionary::addKey(EncryptionDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mEncryptionDictionaryMap, key, std::move(value));
  }

}}
