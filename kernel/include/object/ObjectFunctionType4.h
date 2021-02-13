#pragma once

#include <vector>
#include <memory>

#include <object/Object.h>
#include <object/ObjectStream.h>

namespace kernel{

  enum FunctionType4StreamKey{
    FunctionType4StreamKeyDomain,        // Required. An array of 2 x m numbers, where m is the number of input values.
    FunctionType4StreamKeyRange          // Required. An array of 2 x n numbers, where n is the number of output values.
  };

  class ObjectFunctionType4 : public Object{
  public:
    ObjectFunctionType4(void);
    explicit ObjectFunctionType4(pdfout::InputStream const *stream);
    explicit ObjectFunctionType4(char const *stream);
    ~ObjectFunctionType4(void) {}

    uint64_t write(void const *data, uint32_t sizeOfElements, uint64_t numOfElements) {return mFunctionStream->write(data, sizeOfElements, numOfElements);}
    void addKey(StreamDictionaryKey key, std::unique_ptr<Object> value) {mFunctionStream->addKey(key, std::move(value));}
    void addKey(FunctionType4StreamKey key, std::unique_ptr<Object> value);

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const {mFunctionStream->serialize(stream, params);}

    ObjectFunctionType4 *clone(void) const {return new ObjectFunctionType4(*this);}

  protected:
    ObjectFunctionType4(ObjectFunctionType4 const &obj)
      : Object(obj),
      mFunctionStream(obj.mFunctionStream->clone()){
    }

  private:
    static std::unordered_map<FunctionType4StreamKey, std::pair<std::string, uint32_t>> mObjectFunctionType4Map;

  private:
    ObjectFunctionType4 &operator=(ObjectFunctionType4 const &) = delete;

  private:
    std::unique_ptr<ObjectStream> mFunctionStream;  
  };
}
