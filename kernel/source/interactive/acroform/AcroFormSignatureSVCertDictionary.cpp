#include <interactive/acroform/AcroFormSignatureSVCertDictionary.h>

#include <object/ObjectName.h>

namespace{

  std::string const keyType = "Type";
  std::string const valueType = "SVCert";

  std::string const keySubject = "Subject";
  std::string const keySubjectDN = "SubjectDN";
  std::string const keyKeyUsage = "KeyUsage";
  std::string const keyIssuer = "Issuer";
  std::string const keyOID = "OID";
  std::string const keyURL = "URL";
  std::string const keyURLType = "URLType";
  std::string const keyFf = "Ff";

}

namespace kernel{ namespace interactive{

  std::unordered_map<AcroFormSignatureSVCertDictionaryKey, std::pair<std::string, uint32_t>> AcroFormSignatureSVCertDictionary::mAcroFormSignatureSVCertDictionaryMap{
    {AcroFormSignatureSVCertDictionaryKeySubject,   {keySubject,   ObjectTypeArray}},
    {AcroFormSignatureSVCertDictionaryKeySubjectDN, {keySubjectDN, ObjectTypeArray | ObjectTypeDictionary}},
    {AcroFormSignatureSVCertDictionaryKeyKeyUsage,  {keyKeyUsage,  ObjectTypeArray | ObjectTypeASCIIString}},
    {AcroFormSignatureSVCertDictionaryKeyIssuer,    {keyIssuer,    ObjectTypeArray}},
    {AcroFormSignatureSVCertDictionaryKeyOID,       {keyOID,       ObjectTypeArray}},
    {AcroFormSignatureSVCertDictionaryKeyURL,       {keyURL,       ObjectTypeASCIIString}},
    {AcroFormSignatureSVCertDictionaryKeyURLType,   {keyURLType,   ObjectTypeName}},
    {AcroFormSignatureSVCertDictionaryKeyFf,        {keyFf,        ObjectTypeInteger}}
  };

  AcroFormSignatureSVCertDictionary::AcroFormSignatureSVCertDictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){
    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
  }

  AcroFormSignatureSVCertDictionary::~AcroFormSignatureSVCertDictionary(void) {}

  void AcroFormSignatureSVCertDictionary::addKey(AcroFormSignatureSVCertDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mAcroFormSignatureSVCertDictionaryMap, key, std::move(value));
  }

}}
