#include <interactive/acroform/AcroFormButtonDictionary.h>

#include <object/ObjectName.h>
#include <object/ObjectInteger.h>

namespace{

  std::string const keyOpt = "Opt";
  std::string const valueFT = "Btn";

}

namespace kernel{ namespace interactive{

  std::unordered_map<AcroFormButtonDictionaryKey, std::pair<std::string, uint32_t>> AcroFormButtonDictionary::mAcroFormButtonDictionaryMap{
    {AcroFormButtonDictionaryKeyOpt, {keyOpt, ObjectTypeArray | OBJECT_TYPE_TEXT_STRING}}
  };

  AcroFormButtonDictionary::AcroFormButtonDictionary(void)
    : AcroFormFieldDictionary(){

    AcroFormFieldDictionary::addKey(AcroFormFieldDictionaryKeyFT, std::make_unique<ObjectName>(valueFT));
  }

  AcroFormButtonDictionary::~AcroFormButtonDictionary(void) {}

  void AcroFormButtonDictionary::addKey(AcroFormButtonDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mAcroFormButtonDictionaryMap, key, std::move(value));
  }

}}
