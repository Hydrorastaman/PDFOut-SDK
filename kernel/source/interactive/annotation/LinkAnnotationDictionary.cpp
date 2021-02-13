#include <interactive/annotation/LinkAnnotationDictionary.h>

namespace{
  std::string const keySubtype = "Subtype";
  std::string const valueSubtype = "Link";

  std::string const keyA = "A";
  std::string const keyDest = "Dest";
  std::string const keyH = "H";
  std::string const keyPA = "PA";
  std::string const keyQuadPoints = "QuadPoints";
}

namespace kernel{ namespace interactive{

  std::unordered_map<LinkAnnotationDictionaryKey, std::pair<std::string , uint32_t>> LinkAnnotationDictionary::mLinkAnnotationMap{
    {LinkAnnotationDictionaryKeyA,          {keyA,          ObjectTypeDictionary}},
    {LinkAnnotationDictionaryKeyDest,       {keyDest,       ObjectTypeArray | ObjectTypeName | ObjectTypeByteString}},
    {LinkAnnotationDictionaryKeyH,          {keyH,          ObjectTypeName}},
    {LinkAnnotationDictionaryKeyPA,         {keyPA,         ObjectTypeDictionary}},
    {LinkAnnotationDictionaryKeyQuadPoints, {keyQuadPoints, ObjectTypeArray}}
  };

  LinkAnnotationDictionary::LinkAnnotationDictionary(void)
    : AnnotationDictionary(){
    ObjectDictionary::insert(keySubtype, std::make_unique<ObjectName>(valueSubtype));
  }

  LinkAnnotationDictionary::~LinkAnnotationDictionary(void) {}

  void LinkAnnotationDictionary::addKey(LinkAnnotationDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mLinkAnnotationMap, key, std::move(value));
  }

}}
