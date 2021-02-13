#include <object/MacOSFileInformationDictionary.h>

namespace{

  std::string const keySubtype = "Subtype";
  std::string const keyCreator = "Creator";
  std::string const keyResFork = "ResFork";

}

namespace kernel{

  std::unordered_map<MacOSFileInformationDictionaryKey, std::pair<std::string, uint32_t>> MacOSFileInformationDictionary::mMacOSFileInformationDictionaryMap{
    {MacOSFileInformationDictionaryKeySubtype, {keySubtype, ObjectTypeInteger}},
    {MacOSFileInformationDictionaryKeyCreator, {keyCreator, ObjectTypeInteger}},
    {MacOSFileInformationDictionaryKeyResFork, {keyResFork, ObjectTypeStream}}
  };

  MacOSFileInformationDictionary::MacOSFileInformationDictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){
  }

  MacOSFileInformationDictionary::~MacOSFileInformationDictionary(void) {}

  void MacOSFileInformationDictionary::addKey(MacOSFileInformationDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mMacOSFileInformationDictionaryMap, key, std::move(value));
  }

}
