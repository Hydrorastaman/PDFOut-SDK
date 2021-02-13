#include <filter/FilterAsciiHex.h>

#include <cstdint>

#include <stream/OutputStream.h>
#include <stream/InputStream.h>

namespace{
  unsigned char const NUL = 0x00;
  unsigned char const TAB = 0x09;
  unsigned char const SPACE = 0x20;
  unsigned char const LF = 0x0A;
  unsigned char const FF = 0x0C;
  unsigned char const CR = 0x0D;
}

namespace kernel{ namespace filter{
  bool FilterAsciiHex::encode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream){
    if (!istream || !ostream)
      return false;

    uint32_t const buffSize = 4096;
    unsigned char buff[buffSize] = {0};

    uint64_t read = 0;
    unsigned char ch = 0;
    unsigned char const offset0 = (unsigned char) '0';
    unsigned char const offset1 = (unsigned char) 'a' - 10;
    unsigned char value = 0;

    while (read = istream->read(&buff[0], sizeof(uint8_t), buffSize)){
      for (std::size_t i = 0; i < read; ++i){
        ch = buff[i];

        value = (ch >> 4) + ((ch >> 4) < 10 ? offset0 : offset1);
        ostream->write(&value, sizeof(uint8_t), 1);

        value = (ch & 0xF) + ((ch & 0xF) < 10 ? offset0 : offset1);
        ostream->write(&value, sizeof(uint8_t), 1);
      }
    }

    ostream->write(">", sizeof(uint8_t), 1);

    return true;
  }

  bool FilterAsciiHex::decode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream){
    if (!istream || !ostream)
      return false;

    uint32_t const buffSize = 4096;
    unsigned char buff[buffSize] = {0};

    uint64_t read = 0;
    unsigned char ch = 0;

    unsigned char firstDigit = 0xFF;
    unsigned char digit = 0;
    unsigned char value = 0;

    while (read = istream->read(&buff[0], sizeof(uint8_t), buffSize)){
      for (std::size_t i = 0; i < read; ++i){
        ch = buff[i];
        if (ch >= '0' && ch <= '9')
          digit = ch & 0x0F;
        else if ((ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f'))
          digit = (ch & 0x0F) + 9;
        else if (ch == '>') {
          if (firstDigit != 0xFF){ // incomplete byte
            value = firstDigit << 4;
            ostream->write(&value, sizeof(uint8_t), 1);
          }
          return true;
        }
        else if (ch == NUL || ch == TAB || ch == LF || ch == CR || ch == FF || ch == SPACE)
          continue;
        else
          return false;

        if (firstDigit == 0xFF)
          firstDigit = digit;
        else{
          value = (firstDigit << 4) | digit;
          ostream->write(&value, sizeof(uint8_t), 1);
          firstDigit = 0xFF;
        }
      }
    }

    return false;
  }

  FilterAsciiHex *FilterAsciiHex::clone(void) const{
    return new FilterAsciiHex();
  }
}}
