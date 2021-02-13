#include <HalftoneImpl.h>

#include <object/Object.h>

namespace pdfout{ namespace impl{

  HalftoneImpl::HalftoneImpl(kernel::Object *HalftoneImpl)
    : Halftone(),
    mHalftone(HalftoneImpl){
  }

  HalftoneImpl::~HalftoneImpl(void) {}

  void HalftoneImpl::serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const{
    mHalftone->serialize(stream, params);
  }

  kernel::Object *HalftoneImpl::getObject(void) const{
    return mHalftone.get();
  }

}}

