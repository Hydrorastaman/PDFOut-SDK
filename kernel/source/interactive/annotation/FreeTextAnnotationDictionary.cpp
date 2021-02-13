#include <interactive/annotation/FreeTextAnnotationDictionary.h>

namespace{
  std::string const keySubtype = "Subtype";
  std::string const valueSubtype = "FreeText";

  std::string const keyDA = "DA";
  std::string const keyQ = "Q";
  std::string const keyRC = "RC";
  std::string const keyDS = "DS";
  std::string const keyCL = "CL";
  std::string const keyIT = "IT";
  std::string const keyBE = "BE";
  std::string const keyRD = "RD";
  std::string const keyBS = "BS";
  std::string const keyLE = "LE";
}

namespace kernel{ namespace interactive{

  std::unordered_map<FreeTextAnnotationDictionaryKey, std::pair<std::string , uint32_t>> FreeTextAnnotationDictionary::mFreeTextAnnotationMap{
    {FreeTextAnnotationDictionaryKeyDA, {keyDA, OBJECT_TYPE_STRING}},
    {FreeTextAnnotationDictionaryKeyQ,  {keyQ,  ObjectTypeInteger}},
    {FreeTextAnnotationDictionaryKeyRC, {keyRC, OBJECT_TYPE_TEXT_STRING}}, // or text stream
    {FreeTextAnnotationDictionaryKeyDS, {keyDS, OBJECT_TYPE_TEXT_STRING}},
    {FreeTextAnnotationDictionaryKeyCL, {keyCL, ObjectTypeArray}},
    {FreeTextAnnotationDictionaryKeyIT, {keyIT, ObjectTypeName}},
    {FreeTextAnnotationDictionaryKeyBE, {keyBE, ObjectTypeDictionary}},
    {FreeTextAnnotationDictionaryKeyRD, {keyRD, ObjectTypeRectangle}},
    {FreeTextAnnotationDictionaryKeyBS, {keyBS, ObjectTypeDictionary}},
    {FreeTextAnnotationDictionaryKeyLE, {keyLE, ObjectTypeArray}}
  };

  FreeTextAnnotationDictionary::FreeTextAnnotationDictionary(void)
    : AnnotationDictionary(){
    ObjectDictionary::insert(keySubtype, std::make_unique<ObjectName>(valueSubtype));
  }

  FreeTextAnnotationDictionary::~FreeTextAnnotationDictionary(void) {}

  void FreeTextAnnotationDictionary::addKey(FreeTextAnnotationDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mFreeTextAnnotationMap, key, std::move(value));
  }

}}
