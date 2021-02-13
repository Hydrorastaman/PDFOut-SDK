#include <interactive/annotation/WidgetAnnotationDictionary.h>

namespace{
  std::string const keySubtype = "Subtype";
  std::string const valueSubtype = "Widget";

  std::string const keyH = "H";
  std::string const keyMK = "MK";
  std::string const keyA = "A";
  std::string const keyAA = "AA";
  std::string const keyBS = "BS";

}

namespace kernel{ namespace interactive{
  std::unordered_map<WidgetAnnotationDictionaryKey, std::pair<std::string , uint32_t>> WidgetAnnotationDictionary::mWidgetAnnotationMap{
    {WidgetAnnotationDictionaryKeyH,  {keyH,  ObjectTypeName}},
    {WidgetAnnotationDictionaryKeyMK, {keyMK, ObjectTypeDictionary}},
    {WidgetAnnotationDictionaryKeyA,  {keyA,  ObjectTypeDictionary}},
    {WidgetAnnotationDictionaryKeyAA, {keyAA, ObjectTypeDictionary}},
    {WidgetAnnotationDictionaryKeyBS, {keyBS, ObjectTypeDictionary}}
  };

  WidgetAnnotationDictionary::WidgetAnnotationDictionary(void)
    : AnnotationDictionary(){
    ObjectDictionary::insert(keySubtype, std::make_unique<ObjectName>(valueSubtype));
  }

  WidgetAnnotationDictionary::~WidgetAnnotationDictionary(void) {}

  void WidgetAnnotationDictionary::addKey(WidgetAnnotationDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mWidgetAnnotationMap, key, std::move(value));
  }

}}
