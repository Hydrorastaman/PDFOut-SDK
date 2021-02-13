#include <graphics/halftone/HalftoneType5Dictionary.h>

#include <string>

#include <object/ObjectName.h>
#include <object/ObjectInteger.h>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "Halftone";

  std::string const keyHalftoneType = "HalftoneType";
  int valueHalftoneType = 5;

  std::string const keyHalftoneName = "HalftoneName";
  std::string const keyDefault = "Default";
  std::string const keyGray = "Gray";
  std::string const keyRed = "Red";
  std::string const keyGreen = "Green";
  std::string const keyBlue = "Blue";
  std::string const keyCyan = "Cyan";
  std::string const keyMagenta = "Magenta";
  std::string const keyYellow = "Yellow";
  std::string const keyBlack = "Black";
}

namespace kernel{
  std::unordered_map<HalftoneType5DictionaryKey, std::pair<std::string, uint32_t>> HalftoneType5Dictionary::mHalftoneType5DictionaryMap{
    {HalftoneType5DictionaryKeyHalftoneName, {keyHalftoneName, ObjectTypeByteString}},
    {HalftoneType5DictionaryKeyDefault,      {keyDefault,      ObjectTypeFunction | ObjectTypeName}},
    {HalftoneType5DictionaryKeyGray,         {keyGray,         ObjectTypeDictionary | ObjectTypeStream}},
    {HalftoneType5DictionaryKeyRed,          {keyRed,          ObjectTypeDictionary | ObjectTypeStream}},
    {HalftoneType5DictionaryKeyGreen,        {keyGreen,        ObjectTypeDictionary | ObjectTypeStream}},
    {HalftoneType5DictionaryKeyBlue,         {keyBlue,         ObjectTypeDictionary | ObjectTypeStream}},
    {HalftoneType5DictionaryKeyCyan,         {keyCyan,         ObjectTypeDictionary | ObjectTypeStream}},
    {HalftoneType5DictionaryKeyMagenta,      {keyMagenta,      ObjectTypeDictionary | ObjectTypeStream}},
    {HalftoneType5DictionaryKeyYellow,       {keyYellow,       ObjectTypeDictionary | ObjectTypeStream}},
    {HalftoneType5DictionaryKeyBlack,        {keyBlack,        ObjectTypeDictionary | ObjectTypeStream}}
  };

  HalftoneType5Dictionary::HalftoneType5Dictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){

    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
    ObjectDictionary::insert(keyHalftoneType, std::make_unique<ObjectInteger>(valueHalftoneType));
  }

  void HalftoneType5Dictionary::addKey(HalftoneType5DictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mHalftoneType5DictionaryMap, key, std::move(value));
  }
}
