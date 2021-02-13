#include <object/ObjectFunctionType0.h>

#include <string>

#include <object/ObjectInteger.h>

namespace{
  std::string const keyFunctionType = "FunctionType";
  int valueFunctionType = 0;

  std::string const keyDomain = "Domain";
  std::string const keyRange = "Range";
  std::string const keySize = "Size";
  std::string const keyBitsPerSample = "BitsPerSample";
  std::string const keyOrder = "Order";
  std::string const keyEncode = "Encode";
  std::string const keyDecode = "Decode";
}

namespace kernel{
  std::unordered_map<FunctionType0StreamKey, std::pair<std::string, uint32_t>> ObjectFunctionType0::mObjectFunctionType0Map{
    {FunctionType0StreamKeyDomain,        {keyDomain,        ObjectTypeArray}},
    {FunctionType0StreamKeyRange,         {keyRange,         ObjectTypeArray}},
    {FunctionType0StreamKeySize,          {keySize,          ObjectTypeArray}},
    {FunctionType0StreamKeyBitsPerSample, {keyBitsPerSample, ObjectTypeInteger}},
    {FunctionType0StreamKeyOrder,         {keyOrder,         ObjectTypeInteger}},
    {FunctionType0StreamKeyEncode,        {keyEncode,        ObjectTypeArray}},
    {FunctionType0StreamKeyDecode,        {keyDecode,        ObjectTypeArray}}
  };

  ObjectFunctionType0::ObjectFunctionType0(void)
    : Object(ObjectTypeFunction),
    mFunctionStream(std::make_unique<ObjectStream>()){

    mFunctionStream->insert(keyFunctionType, std::make_unique<ObjectInteger>(valueFunctionType));
  }

  ObjectFunctionType0::ObjectFunctionType0(pdfout::InputStream const *stream)
    : Object(ObjectTypeFunction),
    mFunctionStream(std::make_unique<ObjectStream>(stream)){

    mFunctionStream->insert(keyFunctionType, std::make_unique<ObjectInteger>(valueFunctionType));
  }

  void ObjectFunctionType0::addKey(FunctionType0StreamKey key, std::unique_ptr<Object> value){
    mFunctionStream->addKey(mObjectFunctionType0Map, key, std::move(value));
  }
}
