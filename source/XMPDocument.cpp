
#include <XMPDocument.h>
#include <Exception.h>
#include <XMPDocumentImpl.h>

namespace pdfout{
  XMPDocument::XMPDocument(void)
    : mXMPDocumentImpl(new impl::XMPDocumentImpl()){
  }

  XMPDocument::XMPDocument(pdfout::UString const &xmp)
    : mXMPDocumentImpl(new impl::XMPDocumentImpl(xmp)){
  }

  XMPDocument::~XMPDocument(void){
    if (mXMPDocumentImpl){
      delete mXMPDocumentImpl;
      mXMPDocumentImpl = nullptr;
    }
  }

  void XMPDocument::saveToStream(pdfout::OutputStream *stream) const{
    if (mXMPDocumentImpl)
      return mXMPDocumentImpl->saveToStream(stream);

      RUNTIME_EXCEPTION("Invalid impl pointer");
  }

}
