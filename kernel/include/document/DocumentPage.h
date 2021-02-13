#pragma once

#include <memory>

#include <object/ObjectDictionary.h>

namespace kernel{

  enum DocumentPageKey{
    DocumentPageKeyParent,               // Required; must be an indirect reference
    DocumentPageKeyLastModified,         // Required if PieceInfo is present; optional otherwise; PDF 1.3
    DocumentPageKeyResources,            // Required; inheritable
    DocumentPageKeyMediaBox,             // Required; inheritable
    DocumentPageKeyCropBox,              // Optional; inheritable
    DocumentPageKeyBleedBox,             // Optional; PDF 1.3
    DocumentPageKeyTrimBox,              // Optional; PDF 1.3
    DocumentPageKeyArtBox,               // Optional; PDF 1.3
    DocumentPageKeyBoxColorInfo,         // Optional; PDF 1.4
    DocumentPageKeyContents,             // Optional
    DocumentPageKeyRotate,               // Optional; inheritable
    DocumentPageKeyGroup,                // Optional; PDF 1.4
    DocumentPageKeyThumb,                // Optional
    DocumentPageKeyB,                    // Optional; PDF 1.1; recommended if the page contains article beads
    DocumentPageKeyDur,                  // Optional; PDF 1.1
    DocumentPageKeyTrans,                // Optional; PDF 1.1
    DocumentPageKeyAnnots,               // Optional
    DocumentPageKeyAA,                   // Optional; PDF 1.2
    DocumentPageKeyMetadata,             // Optional; PDF 1.4
    DocumentPageKeyPieceInfo,            // Optional; PDF 1.3
    DocumentPageKeyStructParents,        // Required if the page contains structural content items; PDF 1.3
    DocumentPageKeyID,                   // Optional; PDF 1.3; indirect reference preferred
    DocumentPageKeyPZ,                   // Optional; PDF 1.3
    DocumentPageKeySeparationInfo,       // Optional; PDF 1.3
    DocumentPageKeyTabs,                 // Optional; PDF 1.5
    DocumentPageKeyTemplateInstantiated, // Required if this page was created from a named page object; PDF 1.5
    DocumentPageKeyPresSteps,            // Optional; PDF 1.5
    DocumentPageKeyUserUnit,             // Optional; PDF 1.6
    DocumentPageKeyVP                    // Optional; PDF 1.6
  };

  class DocumentPage : public ObjectDictionary{
  public:
    DocumentPage(void);
    ~DocumentPage(void);

    void addKey(DocumentPageKey key, std::unique_ptr<Object> value);

    Object *find(DocumentPageKey key) const;

  private:
    static std::unordered_map<DocumentPageKey, std::pair<std::string, uint32_t>> mDocumentPageMap;

  private:
    DocumentPage(DocumentPage const &) = delete;
    DocumentPage &operator=(DocumentPage const &) = delete;
  };
}
