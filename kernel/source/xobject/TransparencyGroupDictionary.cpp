#include <xobject/TransparencyGroupDictionary.h>

namespace{

  std::string const valueS = "Transparency";
  std::string const keyCS = "CS";
  std::string const keyI = "I";
  std::string const keyK = "K";

}

namespace kernel{ namespace xobject{
  std::unordered_map<TransparencyGroupDictionaryKey, std::pair<std::string, uint32_t>> TransparencyGroupDictionary::mTransparencyGroupDictionaryMap{
    {TransparencyGroupDictionaryKeyCS, {keyCS, ObjectTypeName | ObjectTypeArray}},
    {TransparencyGroupDictionaryKeyI, {keyI, ObjectTypeBoolean}},
    {TransparencyGroupDictionaryKeyK, {keyK, ObjectTypeBoolean}}
  };

  TransparencyGroupDictionary::TransparencyGroupDictionary(void)
    : GroupXObjectDictionary(valueS){
  }

  TransparencyGroupDictionary::~TransparencyGroupDictionary(void){
  }

  void TransparencyGroupDictionary::addKey(TransparencyGroupDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mTransparencyGroupDictionaryMap, key, std::move(value));
  }

}}
