#pragma once

#include <string>
#include <map>

#include <object/ObjectStream.h>

namespace kernel{

  class DocumentMetadataStream : public ObjectStream{
  public:
    DocumentMetadataStream(void);
    ~DocumentMetadataStream(void);

    using ObjectStream::addKey;

  protected:
    void beforeSerialize(void) const;

  private:
    DocumentMetadataStream(DocumentMetadataStream const &) = delete;
    DocumentMetadataStream &operator=(DocumentMetadataStream const &) = delete;

  private:
  };

}
