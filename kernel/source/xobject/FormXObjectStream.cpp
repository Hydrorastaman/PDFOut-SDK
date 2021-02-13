#include <xobject/FormXObjectStream.h>

namespace{

  std::string const keyType = "Type";
  std::string const valueType = "XObject";

  std::string const keySubtype = "Subtype";
}

namespace kernel{ namespace xobject{

  FormXObjectStream::FormXObjectStream(std::string const &subtype)
    : ObjectStream(IndirectTypeIndirectable){
    ObjectStream::insert(keyType, std::make_unique<ObjectName>(valueType));
    ObjectStream::insert(keySubtype, std::make_unique<ObjectName>(subtype));
  }

  FormXObjectStream::~FormXObjectStream(void) {}

}}
