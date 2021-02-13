#include <interactive/annotation/TextAnnotationDictionary.h>

namespace{
  std::string const keySubtype = "Subtype";
  std::string const valueSubtype = "Text";

  std::string const keyOpen = "Open";
  std::string const keyName = "Name";
  std::string const keyState = "State";
  std::string const keyStateModel = "StateModel";
}

namespace kernel{ namespace interactive{
  std::unordered_map<TextAnnotationDictionaryKey, std::pair<std::string , uint32_t>> TextAnnotationDictionary::mTextAnnotationMap{
    {TextAnnotationDictionaryKeyOpen,       {keyOpen,       ObjectTypeBoolean}},
    {TextAnnotationDictionaryKeyName,       {keyName,       ObjectTypeName}},
    {TextAnnotationDictionaryKeyState,      {keyState,      OBJECT_TYPE_TEXT_STRING}},
    {TextAnnotationDictionaryKeyStateModel, {keyStateModel, OBJECT_TYPE_TEXT_STRING}}
  };

  TextAnnotationDictionary::TextAnnotationDictionary(void)
    : AnnotationDictionary(){
    ObjectDictionary::insert(keySubtype, std::make_unique<ObjectName>(valueSubtype));
  }

  TextAnnotationDictionary::~TextAnnotationDictionary(void) {}

  void TextAnnotationDictionary::addKey(TextAnnotationDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mTextAnnotationMap, key, std::move(value));
  }

}}
