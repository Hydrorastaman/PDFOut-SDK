#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{

  enum MarkInformationDictionaryKey{
    MarkInformationDictionaryKeyMarked,         // (Optional) A flag indicating whether the document conforms to Tagged PDF conventions.
    MarkInformationDictionaryKeyUserProperties, // (Optional) (Optional; PDF 1.6) A flag indicating the presence of structure elements that contain user properties attributes (see "User Properties" on page 876).
    MarkInformationDictionaryKeySuspects,       // (Optional; PDF 1.6) A flag indicating the presence of tag suspects (see "Page Content Order" on page 889).
  };

  class DocumentMarkInformationDictionary : public ObjectDictionary{
  public:
    DocumentMarkInformationDictionary(void) : ObjectDictionary() {}
    ~DocumentMarkInformationDictionary(void) {}

    void addKey(MarkInformationDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<MarkInformationDictionaryKey, std::pair<std::string, uint32_t>> mDocumentMarkInformationDictionaryMap;

  private:
    DocumentMarkInformationDictionary(DocumentMarkInformationDictionary const &) = delete;
    DocumentMarkInformationDictionary &operator=(DocumentMarkInformationDictionary const &) = delete;
  };
}
