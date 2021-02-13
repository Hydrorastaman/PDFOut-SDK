#pragma once

#include <vector>
#include <memory>

#include <object/Object.h>
#include <object/ObjectStream.h>

namespace kernel{

  enum FunctionType0StreamKey{
    FunctionType0StreamKeyDomain,        // Required. An array of 2 x m numbers, where m is the number of input values.
    FunctionType0StreamKeyRange,         // Required. An array of 2 x n numbers, where n is the number of output values.
    FunctionType0StreamKeySize,          // Required. An array of m positive integers.
    FunctionType0StreamKeyBitsPerSample, // Required. The number of bits used to represent each sample.
    FunctionType0StreamKeyOrder,         // Optional. The order of interpolation between samples.
    FunctionType0StreamKeyEncode,        // Optional. An array of 2 x m numbers specifying the linear mapping of input values into the domain of the function's sample table.
    FunctionType0StreamKeyDecode,        // Optional. An array of 2 x n numbers specifying the linear mapping of sample values into the range appropriate for the function's output values.
  };

  class ObjectFunctionType0 : public Object{
  public:
    ObjectFunctionType0(void);
    explicit ObjectFunctionType0(pdfout::InputStream const *stream);
    ~ObjectFunctionType0(void) {}

    uint64_t write(void const *data, uint32_t sizeOfElements, uint64_t numOfElements) {return mFunctionStream->write(data, sizeOfElements, numOfElements);}
    void addKey(StreamDictionaryKey key, std::unique_ptr<Object> value) {mFunctionStream->addKey(key, std::move(value));}
    void addKey(FunctionType0StreamKey key, std::unique_ptr<Object> value);

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const {mFunctionStream->serialize(stream, params);}
    ObjectFunctionType0 *clone(void) const {return new ObjectFunctionType0(*this);}

  private:
    static std::unordered_map<FunctionType0StreamKey, std::pair<std::string, uint32_t>> mObjectFunctionType0Map;

  protected:
    ObjectFunctionType0(ObjectFunctionType0 const &obj)
      : Object(obj),
      mFunctionStream(obj.mFunctionStream->clone()){
    }

  private:
    ObjectFunctionType0 &operator=(ObjectFunctionType0 const &) = delete;

  private:
    std::unique_ptr<ObjectStream> mFunctionStream;
  };
}
