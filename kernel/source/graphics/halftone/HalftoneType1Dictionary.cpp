#include <graphics/halftone/HalftoneType1Dictionary.h>

#include <string>

#include <object/ObjectName.h>
#include <object/ObjectInteger.h>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "Halftone";

  std::string const keyHalftoneType = "HalftoneType";
  int valueHalftoneType = 1;

  std::string const keyHalftoneName = "HalftoneName";
  std::string const keyFrequency = "Frequency";
  std::string const keyAngle = "Angle";
  std::string const keySpotFunction = "SpotFunction";
  std::string const keyAccurateScreens = "AccurateScreens";
  std::string const keyTransferFunction = "TransferFunction";
}

namespace kernel{

  std::unordered_map<HalftoneType1DictionaryKey, std::pair<std::string, uint32_t>> HalftoneType1Dictionary::mHalftoneType1DictionaryMap{
    {HalftoneType1DictionaryKeyHalftoneName,     {keyHalftoneName,     ObjectTypeByteString}},
    {HalftoneType1DictionaryKeyFrequency,        {keyFrequency,        ObjectTypeNumber}},
    {HalftoneType1DictionaryKeyAngle,            {keyAngle,            ObjectTypeNumber}},
    {HalftoneType1DictionaryKeySpotFunction,     {keySpotFunction,     ObjectTypeFunction | ObjectTypeName}},
    {HalftoneType1DictionaryKeyAccurateScreens,  {keyAccurateScreens,  ObjectTypeBoolean}},
    {HalftoneType1DictionaryKeyTransferFunction, {keyTransferFunction, ObjectTypeFunction | ObjectTypeName}}
  };

  HalftoneType1Dictionary::HalftoneType1Dictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){

    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
    ObjectDictionary::insert(keyHalftoneType, std::make_unique<ObjectInteger>(valueHalftoneType));
  }

  void HalftoneType1Dictionary::addKey(HalftoneType1DictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mHalftoneType1DictionaryMap, key, std::move(value));
  }
}
