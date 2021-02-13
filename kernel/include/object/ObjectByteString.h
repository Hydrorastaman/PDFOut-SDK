#pragma once

#include <vector>
#include <cstdint>

#include <object/Object.h>
#include <encryption/Encoder.h>
#include <stream/MemoryIStream.h>
#include <stream/MemoryOStream.h>
#include <filter/FilterAsciiHex.h>

namespace kernel{

  using filter::Filter;
  using filter::FilterAsciiHex;

  class ObjectByteString : public Object{
  public:
    /**
     * Creates Byte string from binary data
     * @param [in, required] data binary data
     * @param [in, required] len data length in bytes
     */
    ObjectByteString(void const *data, std::size_t len)
      : Object(ObjectTypeByteString, IndirectTypeNonIndirectable),
      mString((uint8_t *) data, (uint8_t *) data + len){
    }

    explicit ObjectByteString(std::vector<uint8_t> const &data)
      : Object(ObjectTypeByteString, IndirectTypeNonIndirectable),
      mString(data){
    }

    ~ObjectByteString(void) {}

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
      if (params && params->mStrEncoder){
        auto encoder = params->mStrEncoder->makeEncoder(params->mRootObj);
        std::unique_ptr<pdfout::MemoryIStream> tmpIStream = std::make_unique<pdfout::MemoryIStream>(mString.data(), mString.size());
        std::unique_ptr<pdfout::MemoryOStream> tmpOStream = std::make_unique<pdfout::MemoryOStream>();
        encoder->encode(tmpIStream.get(), tmpOStream.get());
        ObjectByteString byteStr(tmpOStream->getData(), tmpOStream->getSize());
        byteStr.serialize(stream, nullptr);
      }
      else{
        stream->write("<", sizeof(uint8_t), 1);

        pdfout::MemoryIStream istream((void *) mString.data(), mString.size());
        std::unique_ptr<Filter> filter = std::make_unique<FilterAsciiHex>();
        filter->encode(&istream, stream);

        // FilterAsciiHex places '>' at the end of stream
      }
    }

    void const *getData(void) const{
      return mString.data();
    }

    uint32_t getSize(void) const{
      return (uint32_t) mString.size();
    }

    ObjectByteString *clone(void) const{
      return new ObjectByteString(*this);
    }

  protected:
    ObjectByteString(ObjectByteString const &obj)
      : Object(obj),
      mString(std::begin(obj.mString), std::end(obj.mString)){
    }

  private:
    ObjectByteString &operator=(ObjectByteString const &) = delete;

  private:
    std::vector<uint8_t> mString;
  };
}
