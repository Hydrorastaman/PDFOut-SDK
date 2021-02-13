#include <document/DocumentPages.h>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "Pages";

  std::string const keyParent = "Parent";
  std::string const keyKids = "Kids";
  std::string const keyCount = "Count";
  std::string const keyResources = "Resources";
  std::string const keyMediaBox = "MediaBox";
  std::string const keyCropBox = "CropBox";
  std::string const keyRotate = "Rotate";
}

namespace kernel{

  std::unordered_map<DocumentPagesDictionaryKey, std::pair<std::string, uint32_t>> DocumentPagesDictionary::mDocumentPagesMap{
    {DocumentPagesDictionaryKeyParent,     {keyParent,    ObjectTypeDictionary}},
    {DocumentPagesDictionaryKeyKids,       {keyKids,      ObjectTypeArray}},
    {DocumentPagesDictionaryKeyCount,      {keyCount,     ObjectTypeInteger}},
    {DocumentPagesDictionaryKeyResources,  {keyResources, ObjectTypeDictionary}},
    {DocumentPagesDictionaryKeyMediaBox,   {keyMediaBox,  ObjectTypeRectangle}},
    {DocumentPagesDictionaryKeyCropBox,    {keyCropBox,   ObjectTypeRectangle}},
    {DocumentPagesDictionaryKeyRotate,     {keyRotate,    ObjectTypeInteger}}
  };

  DocumentPagesDictionary::DocumentPagesDictionary(void)
    : ObjectDictionary(){
    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
  }

  DocumentPagesDictionary::~DocumentPagesDictionary(void) {}

  void DocumentPagesDictionary::clear(void){
    ObjectDictionary::clear();
    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
  }

  void DocumentPagesDictionary::addKey(DocumentPagesDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mDocumentPagesMap, key, std::move(value));
  }
}
