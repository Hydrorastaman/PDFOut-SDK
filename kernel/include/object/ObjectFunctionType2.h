#pragma once

#include <vector>
#include <memory>

#include <object/Object.h>
#include <object/ObjectDictionary.h>

namespace kernel{

  enum FunctionType2DictionaryKey{
    FunctionType2DictionaryKeyDomain, // Required. An array of 2 x m numbers, where m is the number of input values.
    FunctionType2DictionaryKeyRange,  // Optional. An array of 2 x n numbers, where n is the number of output values
    FunctionType2DictionaryKeyC0,     // Optional. An array of n numbers defining the function result when x = 0.0.
    FunctionType2DictionaryKeyC1,     // Optional. An array of n numbers defining the function result when x = 1.0.
    FunctionType2DictionaryKeyN,      // Required. The interpolation exponent. Each input value x will return n values, given by yj = C0j + xN x (C1j - C0j ), for 0 <= j < n.
  };

  class ObjectFunctionType2 : public Object{
  public:
    ObjectFunctionType2(void);
    ~ObjectFunctionType2(void) {}

    void addKey(FunctionType2DictionaryKey key, std::unique_ptr<Object> value);

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const {mFunctionDictionary->serialize(stream, params);}

    ObjectFunctionType2 *clone(void) const {return new ObjectFunctionType2(*this);}

  protected:
    ObjectFunctionType2(ObjectFunctionType2 const &obj)
      : Object(obj),
      mFunctionDictionary(obj.mFunctionDictionary->clone()){
    }

  private:
    static std::unordered_map<FunctionType2DictionaryKey, std::pair<std::string, uint32_t>> mObjectFunctionType2Map;

  private:
    ObjectFunctionType2 &operator=(ObjectFunctionType2 const &) = delete;

  private:
    std::unique_ptr<ObjectDictionary> mFunctionDictionary;
  };
}
