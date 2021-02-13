#include <object/ObjectFunctionType2.h>

#include <string>

#include <object/ObjectInteger.h>

namespace{
  std::string const keyFunctionType = "FunctionType";
  int valueFunctionType = 2;

  std::string const keyDomain = "Domain";
  std::string const keyRange = "Range";
  std::string const keyC0 = "C0";
  std::string const keyC1 = "C1";
  std::string const keyN = "N";
}

namespace kernel{

  std::unordered_map<FunctionType2DictionaryKey, std::pair<std::string, uint32_t>> ObjectFunctionType2::mObjectFunctionType2Map{
    {FunctionType2DictionaryKeyDomain, {keyDomain, ObjectTypeArray}},
    {FunctionType2DictionaryKeyRange,  {keyRange,  ObjectTypeArray}},
    {FunctionType2DictionaryKeyC0,     {keyC0,     ObjectTypeArray}},
    {FunctionType2DictionaryKeyC1,     {keyC1,     ObjectTypeArray}},
    {FunctionType2DictionaryKeyN,      {keyN,      ObjectTypeNumber}}
  };

  ObjectFunctionType2::ObjectFunctionType2(void)
    : Object(ObjectTypeFunction),
    mFunctionDictionary(std::make_unique<ObjectDictionary>()){

    mFunctionDictionary->insert(keyFunctionType, std::make_unique<ObjectInteger>(valueFunctionType));
  }

  void ObjectFunctionType2::addKey(FunctionType2DictionaryKey key, std::unique_ptr<Object> value){
    mFunctionDictionary->addKey(mObjectFunctionType2Map, key, std::move(value));
  }
}
