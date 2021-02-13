#include <FunctionImpl.h>

#include <object/Object.h>

namespace pdfout{ namespace impl{

  FunctionImpl::FunctionImpl(kernel::Object *function)
    : Function(),
    mFunction(function){
  }

  FunctionImpl::~FunctionImpl(void) {}

  void FunctionImpl::serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const{
    mFunction->serialize(stream, params);
  }

  kernel::Object const *FunctionImpl::getObject(void) const{
    return mFunction.get();
  }

}}
