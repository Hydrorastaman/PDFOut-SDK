#include <interactive/annotation/LineAnnotationDictionary.h>

namespace{
  std::string const keySubtype = "Subtype";
  std::string const valueSubtype = "Line";

  std::string const keyL = "L";
  std::string const keyBS = "BS";
  std::string const keyLE = "LE";
  std::string const keyIC = "IC";
  std::string const keyLL = "LL";
  std::string const keyLLE = "LLE";
  std::string const keyCap = "Cap";
  std::string const keyIT = "IT";
  std::string const keyLLO = "LLO";
  std::string const keyCP = "CP";
  std::string const keyMeasure = "Measure";
  std::string const keyCO = "CO";
}

namespace kernel{ namespace interactive{
  std::unordered_map<LineAnnotationDictionaryKey, std::pair<std::string , uint32_t>> LineAnnotationDictionary::mLineAnnotationMap{
    {LineAnnotationDictionaryKeyL,       {keyL,       ObjectTypeArray}},
    {LineAnnotationDictionaryKeyBS,      {keyBS,      ObjectTypeDictionary}},
    {LineAnnotationDictionaryKeyLE,      {keyLE,      ObjectTypeArray}},
    {LineAnnotationDictionaryKeyIC,      {keyIC,      ObjectTypeArray}},
    {LineAnnotationDictionaryKeyLL,      {keyLL,      ObjectTypeNumber}},
    {LineAnnotationDictionaryKeyLLE,     {keyLLE,     ObjectTypeNumber}},
    {LineAnnotationDictionaryKeyCap,     {keyCap,     ObjectTypeBoolean}},
    {LineAnnotationDictionaryKeyIT,      {keyIT,      ObjectTypeName}},
    {LineAnnotationDictionaryKeyLLO,     {keyLLO,     ObjectTypeNumber}},
    {LineAnnotationDictionaryKeyCP,      {keyCP,      ObjectTypeName}},
    {LineAnnotationDictionaryKeyMeasure, {keyMeasure, ObjectTypeDictionary}},
    {LineAnnotationDictionaryKeyCO,      {keyCO,      ObjectTypeArray}}
  };

  LineAnnotationDictionary::LineAnnotationDictionary(void)
    : AnnotationDictionary(){
    ObjectDictionary::insert(keySubtype, std::make_unique<ObjectName>(valueSubtype));
  }

  LineAnnotationDictionary::~LineAnnotationDictionary(void) {}

  void LineAnnotationDictionary::addKey(LineAnnotationDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mLineAnnotationMap, key, std::move(value));
  }

}}
