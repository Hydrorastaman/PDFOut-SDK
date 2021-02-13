#include <interactive/acroform/AcroFormTextDictionary.h>

#include <object/ObjectName.h>
#include <object/ObjectInteger.h>

namespace{

  std::string const keyOpt = "Opt";
  std::string const valueFT = "Tx";
}

namespace kernel{ namespace interactive{

  std::unordered_map<AcroFormTextDictionaryKey, std::pair<std::string, uint32_t>> AcroFormTextDictionary::mAcroFormTextDictionaryMap{
    {AcroFormTextDictionaryKeyMaxLen, {keyOpt, ObjectTypeInteger}}
  };

  AcroFormTextDictionary::AcroFormTextDictionary(void)
    : AcroFormFieldDictionary(){
    AcroFormFieldDictionary::addKey(AcroFormFieldDictionaryKeyFT, std::make_unique<ObjectName>(valueFT));
  }

  AcroFormTextDictionary::~AcroFormTextDictionary(void) {}

  void AcroFormTextDictionary::addKey(AcroFormTextDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mAcroFormTextDictionaryMap, key, std::move(value));
  }

}}
