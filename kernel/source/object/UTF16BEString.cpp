#include <object/UTF16BEString.h>

#include <iomanip>
#include <sstream>

#include <Serializable.h>
#include <InternalTools.h>
#include <filter/Filter.h>
#include <encryption/Encoder.h>
#include <stream/MemoryIStream.h>
#include <stream/MemoryOStream.h>

using namespace pdfout;

namespace kernel{
  UTF16BEString::UTF16BEString(pdfout::UString const &str){
    pdfout::UString utf16str = str.encode(Encoding_UTF16BE);
    mString.resize(utf16str.getSize() / sizeof(uint16_t));
    memcpy(mString.data(), utf16str.getData(), utf16str.getSize());
  }

  UTF16BEString::UTF16BEString(uint16_t const *data, std::size_t len)
    : mString(data, data + len){
  }

  UTF16BEString::UTF16BEString(UTF16BEString const &obj)
    : mString(obj.mString){
  }

  UTF16BEString::~UTF16BEString(void) {}

  void UTF16BEString::serialize(pdfout::OutputStream *stream, SerializeParams *params, bool isBOM) const{
    Endian const endian = getEndian();

    std::stringstream ss;
    if (isBOM){
      ss << std::setfill('0') << std::setw(2) << std::hex << (int32_t) 0xFE;
      ss << std::setfill('0') << std::setw(2) << std::hex << (int32_t) 0xFF;
    }

    if (endian == EndianLittle){
      uint16_t value = 0;
      for (std::size_t i = 0, size = mString.size(); i < size; ++i){
        value = mString[i];
        swapByteOrder(value);
        if (value)
          ss << std::setfill('0') << std::setw(4) << std::hex << (int32_t) (value);
      }
    }
    else{
      for (std::size_t i = 0, size = mString.size(); i < size; ++i){
        if (uint16_t const value = mString[i])
          ss << std::setfill('0') << std::setw(4) << std::hex << (int32_t) (value);
      }
    }

    std::string const hexStr = ss.str();

    if (params && params->mStrEncoder){
      auto encoder = params->mStrEncoder->makeEncoder(params->mRootObj);
      std::unique_ptr<pdfout::MemoryIStream> istream = std::make_unique<pdfout::MemoryIStream>(hexStr.data(), hexStr.length());
      std::unique_ptr<pdfout::MemoryOStream> ostream = std::make_unique<pdfout::MemoryOStream>();
      ostream->write("<", sizeof(uint8_t), 1);
      encoder->encode(istream.get(), ostream.get());
      ostream->write(">", sizeof(uint8_t), 1);
      stream->write(ostream->getData(), sizeof(uint8_t), ostream->getSize());
    }
    else{
      stream->write("<", sizeof(uint8_t), 1);
      stream->write(hexStr.data(), sizeof(uint8_t), hexStr.length());
      stream->write(">", sizeof(uint8_t), 1);
    }
  }

  UTF16BEString *UTF16BEString::clone(void) const{
    return new UTF16BEString(*this);
  }

  bool UTF16BEString::isSpecial(uint16_t ch) const{
    return (ch == '(' || ch == ')' || ch == '\n' || ch == '\r' || ch == '\t' || ch == '\b' || ch == '\f' || ch == '\\');
  }
}
