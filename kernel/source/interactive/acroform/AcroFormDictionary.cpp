#include <interactive/acroform/AcroFormDictionary.h>

namespace{
  std::string const keyFields = "Fields";
  std::string const keyNeedAppearances = "NeedAppearances";
  std::string const keySigFlags = "SigFlags";
  std::string const keyCO = "CO";
  std::string const keyDR = "DR";
  std::string const keyDA = "DA";
  std::string const keyQ = "Q";
  std::string const keyXFA = "XFA";

}

namespace kernel{ namespace interactive{

  std::unordered_map<AcroFormDictionaryKey, std::pair<std::string, uint32_t>> AcroFormDictionary::mAcroFormDictionaryMap{
    {AcroFormDictionaryKeyFields,          {keyFields,          ObjectTypeArray}},
    {AcroFormDictionaryKeyNeedAppearances, {keyNeedAppearances, ObjectTypeBoolean}},
    {AcroFormDictionaryKeySigFlags,        {keySigFlags,        ObjectTypeInteger}},
    {AcroFormDictionaryKeyCO,              {keyCO,              ObjectTypeArray}},
    {AcroFormDictionaryKeyDR,              {keyDR,              ObjectTypeDictionary}},
    {AcroFormDictionaryKeyDA,              {keyDA,              OBJECT_TYPE_STRING}},
    {AcroFormDictionaryKeyQ,               {keyQ,               ObjectTypeInteger}},
    {AcroFormDictionaryKeyXFA,             {keyXFA,             ObjectTypeStream | ObjectTypeArray}}
  };

  AcroFormDictionary::AcroFormDictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){
  }

  AcroFormDictionary::~AcroFormDictionary(void) {}

  void AcroFormDictionary::addKey(AcroFormDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mAcroFormDictionaryMap, key, std::move(value));
  }

}}
