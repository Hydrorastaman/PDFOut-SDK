#include <XMPDocumentImpl.h>

namespace pdfout{ namespace impl{

  XMPDocumentImpl::XMPDocumentImpl()
    : mData("", pdfout::Encoding::Encoding_UTF8){
  }

  XMPDocumentImpl::XMPDocumentImpl(pdfout::UString const &xmp)
    : mData(xmp.encode(pdfout::Encoding::Encoding_UTF8)){
  }

  XMPDocumentImpl::~XMPDocumentImpl() noexcept{
  }

  void XMPDocumentImpl::saveToStream(pdfout::OutputStream *stream) const{
    stream->write(mData.getData(), sizeof(uint8_t), mData.getSize());
  }

}}
