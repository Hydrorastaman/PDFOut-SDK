#include <graphics/halftone/HalftoneType10Dictionary.h>

#include <string>

#include <object/ObjectName.h>
#include <object/ObjectInteger.h>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "Halftone";

  std::string const keyHalftoneType = "HalftoneType";
  int valueHalftoneType = 10;

  std::string const keyHalftoneName = "HalftoneName";
  std::string const keyXsquare = "Xsquare";
  std::string const keyYsquare = "Ysquare";
  std::string const keyTransferFunction = "TransferFunction";
}

namespace kernel{

  std::unordered_map<HalftoneType10DictionaryKey, std::pair<std::string, uint32_t>> HalftoneType10Dictionary::mHalftoneType10DictionaryMap{
    {HalftoneType10DictionaryKeyHalftoneName,     {keyHalftoneName,     ObjectTypeByteString}},
    {HalftoneType10DictionaryKeyXsquare,          {keyXsquare,          ObjectTypeInteger}},
    {HalftoneType10DictionaryKeyYsquare,          {keyYsquare,          ObjectTypeInteger}},
    {HalftoneType10DictionaryKeyTransferFunction, {keyTransferFunction, ObjectTypeFunction | ObjectTypeName}},
  };

  HalftoneType10Dictionary::HalftoneType10Dictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){

    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
    ObjectDictionary::insert(keyHalftoneType, std::make_unique<ObjectInteger>(valueHalftoneType));
  }

  void HalftoneType10Dictionary::addKey(HalftoneType10DictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mHalftoneType10DictionaryMap, key, std::move(value));
  }
}
