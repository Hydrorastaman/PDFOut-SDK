#pragma once

#include <memory>

#include <object/ObjectDictionary.h>

namespace kernel{

  enum DocumentPagesDictionaryKey{
    DocumentPagesDictionaryKeyParent,    // Required except in root node; must be an indirect reference
    DocumentPagesDictionaryKeyKids,      // Required
    DocumentPagesDictionaryKeyCount,     // Required
    DocumentPagesDictionaryKeyResources, // Required; inheritable
    DocumentPagesDictionaryKeyMediaBox,  // Required; inheritable
    DocumentPagesDictionaryKeyCropBox,   // Optional; inheritable
    DocumentPagesDictionaryKeyRotate     // Optional; inheritable
  };

  class DocumentPagesDictionary : public ObjectDictionary{
  public:
    DocumentPagesDictionary(void);
    ~DocumentPagesDictionary(void);

    void addKey(DocumentPagesDictionaryKey key, std::unique_ptr<Object> value);
    void clear(void);

  private:
    static std::unordered_map<DocumentPagesDictionaryKey, std::pair<std::string, uint32_t>> mDocumentPagesMap;

  private:
    DocumentPagesDictionary(DocumentPagesDictionary const &) = delete;
    DocumentPagesDictionary &operator=(DocumentPagesDictionary const &) = delete;
  };
}
