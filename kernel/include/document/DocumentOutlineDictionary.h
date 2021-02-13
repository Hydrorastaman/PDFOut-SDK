#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{

  enum OutlineDictionaryKey{
    OutlineDictionaryKeyFirst, // Required if there are any open or closed outline entries; must be an indirect reference
    OutlineDictionaryKeyLast,  // Required if there are any open or closed outline entries; must be an indirect reference
    OutlineDictionaryKeyCount  // Required if the document has any open outline entries
  };

  enum OutlineItemDictionaryKey{
    OutlineItemDictionaryKeyTitle,  // Required
    OutlineItemDictionaryKeyParent, // Required; must be an indirect reference
    OutlineItemDictionaryKeyPrev,   // Required for all but the first item at each level; must be an indirect reference
    OutlineItemDictionaryKeyNext,   // Required for all but the last item at each level; must be an indirect reference
    OutlineItemDictionaryKeyFirst,  // Required if the item has any descendants; must be an indirect reference
    OutlineItemDictionaryKeyLast,   // Required if the item has any descendants; must be an indirect reference
    OutlineItemDictionaryKeyCount,  // Required if the item has any descendants
    OutlineItemDictionaryKeyDest,   // Optional; not permitted if an A entry is present
    OutlineItemDictionaryKeyA,      // Optional; PDF 1.1; not permitted if a Dest entry is present
    OutlineItemDictionaryKeySE,     // Optional; PDF 1.3; must be an indirect reference
    OutlineItemDictionaryKeyC,      // Optional; PDF 1.4
    OutlineItemDictionaryKeyF       // Optional; PDF 1.4
  };

  class DocumentOutlineItemDictionary;

  class DocumentOutlineDictionary : public ObjectDictionary{
  public:
    DocumentOutlineDictionary(void);
    ~DocumentOutlineDictionary(void) {}

    void addKey(OutlineDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<OutlineDictionaryKey, std::pair<std::string, uint32_t>> mDocumentOutlineDictionaryMap;

  private:
    DocumentOutlineDictionary(DocumentOutlineDictionary const &) = delete;
    DocumentOutlineDictionary &operator=(DocumentOutlineDictionary const &) = delete;
  };

  class DocumentOutlineItemDictionary : public ObjectDictionary{
  public:
    DocumentOutlineItemDictionary(void) : ObjectDictionary() {}
    ~DocumentOutlineItemDictionary(void) {}

    void addKey(OutlineItemDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<OutlineItemDictionaryKey, std::pair<std::string, uint32_t>> mDocumentOutlineItemDictionaryMap;

  private:
    DocumentOutlineItemDictionary(DocumentOutlineItemDictionary const &) = delete;
    DocumentOutlineItemDictionary &operator=(DocumentOutlineItemDictionary const &) = delete;
  };
}
