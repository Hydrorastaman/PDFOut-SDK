#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace xobject{

  class GroupXObjectDictionary : public ObjectDictionary{
  public:
    GroupXObjectDictionary(std::string const &subtype);
    ~GroupXObjectDictionary(void);

  private:
    GroupXObjectDictionary(GroupXObjectDictionary const &) = delete;
    GroupXObjectDictionary &operator=(GroupXObjectDictionary const &) = delete;
  };

}}
