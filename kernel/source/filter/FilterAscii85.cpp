#include <filter/FilterAscii85.h>

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
  void FilterAscii85::valueToWord85(uint32_t value, unsigned char word85[5]) const{
    word85[4] = value % 85 + '!';
    value /= 85;
    word85[3] = value % 85 + '!';
    value /= 85;
    word85[2] = value % 85 + '!';
    value /= 85;
    word85[1] = value % 85 + '!';
    value /= 85;
    word85[0] = value + '!';
  }

  uint32_t FilterAscii85::word85ToValue(unsigned char word85[5]) const{
    uint32_t value = word85[0] - '!';
    for (std::size_t i = 1; i < 5; ++i)
      value = value * 85 + (word85[i] - '!');

    return value;
  }

  void FilterAscii85::valueToAsciiWord(uint32_t value, unsigned char wordAscii[4]) const{
    for (int i = 3; i >= 0; --i){
      wordAscii[i] = value & 0xFF;
      value >>= 8;
    }
  }

  uint32_t FilterAscii85::asciiWordToValue(unsigned char wordAscii[4]) const{
    return ((uint32_t) wordAscii[0] << 24)
            + ((uint32_t) wordAscii[1] << 16)
            + ((uint32_t) wordAscii[2] << 8)
            + (uint32_t) wordAscii[3];
  }

  bool FilterAscii85::encode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream){
    if (!istream || !ostream)
      return false;

    uint32_t const buffSize = 4096;
    unsigned char buff[buffSize] = {0};

    std::size_t read = 0;
    std::size_t words = 0;
    std::size_t tail = 0;
    uint32_t value = 0;
    std::size_t const word85Size = 5;
    unsigned char word85[word85Size] = {0};

    while (read = (std::size_t) istream->read(&buff[tail], sizeof(uint8_t), buffSize - tail)){
      read += tail;
      words = read / 4;
      tail = read % 4;
      for (std::size_t i = 0; i < words; ++i){
        value = asciiWordToValue(&buff[i * 4]);
        if (value == 0)
          ostream->write("z", sizeof(uint8_t), 1);
        else{
          valueToWord85(value, word85);
          ostream->write(word85, sizeof(uint8_t), word85Size);
        }
      }

      if (tail)
        memcpy(&buff[0], &buff[read - tail], tail * sizeof(unsigned char));
    }

    if (tail){
      memset(&buff[tail], 0, 3);
      value = asciiWordToValue(&buff[0]);
      if (value){
        valueToWord85(value, word85);
        ostream->write(word85, sizeof(uint8_t), tail + 1);
      }
    }

    ostream->write("~>", sizeof(uint8_t), 2);

    return true;
  }

  bool FilterAscii85::decode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream){
    if (!istream || !ostream)
      return false;

    uint32_t const buffSize = 4096;
    unsigned char buff[buffSize] = {0};

    std::size_t read = 0;
    std::size_t words = 0;
    std::size_t tail = 0;
    uint32_t value = 0;

    std::size_t const word85Size = 5;
    unsigned char word85[word85Size] = {0};
    std::size_t word85Len = 0;

    std::size_t const wordAsciiSize = 4;
    unsigned char wordAscii[wordAsciiSize] = {0};

    unsigned char ch = 0;

    bool eos = false;

    while (read = (std::size_t) istream->read(&buff[0], sizeof(uint8_t), buffSize)){
      for (std::size_t i = 0 ; i < read; ++i){
        ch = buff[i];
        if (ch >= '!' && ch <= 'u'){
          if (eos){
            if (ch != '>' || word85Len == 1)
              return false;

            if (word85Len > 1){
              memset(&word85[word85Len], 'u', (word85Size - word85Len) * sizeof(unsigned char));
              value = word85ToValue(word85);
              valueToAsciiWord(value, wordAscii);
              ostream->write(wordAscii, sizeof(uint8_t), word85Len - 1);
            }

            return true;
          }

          word85[word85Len++] = ch;
          if (word85Len == word85Size){
            value = word85ToValue(word85);
            valueToAsciiWord(value, wordAscii);
            ostream->write(wordAscii, sizeof(uint8_t), wordAsciiSize);
            word85Len = 0;
          }
        }
        else if (ch == 'z' && !eos){
          if (word85Len)
            return false;
          memset(wordAscii, 0, wordAsciiSize * sizeof(unsigned char));
          ostream->write(wordAscii, sizeof(uint8_t), wordAsciiSize);
        }
        else if (ch == '~' && !eos){
          if (word85Len == 1)
            return false;

          eos = true;
        }
        else if (!eos && (ch == NUL || ch == TAB || ch == LF || ch == CR || ch == FF || ch == SPACE))
          continue;
        else
          return false;
      }
    }

    return false;
  }

  FilterAscii85 *FilterAscii85::clone(void) const{
    return new FilterAscii85();
  }
}}
