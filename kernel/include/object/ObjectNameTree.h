#pragma once

#include <object/Object.h>

namespace kernel{

  class ObjectNameTree : public Object{
  public:
    ObjectNameTree(void) : Object(ObjectTypeNameTree) {}
    ~ObjectNameTree(void) {}

  protected:
    ObjectNameTree(ObjectNameTree const &) = delete;

  private:
    ObjectNameTree &operator=(ObjectNameTree const &) = delete;
  };
}
