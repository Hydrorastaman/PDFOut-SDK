#include <interactive/annotation/CircleAnnotationDictionary.h>

namespace{
  std::string const keySubtype = "Subtype";
  std::string const valueSubtype = "Circle";

  std::string const keyBS = "BS";
  std::string const keyIC = "IC";
  std::string const keyBE = "BE";
  std::string const keyRD = "RD";

}

namespace kernel{ namespace interactive{
  std::unordered_map<CircleAnnotationDictionaryKey, std::pair<std::string , uint32_t>> CircleAnnotationDictionary::mCircleAnnotationMap{
    {CircleAnnotationDictionaryKeyBS, {keyBS, ObjectTypeDictionary}},
    {CircleAnnotationDictionaryKeyIC, {keyIC, ObjectTypeArray}},
    {CircleAnnotationDictionaryKeyBE, {keyBE, ObjectTypeDictionary}},
    {CircleAnnotationDictionaryKeyRD, {keyRD, ObjectTypeRectangle}}
  };

  CircleAnnotationDictionary::CircleAnnotationDictionary(void)
    : AnnotationDictionary(){
    ObjectDictionary::insert(keySubtype, std::make_unique<ObjectName>(valueSubtype));
  }

  CircleAnnotationDictionary::~CircleAnnotationDictionary(void) {}

  void CircleAnnotationDictionary::addKey(CircleAnnotationDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mCircleAnnotationMap, key, std::move(value));
  }

}}
