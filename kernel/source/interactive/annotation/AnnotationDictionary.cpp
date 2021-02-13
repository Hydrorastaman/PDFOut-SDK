#include <interactive/annotation/AnnotationDictionary.h>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "Annot";

  std::string const keySubtype = "Subtype";
  std::string const keyRect = "Rect";
  std::string const keyContents = "Contents";
  std::string const keyP = "P";
  std::string const keyNM = "NM";
  std::string const keyM = "M";
  std::string const keyF = "F";
  std::string const keyAP = "AP";
  std::string const keyAS = "AS";
  std::string const keyBorder = "Border";
  std::string const keyC = "C";
  std::string const keyStructParent = "StructParent";
  std::string const keyOC = "OC";

  // markup annotations
  std::string const keyT = "T";
  std::string const keyPopup = "Popup";
  std::string const keyCA = "CA";
  std::string const keyRC = "RC";
  std::string const keyCreationDate = "CreationDate";
  std::string const keyIRT = "IRT";
  std::string const keySubj = "Subj";
  std::string const keyRT = "RT";
  std::string const keyIT = "IT";
  std::string const keyExData = "ExData";
}

namespace kernel{ namespace interactive{

  std::unordered_map<AnnotationDictionaryKey, std::pair<std::string, uint32_t>> AnnotationDictionary::mAnnotationDictionaryMap{
    {AnnotationDictionaryKeySubtype,      {keySubtype,      ObjectTypeName}},
    {AnnotationDictionaryKeyRect,         {keyRect,         ObjectTypeRectangle}},
    {AnnotationDictionaryKeyContents,     {keyContents,     OBJECT_TYPE_TEXT_STRING}},
    {AnnotationDictionaryKeyP,            {keyP,            ObjectTypeDictionary}},
    {AnnotationDictionaryKeyNM,           {keyNM,           OBJECT_TYPE_TEXT_STRING}},
    {AnnotationDictionaryKeyM,            {keyM,            ObjectTypeDate | OBJECT_TYPE_TEXT_STRING}},
    {AnnotationDictionaryKeyF,            {keyF,            ObjectTypeInteger}},
    {AnnotationDictionaryKeyAP,           {keyAP,           ObjectTypeDictionary}},
    {AnnotationDictionaryKeyAS,           {keyAS,           ObjectTypeName}},
    {AnnotationDictionaryKeyBorder,       {keyBorder,       ObjectTypeArray}},
    {AnnotationDictionaryKeyC,            {keyC,            ObjectTypeArray}},
    {AnnotationDictionaryKeyStructParent, {keyStructParent, ObjectTypeInteger}},
    {AnnotationDictionaryKeyOC,           {keyOC,           ObjectTypeDictionary}}
  };

  std::unordered_map<MarkupAnnotationDictionaryKey, std::pair<std::string, uint32_t>> AnnotationDictionary::mMarkupAnnotationDictionaryMap{
    {MarkupAnnotationDictionaryKeyT,            {keyT,            OBJECT_TYPE_TEXT_STRING}},
    {MarkupAnnotationDictionaryKeyPopup,        {keyPopup,        ObjectTypeDictionary}},
    {MarkupAnnotationDictionaryKeyCA,           {keyCA,           ObjectTypeNumber}},
    {MarkupAnnotationDictionaryKeyRC,           {keyRC,           OBJECT_TYPE_TEXT_STRING}},
    {MarkupAnnotationDictionaryKeyCreationDate, {keyCreationDate, ObjectTypeDate}},
    {MarkupAnnotationDictionaryKeyIRT,          {keyIRT,          ObjectTypeDictionary}},
    {MarkupAnnotationDictionaryKeySubj,         {keySubj,         OBJECT_TYPE_TEXT_STRING}},
    {MarkupAnnotationDictionaryKeyRT,           {keyRT,           ObjectTypeName}},
    {MarkupAnnotationDictionaryKeyIT,           {keyIT,           ObjectTypeName}},
    {MarkupAnnotationDictionaryKeyExData,       {keyExData,       ObjectTypeDictionary}}
  };

  AnnotationDictionary::AnnotationDictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){

    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
  }

  AnnotationDictionary::~AnnotationDictionary(void) {}

  void AnnotationDictionary::addKey(AnnotationDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mAnnotationDictionaryMap, key, std::move(value));
  }

  void AnnotationDictionary::addKey(MarkupAnnotationDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mMarkupAnnotationDictionaryMap, key, std::move(value));
  }

}}
