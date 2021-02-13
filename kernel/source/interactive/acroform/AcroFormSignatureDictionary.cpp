#include <interactive/acroform/AcroFormSignatureDictionary.h>

namespace{

  std::string const valueFT = "Sig";

  std::string const keyLock = "Lock";
  std::string const keySV = "SV";
}

namespace kernel{ namespace interactive{

  std::unordered_map<AcroFormSignatureDictionaryKey, std::pair<std::string, uint32_t>> AcroFormSignatureDictionary::mAcroFormSignatureDictionaryMap{
    {AcroFormSignatureDictionaryKeyLock, {keyLock, ObjectTypeDictionary}}, // must be indirect reference
    {AcroFormSignatureDictionaryKeySV  , {keySV,   ObjectTypeDictionary}}  // must be indirect reference
  };

  AcroFormSignatureDictionary::AcroFormSignatureDictionary(void)
    : AcroFormFieldDictionary(){
    AcroFormFieldDictionary::addKey(AcroFormFieldDictionaryKeyFT, std::make_unique<ObjectName>(valueFT));
  }

  AcroFormSignatureDictionary::~AcroFormSignatureDictionary(void) {}

  void AcroFormSignatureDictionary::addKey(AcroFormSignatureDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mAcroFormSignatureDictionaryMap, key, std::move(value));
  }

}}
