#include <interactive/annotation/WidgetAnnotationMKDictionary.h>

namespace{
  std::string const keyR = "R";
  std::string const keyBC = "BC";
  std::string const keyBG = "BG";
  std::string const keyCA = "CA";
  std::string const keyRC = "RC";
  std::string const keyAC = "AC";
  std::string const keyI = "I";
  std::string const keyRI = "RI";
  std::string const keyIX = "IX";
  std::string const keyIF = "IF";
  std::string const keyTP = "TP";
}

namespace kernel{ namespace interactive{
  std::unordered_map<WidgetAnnotationMKDictionaryKey, std::pair<std::string , uint32_t>> WidgetAnnotationMKDictionary::mWidgetAnnotationMKMap{
    {WidgetAnnotationMKDictionaryKeyR,  {keyR,  ObjectTypeInteger}},
    {WidgetAnnotationMKDictionaryKeyBC, {keyBC, ObjectTypeArray}},
    {WidgetAnnotationMKDictionaryKeyBG, {keyBG, ObjectTypeArray}},
    {WidgetAnnotationMKDictionaryKeyCA, {keyCA, OBJECT_TYPE_TEXT_STRING}},
    {WidgetAnnotationMKDictionaryKeyRC, {keyRC, OBJECT_TYPE_TEXT_STRING}},
    {WidgetAnnotationMKDictionaryKeyAC, {keyAC, OBJECT_TYPE_TEXT_STRING}},
    {WidgetAnnotationMKDictionaryKeyI,  {keyI,  ObjectTypeStream}},
    {WidgetAnnotationMKDictionaryKeyRI, {keyRI, ObjectTypeStream}},
    {WidgetAnnotationMKDictionaryKeyIX, {keyIX, ObjectTypeStream}},
    {WidgetAnnotationMKDictionaryKeyIF, {keyIF, ObjectTypeDictionary}},
    {WidgetAnnotationMKDictionaryKeyTP, {keyTP, ObjectTypeInteger}}
  };

  WidgetAnnotationMKDictionary::WidgetAnnotationMKDictionary(IndirectType indirectable)
    : ObjectDictionary(indirectable){
  }

  WidgetAnnotationMKDictionary::~WidgetAnnotationMKDictionary(void) {}

  void WidgetAnnotationMKDictionary::addKey(WidgetAnnotationMKDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mWidgetAnnotationMKMap, key, std::move(value));
  }

}}
