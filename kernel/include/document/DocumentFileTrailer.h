#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{

  enum DocumentFileTrailerKey{
    DocumentFileTrailerKeySize,    // Required; must not be an indirect reference
    DocumentFileTrailerKeyPrev,    // Present only if the file has more than one cross-reference section; must not be an indirect reference
    DocumentFileTrailerKeyRoot,    // Required; must be an indirect reference
    DocumentFileTrailerKeyEncrypt, // Required if document is encrypted; PDF 1.1
    DocumentFileTrailerKeyInfo,    // Optional; must be an indirect reference
    DocumentFileTrailerKeyID       // Optional, but strongly recommended; PDF 1.1
  };

  class DocumentFileTrailer : public ObjectDictionary{
  public:
    DocumentFileTrailer(void) : ObjectDictionary(IndirectTypeNonIndirectable) {}
    ~DocumentFileTrailer(void) {}

    void addKey(DocumentFileTrailerKey key, std::unique_ptr<Object> value);

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const;

  private:
    static std::unordered_map<DocumentFileTrailerKey, std::pair<std::string, uint32_t>> mDocumentFileTrailerMap;

  private:
    DocumentFileTrailer(DocumentFileTrailer const &) = delete;
    DocumentFileTrailer &operator=(DocumentFileTrailer const &) = delete;
  };
}
