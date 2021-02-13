#pragma once

#include <object/Object.h>

namespace kernel{

  class ObjectNumberTree : public Object{
  public:
    ObjectNumberTree(void) : Object(ObjectTypeNumberTree) {}
    ~ObjectNumberTree(void) {}

  protected:
    ObjectNumberTree(ObjectNumberTree const &) = delete;

  private:
    ObjectNumberTree &operator=(ObjectNumberTree const &) = delete;
  };
}
