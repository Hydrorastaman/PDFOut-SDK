#include <interactive/acroform/AcroFormSignatureSVMDPDictionary.h>

namespace{
  std::string const keyP = "P";
}

namespace kernel{ namespace interactive{

  std::unordered_map<AcroFormSignatureSVMDPDictionaryKey, std::pair<std::string, uint32_t>> AcroFormSignatureSVMDPDictionary::mAcroFormSignatureSVMDPDictionaryMap{
    {AcroFormSignatureSVMDPDictionaryKeyP,     {keyP,     ObjectTypeInteger}},
  };

  AcroFormSignatureSVMDPDictionary::AcroFormSignatureSVMDPDictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){
  }

  AcroFormSignatureSVMDPDictionary::~AcroFormSignatureSVMDPDictionary(void) {}

  void AcroFormSignatureSVMDPDictionary::addKey(AcroFormSignatureSVMDPDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mAcroFormSignatureSVMDPDictionaryMap, key, std::move(value));
  }

}}
