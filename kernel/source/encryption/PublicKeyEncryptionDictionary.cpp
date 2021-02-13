#include <encryption/PublicKeyEncryptionDictionary.h>

#include <string>

namespace{

  std::string const keyRecipients = "Recipients";

}

namespace kernel{ namespace encryption{

  std::unordered_map<PublicKeyEncryptionDictionaryKey, std::pair<std::string, uint32_t>> PublicKeyEncryptionDictionary::mPublicKeyEncryptionDictionaryMap{
    {PublicKeyEncryptionDictionaryKeyRecipients, {keyRecipients, ObjectTypeArray}}
  };

  PublicKeyEncryptionDictionary::PublicKeyEncryptionDictionary(void)
    : EncryptionDictionary(){
  }

  PublicKeyEncryptionDictionary::~PublicKeyEncryptionDictionary(void) {}

  void PublicKeyEncryptionDictionary::addKey(PublicKeyEncryptionDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mPublicKeyEncryptionDictionaryMap, key, std::move(value));
  }

}}
