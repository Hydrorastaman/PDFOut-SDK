#include <graphics/SoftMaskDictionary.h>

#include <string>

#include <Function.h>
#include <object/ObjectName.h>
#include <object/ObjectArray.h>
#include <object/ObjectStream.h>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "Mask";

  std::string const keyS = "S";
  std::string const keyG = "G";
  std::string const keyBC = "BC";
  std::string const keyTR = "TR";

}

namespace kernel{

  std::unordered_map<SoftMaskDictionaryKey, std::pair<std::string, uint32_t>> SoftMaskDictionary::mSoftMaskDictionaryMap{
    {SoftMaskDictionaryKeyS,  {keyS,  ObjectTypeName}},
    {SoftMaskDictionaryKeyG,  {keyG,  ObjectTypeStream}},
    {SoftMaskDictionaryKeyBC, {keyBC, ObjectTypeArray}},
    {SoftMaskDictionaryKeyTR, {keyTR, ObjectTypeFunction | ObjectTypeName}}
  };

  SoftMaskDictionary::SoftMaskDictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){

    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
  }

  SoftMaskDictionary::~SoftMaskDictionary(void){
  }

  void SoftMaskDictionary::addKey(SoftMaskDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mSoftMaskDictionaryMap, key, std::move(value));
  }

  SoftMaskDictionary *SoftMaskDictionary::clone(void) const{
    return new SoftMaskDictionary(*this);
  }

  SoftMaskDictionary::SoftMaskDictionary(SoftMaskDictionary const &obj)
    : ObjectDictionary(obj){
  }
}
