#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{

  enum NameDictionaryKey{
    NameDictionaryKeyDests,                  // Optional; PDF 1.2
    NameDictionaryKeyAP,                     // Optional; PDF 1.3
    NameDictionaryKeyJavaScript,             // Optional; PDF 1.3
    NameDictionaryKeyPages,                  // Optional; PDF 1.3
    NameDictionaryKeyTemplates,              // Optional; PDF 1.3
    NameDictionaryKeyIDS,                    // Optional; PDF 1.3
    NameDictionaryKeyURLS,                   // Optional; PDF 1.3
    NameDictionaryKeyEmbeddedFiles,          // Optional; PDF 1.4
    NameDictionaryKeyAlternatePresentations, // Optional; PDF 1.4
    NameDictionaryKeyRenditions              // Optional; PDF 1.5
  };

  class DocumentNameDictionary : private ObjectDictionary{
  private:

  public:
    DocumentNameDictionary(void);
    ~DocumentNameDictionary(void) {}

    void addKey(NameDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<NameDictionaryKey, std::pair<std::string, uint32_t>> mDocumentNameDictionaryMap;

  private:
    DocumentNameDictionary(DocumentNameDictionary const &) = delete;
    DocumentNameDictionary &operator=(DocumentNameDictionary const &) = delete;
  };
}
