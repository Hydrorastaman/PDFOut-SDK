#pragma once

#include <vector>
#include <memory>

#include <object/Object.h>
#include <object/ObjectDictionary.h>

namespace kernel{

  enum FunctionType3DictionaryKey{
    FunctionType3DictionaryKeyDomain,    // Required. An array of 2 x m numbers, where m is the number of input values.
    FunctionType3DictionaryKeyRange,     // Optional. An array of 2 x n numbers, where n is the number of output values.
    FunctionType3DictionaryKeyFunctions, // Required. An array of k 1_input functions making up the stitching function.
    FunctionType3DictionaryKeyBounds,    // Required. An array of k - 1 numbers that, in combination with Domain, define the intervals to which each function from the Functions array applies.
    FunctionType3DictionaryKeyEncode     // Required. An array of 2 x k numbers that, taken in pairs, map each subset of the domain defined by Domain and the Bounds array to the domain of the corresponding function.
  };

  class ObjectFunctionType3 : public Object{
  public:
    ObjectFunctionType3(void);
    ~ObjectFunctionType3(void) {}

    void addKey(FunctionType3DictionaryKey key, std::unique_ptr<Object> value);

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const {mFunctionDictionary->serialize(stream, params);}

    ObjectFunctionType3 *clone(void) const {return new ObjectFunctionType3(*this);}

  protected:
    ObjectFunctionType3(ObjectFunctionType3 const &obj)
      : Object(obj),
      mFunctionDictionary(obj.mFunctionDictionary->clone()){
    }

  private:
    static std::unordered_map<FunctionType3DictionaryKey, std::pair<std::string, uint32_t>> mObjectFunctionType3Map;

  private:
    ObjectFunctionType3 &operator=(ObjectFunctionType3 const &) = delete;

  private:
    std::unique_ptr<ObjectDictionary> mFunctionDictionary;
  };
}
