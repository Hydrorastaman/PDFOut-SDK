#include <document/DocumentResourceDictionary.h>

namespace{
  std::string const keyExtGState = "ExtGState";
  std::string const keyColorSpace = "ColorSpace";
  std::string const keyPattern = "Pattern";
  std::string const keyShading = "Shading";
  std::string const keyXObject = "XObject";
  std::string const keyFont = "Font";
  std::string const keyProcSet = "ProcSet";
  std::string const keyProperties = "Properties";
}

namespace kernel{

  std::unordered_map<ResourceDictionaryKey, std::pair<std::string, uint32_t>> DocumentResourceDictionary::mDocumentResourceDictionaryMap{
    {ResourceDictionaryKeyExtGState,  {keyExtGState,  ObjectTypeDictionary}},
    {ResourceDictionaryKeyColorSpace, {keyColorSpace, ObjectTypeDictionary}},
    {ResourceDictionaryKeyPattern,    {keyPattern,    ObjectTypeDictionary}},
    {ResourceDictionaryKeyShading,    {keyShading,    ObjectTypeDictionary}},
    {ResourceDictionaryKeyXObject,    {keyXObject,    ObjectTypeDictionary}},
    {ResourceDictionaryKeyFont,       {keyFont,       ObjectTypeDictionary}},
    {ResourceDictionaryKeyProcSet,    {keyProcSet,    ObjectTypeArray}},
    {ResourceDictionaryKeyProperties, {keyProperties, ObjectTypeDictionary}}
  };

  DocumentResourceDictionary::DocumentResourceDictionary(void)
    : ObjectDictionary(){
  }

  DocumentResourceDictionary::~DocumentResourceDictionary(void) {}

  void DocumentResourceDictionary::clear(void){
    ObjectDictionary::clear();
  }

  void DocumentResourceDictionary::addKey(ResourceDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mDocumentResourceDictionaryMap, key, std::move(value));
  }

  Object *DocumentResourceDictionary::find(ResourceDictionaryKey key) const{
    auto iter = mDocumentResourceDictionaryMap.find(key);
    if (iter != std::end(mDocumentResourceDictionaryMap))
      return ObjectDictionary::find(iter->second.first);

    return nullptr;
  }

}
