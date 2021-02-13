#include <interactive/acroform/AcroFormChoiceDictionary.h>

namespace{

  std::string const valueFT = "Ch";

  std::string const keyOpt = "Opt";
  std::string const keyTI = "TI";
  std::string const keyI = "I";

}

namespace kernel{ namespace interactive{

  std::unordered_map<AcroFormChoiceDictionaryKey, std::pair<std::string, uint32_t>> AcroFormChoiceDictionary::mAcroFormChoiceDictionaryMap{
    {AcroFormChoiceDictionaryKeyOpt, {keyOpt, ObjectTypeArray}},
    {AcroFormChoiceDictionaryKeyTI,  {keyTI,  ObjectTypeInteger}},
    {AcroFormChoiceDictionaryKeyI,   {keyI,   ObjectTypeArray}}
  };

  AcroFormChoiceDictionary::AcroFormChoiceDictionary(void)
    : AcroFormFieldDictionary(){
    AcroFormFieldDictionary::addKey(AcroFormFieldDictionaryKeyFT, std::make_unique<ObjectName>(valueFT));
  }

  AcroFormChoiceDictionary::~AcroFormChoiceDictionary(void) {}

  void AcroFormChoiceDictionary::addKey(AcroFormChoiceDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mAcroFormChoiceDictionaryMap, key, std::move(value));
  }

}}
