#include <object/ObjectFunctionType4.h>

#include <string>

#include <object/ObjectInteger.h>

namespace{
  std::string const keyFunctionType = "FunctionType";
  int valueFunctionType = 4;

  std::string const keyDomain = "Domain";
  std::string const keyRange = "Range";
}

namespace kernel{

  std::unordered_map<FunctionType4StreamKey, std::pair<std::string, uint32_t>> ObjectFunctionType4::mObjectFunctionType4Map{
    {FunctionType4StreamKeyDomain, {keyDomain, ObjectTypeArray}},
    {FunctionType4StreamKeyRange,  {keyRange, ObjectTypeArray}}
  };

  ObjectFunctionType4::ObjectFunctionType4(void)
    : Object(ObjectTypeFunction),
    mFunctionStream(std::make_unique<ObjectStream>()){

    mFunctionStream->insert(keyFunctionType, std::make_unique<ObjectInteger>(valueFunctionType));
  }

  ObjectFunctionType4::ObjectFunctionType4(pdfout::InputStream const *stream)
    : Object(ObjectTypeFunction),
    mFunctionStream(std::make_unique<ObjectStream>(stream)){

    mFunctionStream->insert(keyFunctionType, std::make_unique<ObjectInteger>(valueFunctionType));
  }

  ObjectFunctionType4::ObjectFunctionType4(char const *stream)
    : Object(ObjectTypeFunction),
    mFunctionStream(std::make_unique<ObjectStream>(stream)){

    mFunctionStream->insert(keyFunctionType, std::make_unique<ObjectInteger>(valueFunctionType));
  }

  void ObjectFunctionType4::addKey(FunctionType4StreamKey key, std::unique_ptr<Object> value){
    mFunctionStream->addKey(mObjectFunctionType4Map, key, std::move(value));
  }
}
