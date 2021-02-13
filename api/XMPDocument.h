#pragma once

#include "PdfOutConfig.h"

#include "UString.h"
#include "stream/OutputStream.h"

namespace pdfout{

  namespace impl{
    class XMPDocumentImpl;
  }

  class PDFOUT_EXPORTS XMPDocument{
  public:
    /**
     * Creates an empty XMP document
     */
    XMPDocument(void);

    /**
     * Creates an XMP document with provided content
     */
    explicit XMPDocument(pdfout::UString const &xmp);

    ~XMPDocument(void);

    /**
     * Saves document to stream.
     */
    void saveToStream(pdfout::OutputStream *stream) const;

  private:
    XMPDocument(XMPDocument const &);
    XMPDocument &operator=(XMPDocument const &);

  private:
    impl::XMPDocumentImpl *mXMPDocumentImpl;
  };

}
