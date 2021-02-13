#include <encryption/StandardEncryptionDictionary.h>

#include <string>

namespace{

  std::string const valueFilter = "Standard";

  std::string const keyR = "R";
  std::string const keyO = "O";
  std::string const keyU = "U";
  std::string const keyP = "P";
  std::string const keyEncryptMetadata = "EncryptMetadata";
  std::string const keyOE = "OE";
  std::string const keyUE = "UE"; 
  std::string const keyPerms = "Perms";

}

namespace kernel{ namespace encryption{

  std::unordered_map<StandardEncryptionDictionaryKey, std::pair<std::string, uint32_t>> StandardEncryptionDictionary::mStandardEncryptionDictionaryMap{
    {StandardEncryptionDictionaryKeyR,               {keyR, ObjectTypeInteger}},
    {StandardEncryptionDictionaryKeyO,               {keyO, ObjectTypeByteString | OBJECT_TYPE_STRING}},
    {StandardEncryptionDictionaryKeyU,               {keyU, ObjectTypeByteString | OBJECT_TYPE_STRING}},
    {StandardEncryptionDictionaryKeyP,               {keyP, ObjectTypeInteger}},
    {StandardEncryptionDictionaryKeyEncryptMetadata, {keyEncryptMetadata, ObjectTypeBoolean}},
    {StandardEncryptionDictionaryKeyOE,              {keyOE, ObjectTypeByteString  | OBJECT_TYPE_STRING}},
    {StandardEncryptionDictionaryKeyUE,              {keyUE, ObjectTypeByteString  | OBJECT_TYPE_STRING}},
    {StandardEncryptionDictionaryKeyPerms,           {keyPerms, ObjectTypeByteString | OBJECT_TYPE_STRING}}
  };

  StandardEncryptionDictionary::StandardEncryptionDictionary(void)
    : EncryptionDictionary(){
    addKey(EncryptionDictionaryKeyFilter, std::make_unique<ObjectName>(valueFilter));
  }

  StandardEncryptionDictionary::~StandardEncryptionDictionary(void) {}

  void StandardEncryptionDictionary::addKey(StandardEncryptionDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mStandardEncryptionDictionaryMap, key, std::move(value));
  }

}}
