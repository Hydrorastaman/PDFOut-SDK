#include <OutlineImpl.h>

#include <stream/OutputStream.h>
#include <object/ObjectInteger.h>
#include <document/DocumentOutlineDictionary.h>

using namespace kernel;

namespace pdfout{ namespace impl{

  OutlineImpl::OutlineImpl(DocumentOutlineDictionary *root)
    : mRoot(root){
  }

  OutlineImpl::~OutlineImpl(void) {}

  void OutlineImpl::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
    mRoot->addKey(OutlineDictionaryKeyCount, std::make_unique<ObjectInteger>(0));
    mRoot->serialize(stream, params);
  }

}}
