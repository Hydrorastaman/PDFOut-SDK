#include <document/DocumentOutlineDictionary.h>

#include <string>

namespace{
  std::string const keyTitle = "Title";
  std::string const keyParent = "Parent";
  std::string const keyPrev = "Prev";
  std::string const keyNext = "Next";
  std::string const keyFirst = "First";
  std::string const keyLast = "Last";
  std::string const keyCount = "Count";
  std::string const keyDest = "Dest";
  std::string const keyA = "A";
  std::string const keySE = "SE";
  std::string const keyC = "C";
  std::string const keyF = "F";
}

namespace kernel{

  std::unordered_map<OutlineItemDictionaryKey, std::pair<std::string, uint32_t>> DocumentOutlineItemDictionary::mDocumentOutlineItemDictionaryMap{
    {OutlineItemDictionaryKeyTitle,  {keyTitle,  OBJECT_TYPE_TEXT_STRING}},
    {OutlineItemDictionaryKeyParent, {keyParent, ObjectTypeDictionary}},
    {OutlineItemDictionaryKeyPrev,   {keyPrev,   ObjectTypeDictionary}},
    {OutlineItemDictionaryKeyNext,   {keyNext,   ObjectTypeDictionary}},
    {OutlineItemDictionaryKeyFirst,  {keyFirst,  ObjectTypeDictionary}},
    {OutlineItemDictionaryKeyLast,   {keyLast,   ObjectTypeDictionary}},
    {OutlineItemDictionaryKeyCount,  {keyCount,  ObjectTypeInteger}},
    {OutlineItemDictionaryKeyDest,   {keyDest,   ObjectTypeName | ObjectTypeArray | ObjectTypeByteString}},
    {OutlineItemDictionaryKeyA,      {keyA,      ObjectTypeDictionary}},
    {OutlineItemDictionaryKeySE,     {keySE,     ObjectTypeDictionary}},
    {OutlineItemDictionaryKeyC,      {keyC,      ObjectTypeArray}},
    {OutlineItemDictionaryKeyF,      {keyF,      ObjectTypeInteger}}
  };

  void DocumentOutlineItemDictionary::addKey(OutlineItemDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mDocumentOutlineItemDictionaryMap, key, std::move(value));
  }
}
