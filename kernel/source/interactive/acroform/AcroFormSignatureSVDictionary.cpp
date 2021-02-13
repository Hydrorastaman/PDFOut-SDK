#include <interactive/acroform/AcroFormSignatureSVDictionary.h>

#include <object/ObjectName.h>

namespace{

  std::string const keyType = "Type";
  std::string const valueType = "SV";

  std::string const keyFilter = "Filter";
  std::string const keySubFilter = "SubFilter";
  std::string const keyDigestMethod = "DigestMethod";
  std::string const keyV = "V";
  std::string const keyCert = "Cert";
  std::string const keyReasons = "Reasons";
  std::string const keyMDP = "MDP";
  std::string const keyTimeStamp = "TimeStamp";
  std::string const keyLegalAttestation = "LegalAttestation";
  std::string const keyAddRevInfo = "AddRevInfo";
  std::string const keyFf = "Ff";

}

namespace kernel{ namespace interactive{

  std::unordered_map<AcroFormSignatureSVDictionaryKey, std::pair<std::string, uint32_t>> AcroFormSignatureSVDictionary::mAcroFormSignatureSVDictionaryMap{
    {AcroFormSignatureSVDictionaryKeyFilter,           {keyFilter,           ObjectTypeName}},
    {AcroFormSignatureSVDictionaryKeySubFilter,        {keySubFilter,        ObjectTypeArray}},
    {AcroFormSignatureSVDictionaryKeyDigestMethod,     {keyDigestMethod,     ObjectTypeArray}},
    {AcroFormSignatureSVDictionaryKeyV,                {keyV,                ObjectTypeNumber}},
    {AcroFormSignatureSVDictionaryKeyCert,             {keyCert,             ObjectTypeDictionary}},
    {AcroFormSignatureSVDictionaryKeyReasons,          {keyReasons,          ObjectTypeArray}},
    {AcroFormSignatureSVDictionaryKeyMDP,              {keyMDP,              ObjectTypeDictionary}},
    {AcroFormSignatureSVDictionaryKeyTimeStamp,        {keyTimeStamp,        ObjectTypeDictionary}},
    {AcroFormSignatureSVDictionaryKeyLegalAttestation, {keyLegalAttestation, ObjectTypeArray}},
    {AcroFormSignatureSVDictionaryKeyAddRevInfo,       {keyAddRevInfo,       ObjectTypeBoolean}},
    {AcroFormSignatureSVDictionaryKeyFf,               {keyFf,               ObjectTypeInteger}}
  };

  AcroFormSignatureSVDictionary::AcroFormSignatureSVDictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){
    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
  }

  AcroFormSignatureSVDictionary::~AcroFormSignatureSVDictionary(void) {}

  void AcroFormSignatureSVDictionary::addKey(AcroFormSignatureSVDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mAcroFormSignatureSVDictionaryMap, key, std::move(value));
  }

}}
