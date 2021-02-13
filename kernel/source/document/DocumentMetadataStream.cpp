#include <document/DocumentMetadataStream.h>

#include <string>
#include <sstream>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "Metadata";

  std::string const keySubtype = "Subtype";
  std::string const valueSubtype = "XML";
}

namespace kernel{

  DocumentMetadataStream::DocumentMetadataStream(void)
    : ObjectStream(){

    ObjectStream::insert(keyType, std::make_unique<ObjectName>(valueType));
    ObjectStream::insert(keySubtype, std::make_unique<ObjectName>(valueSubtype));
  }

  DocumentMetadataStream::~DocumentMetadataStream(void){
  }

  void DocumentMetadataStream::beforeSerialize(void) const{
  }
}
