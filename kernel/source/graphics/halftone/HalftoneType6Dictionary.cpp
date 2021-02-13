#include <graphics/halftone/HalftoneType6Dictionary.h>

#include <string>

#include <object/ObjectName.h>
#include <object/ObjectInteger.h>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "Halftone";

  std::string const keyHalftoneType = "HalftoneType";
  int valueHalftoneType = 6;

  std::string const keyHalftoneName = "HalftoneName";
  std::string const keyWidth = "Width";
  std::string const keyHeight = "Height";
  std::string const keyTransferFunction = "TransferFunction";
}

namespace kernel{
  std::unordered_map<HalftoneType6DictionaryKey, std::pair<std::string, uint32_t>> HalftoneType6Dictionary::mHalftoneType6DictionaryMap{
    {HalftoneType6DictionaryKeyHalftoneName,     {keyHalftoneName,     ObjectTypeByteString}},
    {HalftoneType6DictionaryKeyWidth,            {keyWidth,            ObjectTypeInteger}},
    {HalftoneType6DictionaryKeyHeight,           {keyHeight,           ObjectTypeInteger}},
    {HalftoneType6DictionaryKeyTransferFunction, {keyTransferFunction, ObjectTypeFunction | ObjectTypeName}}
  };

  HalftoneType6Dictionary::HalftoneType6Dictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){

    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
    ObjectDictionary::insert(keyHalftoneType, std::make_unique<ObjectInteger>(valueHalftoneType));
  }

  void HalftoneType6Dictionary::addKey(HalftoneType6DictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mHalftoneType6DictionaryMap, key, std::move(value));
  }
}
