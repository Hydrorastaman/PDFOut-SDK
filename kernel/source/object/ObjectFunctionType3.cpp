#include <object/ObjectFunctionType3.h>

#include <string>

#include <object/ObjectInteger.h>

namespace{
  std::string const keyFunctionType = "FunctionType";
  int valueFunctionType = 3;

  std::string const keyDomain = "Domain";
  std::string const keyRange = "Range";
  std::string const keyFunctions = "Functions";
  std::string const keyBounds = "Bounds";
  std::string const keyEncode = "Encode";
}

namespace kernel{

  std::unordered_map<FunctionType3DictionaryKey, std::pair<std::string, uint32_t>> ObjectFunctionType3::mObjectFunctionType3Map{
    {FunctionType3DictionaryKeyDomain,    {keyDomain,    ObjectTypeArray}},
    {FunctionType3DictionaryKeyRange,     {keyRange,     ObjectTypeArray}},
    {FunctionType3DictionaryKeyFunctions, {keyFunctions, ObjectTypeArray}},
    {FunctionType3DictionaryKeyBounds,    {keyBounds,    ObjectTypeArray}},
    {FunctionType3DictionaryKeyEncode,    {keyEncode,    ObjectTypeArray}}
  };

  ObjectFunctionType3::ObjectFunctionType3(void)
    : Object(ObjectTypeFunction),
    mFunctionDictionary(std::make_unique<ObjectDictionary>()){

    mFunctionDictionary->insert(keyFunctionType, std::make_unique<ObjectInteger>(valueFunctionType));
  }

  void ObjectFunctionType3::addKey(FunctionType3DictionaryKey key, std::unique_ptr<Object> value){
    mFunctionDictionary->addKey(mObjectFunctionType3Map, key, std::move(value));
  }
}
