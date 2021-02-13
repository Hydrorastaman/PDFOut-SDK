#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{

  enum ResourceDictionaryKey{
    ResourceDictionaryKeyExtGState,  // Optional
    ResourceDictionaryKeyColorSpace, // Optional
    ResourceDictionaryKeyPattern,    // Optional
    ResourceDictionaryKeyShading,    // Optional; PDF 1.3
    ResourceDictionaryKeyXObject,    // Optional
    ResourceDictionaryKeyFont,       // Optional
    ResourceDictionaryKeyProcSet,    // Optional
    ResourceDictionaryKeyProperties  // Optional; PDF 1.2
  };

  class DocumentResourceDictionary : public ObjectDictionary{
  public:
    DocumentResourceDictionary(void);
    ~DocumentResourceDictionary(void);

    void addKey(ResourceDictionaryKey key, std::unique_ptr<Object> value);
    void clear(void);

    Object *find(ResourceDictionaryKey key) const;

  private:
    static std::unordered_map<ResourceDictionaryKey, std::pair<std::string, uint32_t>> mDocumentResourceDictionaryMap;

  private:
    DocumentResourceDictionary(DocumentResourceDictionary const &) = delete;
    DocumentResourceDictionary &operator=(DocumentResourceDictionary const &) = delete;
  };
}
