#pragma once

#include <XMPDocument.h>

namespace pdfout{ namespace impl{

  class XMPDocumentImpl{
  public:
    XMPDocumentImpl();
    explicit XMPDocumentImpl(pdfout::UString const &xmp);
    ~XMPDocumentImpl() noexcept;

    void saveToStream(pdfout::OutputStream *stream) const;

  private:
    XMPDocumentImpl(XMPDocumentImpl const &) = delete;
    XMPDocumentImpl &operator=(XMPDocumentImpl const &) = delete;

  private:
    pdfout::UString mData;
  };

}}
