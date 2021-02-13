#include <interactive/acroform/AcroFormFieldDictionary.h>

namespace{

  std::string const keyFT = "FT";
  std::string const keyParent = "Parent";
  std::string const keyKids = "Kids";
  std::string const keyT = "T";
  std::string const keyTU = "TU";
  std::string const keyTM = "TM";
  std::string const keyFf = "Ff";
  std::string const keyV = "V";
  std::string const keyDV = "DV";
  std::string const keyAA = "AA";

  std::string const keyDA = "DA";
  std::string const keyQ = "Q";
  std::string const keyDS = "DS";
  std::string const keyRV = "RV";
}

namespace kernel{ namespace interactive{

  std::unordered_map<AcroFormFieldDictionaryKey, std::pair<std::string, uint32_t>> AcroFormFieldDictionary::mAcroFormFieldDictionaryMap{
    {AcroFormFieldDictionaryKeyFT,     {keyFT,     ObjectTypeName}},
    {AcroFormFieldDictionaryKeyParent, {keyParent, ObjectTypeDictionary}},
    {AcroFormFieldDictionaryKeyKids,   {keyKids,   ObjectTypeArray}},
    {AcroFormFieldDictionaryKeyT,      {keyT,      OBJECT_TYPE_TEXT_STRING}},
    {AcroFormFieldDictionaryKeyTU,     {keyTU,     OBJECT_TYPE_TEXT_STRING}},
    {AcroFormFieldDictionaryKeyTM,     {keyTM,     OBJECT_TYPE_TEXT_STRING}},
    {AcroFormFieldDictionaryKeyFf,     {keyFf,     ObjectTypeInteger}},
    {AcroFormFieldDictionaryKeyV,      {keyV,      ObjectTypeName | OBJECT_TYPE_TEXT_STRING | ObjectTypeDictionary}},
    {AcroFormFieldDictionaryKeyDV,     {keyDV,     ObjectTypeName | OBJECT_TYPE_TEXT_STRING | ObjectTypeDictionary}},
    {AcroFormFieldDictionaryKeyAA,     {keyAA,     ObjectTypeDictionary}},

    // for variable text
    {AcroFormFieldDictionaryKeyDA,     {keyDA,     OBJECT_TYPE_STRING}},
    {AcroFormFieldDictionaryKeyQ,      {keyQ,      ObjectTypeInteger}},
    {AcroFormFieldDictionaryKeyDS,     {keyDS,     OBJECT_TYPE_TEXT_STRING}},
    {AcroFormFieldDictionaryKeyRV,     {keyRV,     OBJECT_TYPE_TEXT_STRING}}
  };

  AcroFormFieldDictionary::AcroFormFieldDictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){
  }

  AcroFormFieldDictionary::~AcroFormFieldDictionary(void) {}

  void AcroFormFieldDictionary::addKey(AcroFormFieldDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mAcroFormFieldDictionaryMap, key, std::move(value));
  }

}}
