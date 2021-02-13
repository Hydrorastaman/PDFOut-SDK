#pragma once

#include <object/Object.h>

namespace kernel{

  class ObjectNull : public Object{
  public:
    ObjectNull(void) : Object(ObjectTypeNull, IndirectTypeNonIndirectable) {}

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const {stream->write("null", sizeof(uint8_t), 4);}

    ObjectNull *clone(void) const {return new ObjectNull(*this);}

  protected:
    ObjectNull(ObjectNull const &obj) : Object(obj) {}

  private:
    ObjectNull &operator=(ObjectNull const &) = delete;
  };
}
