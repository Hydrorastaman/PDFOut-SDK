#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <memory>
#include <unordered_map>

#include <object/Object.h>
#include <filter/Filter.h>
#include <encryption/Encoder.h>
#include <stream/MemoryIStream.h>
#include <stream/MemoryOStream.h>

namespace{
  std::unordered_map<uint8_t, uint8_t> escCharMap({
    {'\n', 'n'},
    {'\r', 'r'},
    {'\t', 't'},
    {'\b', 'b'},
    {'\f', 'f'},
    {')', ')'},
    {'(', '('},
    {'\\', '\\'}
  }, 256);
}

namespace kernel{

  class ObjectASCIIString : public Object{
  public:
    explicit ObjectASCIIString(char const *data)
      : Object(ObjectTypeASCIIString, IndirectTypeNonIndirectable),
      mString(data, data + strlen(data)) {}

    ObjectASCIIString(char const *data, std::size_t len)
      : Object(ObjectTypeASCIIString, IndirectTypeNonIndirectable),
      mString(data, data + len) {}

    ObjectASCIIString(uint8_t const *data, std::size_t len)
      : Object(ObjectTypeASCIIString, IndirectTypeNonIndirectable),
      mString(data, data + len) {}

    explicit ObjectASCIIString(std::string const &data)
      : Object(ObjectTypeASCIIString, IndirectTypeNonIndirectable),
      mString(std::begin(data), std::end(data)) {}

    ~ObjectASCIIString(void) {}

    virtual void serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
      if (params && params->mStrEncoder){
        auto encoder = params->mStrEncoder->makeEncoder(params->mRootObj);
        std::unique_ptr<pdfout::MemoryIStream> tmpIStream = std::make_unique<pdfout::MemoryIStream>(mString.data(), mString.size());
        std::unique_ptr<pdfout::MemoryOStream> tmpOStream = std::make_unique<pdfout::MemoryOStream>();
        encoder->encode(tmpIStream.get(), tmpOStream.get());
        ObjectASCIIString asciiStr((uint8_t const *) tmpOStream->getData(), tmpOStream->getSize());
        asciiStr.serialize(stream, nullptr);
      }
      else{
        stream->write("(", sizeof(uint8_t), 1);
        serialize2(stream);
        stream->write(")", sizeof(uint8_t), 1);
      }
    }

    ObjectASCIIString *clone(void) const{
      return new ObjectASCIIString(*this);
    }

    char const *getData(void) const{
      return mString.data();
    }

    uint32_t getSize(void) const{
      return mString.size();
    }

  protected:
    ObjectASCIIString(ObjectASCIIString const &obj)
      : Object(obj),
      mString(obj.mString){
    }

    void serialize2(pdfout::OutputStream *stream) const{
      uint8_t ch = 0;
      for (auto const &elem: mString){
        ch = escCharMap[elem];
        if (ch != 0){
          stream->write("\\", sizeof(uint8_t), 1);
          stream->write(&ch, sizeof(uint8_t), 1);
        }
        else
          stream->write(&elem, sizeof(uint8_t), 1);
      }
    }

  private:
    ObjectASCIIString &operator=(ObjectASCIIString const &) = delete;

  private:
    std::vector<char> mString;
  };
}
