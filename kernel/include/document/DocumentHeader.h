#pragma once

#include <Types.h>
#include <Serializable.h>

namespace kernel{

  class DocumentHeader : public Serializable{
  public:
    DocumentHeader(pdfout::DocumentVersion documentVersion) : mDocumentVersion(documentVersion) {}
    ~DocumentHeader(void) {}

    pdfout::DocumentVersion getDocumentVersion(void) const {return mDocumentVersion;}

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const;

  private:
    DocumentHeader(DocumentHeader const &) = delete;
    DocumentHeader &operator=(DocumentHeader const &) = delete;

  private:
    pdfout::DocumentVersion mDocumentVersion;
  };
}
