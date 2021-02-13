#include <interactive/acroform/AcroFormSignatureLockDictionary.h>

#include <object/ObjectName.h>

namespace{

  std::string const keyType = "Type";
  std::string const valueType = "SigFieldLock";

  std::string const keyAction = "Action";
  std::string const keyFields = "Fields";

}

namespace kernel{ namespace interactive{

  std::unordered_map<AcroFormSignatureLockDictionaryKey, std::pair<std::string, uint32_t>> AcroFormSignatureLockDictionary::mAcroFormSignatureLockDictionaryMap{
    {AcroFormSignatureLockDictionaryKeyAction, {keyAction, ObjectTypeName}},
    {AcroFormSignatureLockDictionaryKeyFields, {keyFields, ObjectTypeArray}}
  };

  AcroFormSignatureLockDictionary::AcroFormSignatureLockDictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){

    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
  }

  AcroFormSignatureLockDictionary::~AcroFormSignatureLockDictionary(void) {}

  void AcroFormSignatureLockDictionary::addKey(AcroFormSignatureLockDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mAcroFormSignatureLockDictionaryMap, key, std::move(value));
  }

}}
