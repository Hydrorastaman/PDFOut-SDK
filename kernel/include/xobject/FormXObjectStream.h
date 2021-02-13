#pragma once

#include <object/ObjectStream.h>

namespace kernel{ namespace xobject{

  class FormXObjectStream : public ObjectStream{
  public:
    FormXObjectStream(std::string const &subtype);
    ~FormXObjectStream(void);

  private:
    FormXObjectStream(FormXObjectStream const &) = delete;
    FormXObjectStream &operator=(FormXObjectStream const &) = delete;
  };

}}
