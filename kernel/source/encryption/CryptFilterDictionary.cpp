#include <encryption/CryptFilterDictionary.h>

#include <string>
#include <memory>

#include <object/ObjectName.h>

namespace{

  std::string const keyType = "Type";
  std::string const valueType = "CryptFilter";

  std::string const keyCFM = "CFM";
  std::string const keyAuthEvent = "AuthEvent";
  std::string const keyLength = "Length";
}

namespace kernel{ namespace encryption{

  std::unordered_map<CryptFilterDictionaryKey, std::pair<std::string, uint32_t>> CryptFilterDictionary::mCryptFilterDictionaryMap{
    {CryptFilterDictionaryKeyCFM,       {keyCFM,       ObjectTypeName}},
    {CryptFilterDictionaryKeyAuthEvent, {keyAuthEvent, ObjectTypeName}},
    {CryptFilterDictionaryKeyLength,    {keyLength,    ObjectTypeInteger}},
  };

  CryptFilterDictionary::CryptFilterDictionary()
    : ObjectDictionary(){
    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
  }

  CryptFilterDictionary::~CryptFilterDictionary(void) {}

  void CryptFilterDictionary::addKey(CryptFilterDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mCryptFilterDictionaryMap, key, std::move(value));
  }

}}
