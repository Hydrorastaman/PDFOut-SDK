#include  <interactive/annotation/AnnotationBorderStyleDictionary.h>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "Border";

  std::string const keyW = "W";
  std::string const keyS = "S";
  std::string const keyD = "D";
  std::string const keyBE = "BE";
}

namespace kernel{ namespace interactive{

  std::unordered_map<AnnotationBorderStyleKey, std::pair<std::string, uint32_t>> AnnotationBorderStyle::mAnnotationBorderStyleDictionaryMap{
    {AnnotationBorderStyleKeyW,  {keyW,  ObjectTypeNumber}},
    {AnnotationBorderStyleKeyS,  {keyS,  ObjectTypeName}},
    {AnnotationBorderStyleKeyD,  {keyD,  ObjectTypeArray}},
    {AnnotationBorderStyleKeyBE, {keyBE, ObjectTypeDictionary}},
  };

  AnnotationBorderStyle::AnnotationBorderStyle(void)
    : ObjectDictionary(IndirectTypeNonIndirectable){

    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
  }

  AnnotationBorderStyle::~AnnotationBorderStyle(void) {}

  void AnnotationBorderStyle::addKey(AnnotationBorderStyleKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mAnnotationBorderStyleDictionaryMap, key, std::move(value));
  }

}}
