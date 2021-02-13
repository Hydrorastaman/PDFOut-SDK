#include <graphics/halftone/HalftoneType16Dictionary.h>

#include <string>

#include <object/ObjectName.h>
#include <object/ObjectInteger.h>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "Halftone";

  std::string const keyHalftoneType = "HalftoneType";
  int valueHalftoneType = 16;

  std::string const keyHalftoneName = "HalftoneName";
  std::string const keyWidth = "Width";
  std::string const keyHeight = "Height";
  std::string const keyWidth2 = "Width2";
  std::string const keyHeight2 = "Height2";
  std::string const keyTransferFunction = "TransferFunction";
}

namespace kernel{
  std::unordered_map<HalftoneType16DictionaryKey, std::pair<std::string, uint32_t>> HalftoneType16Dictionary::mHalftoneType16DictionaryMap{
    {HalftoneType16DictionaryKeyHalftoneName,     {keyHalftoneName,     ObjectTypeByteString}},
    {HalftoneType16DictionaryKeyWidth,            {keyWidth,            ObjectTypeInteger}},
    {HalftoneType16DictionaryKeyHeight,           {keyHeight,           ObjectTypeInteger}},
    {HalftoneType16DictionaryKeyWidth2,           {keyWidth2,           ObjectTypeInteger}},
    {HalftoneType16DictionaryKeyHeight2,          {keyHeight2,          ObjectTypeInteger}},
    {HalftoneType16DictionaryKeyTransferFunction, {keyTransferFunction, ObjectTypeFunction | ObjectTypeName}},
  };

  HalftoneType16Dictionary::HalftoneType16Dictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){

    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
    ObjectDictionary::insert(keyHalftoneType, std::make_unique<ObjectInteger>(valueHalftoneType));
  }

  void HalftoneType16Dictionary::addKey(HalftoneType16DictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mHalftoneType16DictionaryMap, key, std::move(value));
  }
}
