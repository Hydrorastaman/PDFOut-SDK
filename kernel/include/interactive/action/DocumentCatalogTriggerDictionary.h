#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace interactive{

  enum DocumentCatalogTriggerDictionaryKey{
    DocumentCatalogTriggerDictionaryKeyWC, /**< (Optional; PDF 1.4) A JavaScript action to be performed before closing a document. (The name WC stands for “will close.”) */
    DocumentCatalogTriggerDictionaryKeyWS, /**< (Optional; PDF 1.4) A JavaScript action to be performed before saving a document. (The name WS stands for “will save.”) */
    DocumentCatalogTriggerDictionaryKeyDS, /**< (Optional; PDF 1.4) A JavaScript action to be performed after saving a document. (The name DS stands for “did save.”) */
    DocumentCatalogTriggerDictionaryKeyWP, /**< (Optional; PDF 1.4) A JavaScript action to be performed before printing a document. (The name WP stands for “will print.”) */
    DocumentCatalogTriggerDictionaryKeyDP  /**< (Optional; PDF 1.4) A JavaScript action to be performed after printing a document. (The name DP stands for “did print.”) */
  };

  /**
   * @brief Annotation dictionary
   */
  class DocumentCatalogTriggerDictionary : public ObjectDictionary{
  public:
    DocumentCatalogTriggerDictionary(void);
    ~DocumentCatalogTriggerDictionary(void);

    void addKey(DocumentCatalogTriggerDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<DocumentCatalogTriggerDictionaryKey, std::pair<std::string, uint32_t>> mDocumentCatalogTriggerDictionaryMap;

  private:
    DocumentCatalogTriggerDictionary(DocumentCatalogTriggerDictionary const &) = delete;
    DocumentCatalogTriggerDictionary &operator=(DocumentCatalogTriggerDictionary const &) = delete;
  };

}}
