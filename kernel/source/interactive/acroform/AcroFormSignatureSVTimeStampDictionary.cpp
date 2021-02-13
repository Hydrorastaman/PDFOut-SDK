#include <interactive/acroform/AcroFormSignatureSVTimeStampDictionary.h>

namespace{
  std::string const keyURL = "URL";
  std::string const keyFf = "Ff";
}

namespace kernel{ namespace interactive{

  std::unordered_map<AcroFormSignatureSVTimeStampDictionaryKey, std::pair<std::string, uint32_t>> AcroFormSignatureSVTimeStampDictionary::mAcroFormSignatureSVTimeStampDictionaryMap{
    {AcroFormSignatureSVTimeStampDictionaryKeyURL, {keyURL, ObjectTypeASCIIString}},
    {AcroFormSignatureSVTimeStampDictionaryKeyFf , {keyFf, ObjectTypeInteger}}
  };

  AcroFormSignatureSVTimeStampDictionary::AcroFormSignatureSVTimeStampDictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){
  }

  AcroFormSignatureSVTimeStampDictionary::~AcroFormSignatureSVTimeStampDictionary(void) {}

  void AcroFormSignatureSVTimeStampDictionary::addKey(AcroFormSignatureSVTimeStampDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mAcroFormSignatureSVTimeStampDictionaryMap, key, std::move(value));
  }

}}
