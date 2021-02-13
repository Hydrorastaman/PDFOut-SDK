#include <interactive/annotation/SquareAnnotationDictionary.h>

namespace{
  std::string const keySubtype = "Subtype";
  std::string const valueSubtype = "Square";

  std::string const keyBS = "BS";
  std::string const keyIC = "IC";
  std::string const keyBE = "BE";
  std::string const keyRD = "RD";

}

namespace kernel{ namespace interactive{
  std::unordered_map<SquareAnnotationDictionaryKey, std::pair<std::string , uint32_t>> SquareAnnotationDictionary::mSquareAnnotationMap{
    {SquareAnnotationDictionaryKeyBS, {keyBS, ObjectTypeDictionary}},
    {SquareAnnotationDictionaryKeyIC, {keyIC, ObjectTypeArray}},
    {SquareAnnotationDictionaryKeyBE, {keyBE, ObjectTypeDictionary}},
    {SquareAnnotationDictionaryKeyRD, {keyRD, ObjectTypeRectangle}}
  };

  SquareAnnotationDictionary::SquareAnnotationDictionary(void)
    : AnnotationDictionary(){
    ObjectDictionary::insert(keySubtype, std::make_unique<ObjectName>(valueSubtype));
  }

  SquareAnnotationDictionary::~SquareAnnotationDictionary(void) {}

  void SquareAnnotationDictionary::addKey(SquareAnnotationDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mSquareAnnotationMap, key, std::move(value));
  }

}}
