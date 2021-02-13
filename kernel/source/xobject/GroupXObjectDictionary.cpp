#include <xobject/GroupXObjectDictionary.h>

#include <object/ObjectName.h>

namespace{

  std::string const keyType = "Type";
  std::string const valueType = "Group";

  std::string const keyS = "S";

}

namespace kernel{ namespace xobject{

  GroupXObjectDictionary::GroupXObjectDictionary(std::string const &subtype)
    : ObjectDictionary(IndirectTypeIndirectable){
    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
    ObjectDictionary::insert(keyS, std::make_unique<ObjectName>(subtype));
  }

  GroupXObjectDictionary::~GroupXObjectDictionary(void){
  }
}}
