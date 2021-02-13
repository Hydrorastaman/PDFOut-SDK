#include <font/TrueType/TableCff.h>

#include <memory>
#include <Exception.h>
#include <stream/MemoryIStream.h>
#include <stream/MemoryOStream.h>

#include <font/CFF/FontCFF.h>

namespace kernel{ namespace font{ namespace truetype{
  TableCff::TableCff(void)
    : Table(){
  }

  TableCff::~TableCff(void) {
  }

  void TableCff::load(pdfout::InputStream *stream) {
    Endian const endian = getEndian();

    std::vector<uint8_t> source = {
      0x01, 0x00, 0x04, 0x01, 0x00, 0x01, 0x01, 0x01, 0x13, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x2b,
      0x54, 0x69, 0x6d, 0x65, 0x73, 0x2d, 0x52, 0x6f, 0x6d, 0x61, 0x6e, 0x00, 0x01, 0x01, 0x01, 0x1f, 
      0xf8, 0x1b, 0x00, 0xf8, 0x1c, 0x02, 0xf8, 0x1d, 0x03, 0xf8, 0x19, 0x04, 0x1c, 0x6f, 0x00, 0x0d,
      0xfb, 0x3c, 0xfb, 0x6e, 0xfa, 0x7c, 0xfa, 0x16, 0x05, 0xe9, 0x11, 0xb8, 0xf1, 0x12, 0x00, 0x03,
      0x01, 0x01, 0x08, 0x13, 0x18, 0x30, 0x30, 0x31, 0x2e, 0x30, 0x30, 0x37, 0x54, 0x69, 0x6d, 0x65,
      0x73, 0x20, 0x52, 0x6f, 0x6d, 0x61, 0x6e, 0x54, 0x69, 0x6d, 0x65, 0x73, 0x00, 0x00, 0x00, 0x02,
      0x01, 0x01, 0x02, 0x03, 0x0e, 0x0e, 0x7d, 0x99, 0xf9, 0x2a, 0x99, 0xfb, 0x76, 0x95, 0xf7, 0x73,
      0x8b, 0x06, 0xf7, 0x9a, 0x93, 0xfc, 0x7c, 0x8c, 0x07, 0x7d, 0x99, 0xf8, 0x56, 0x95, 0xf7, 0x5e,
      0x99, 0x08, 0xfb, 0x6e, 0x8c, 0xf8, 0x73, 0x93, 0xf7, 0x10, 0x8b, 0x09, 0xa7, 0x0a, 0xdf, 0x0b,
      0xf7, 0x8e, 0x14
    };

    pdfout::MemoryIStream istream(source.data(), source.size());
    stream = (pdfout::InputStream *) &istream;

    std::unique_ptr<cff::FontCFF> cff = std::make_unique<cff::FontCFF>(stream);
  }

  void TableCff::save(pdfout::OutputStream *stream) const {
    Endian const endian = getEndian();

    //stream->write(&data, sizeof(TableCffData), 1);
  }

  TableCff::Index TableCff::readIndex(pdfout::InputStream *stream) const{
    Endian const endian = getEndian();
    Index index;

    uint16_t count = 0;
    stream->read(&count, sizeof(uint16_t), 1);
    if (endian == EndianLittle)
      swapByteOrder(count);

    if (!count)
      return index;

    uint8_t offSize = 0;
    stream->read(&offSize, sizeof(uint8_t), 1);

    auto const offsets = readOffsets(stream, offSize, count + 1);
    auto const streamPos = stream->tellg();

    for (std::size_t i = 0; i < count; ++i){
      auto length = offsets[i + 1] - offsets[i];
      std::vector<uint8_t> data(length, 0);
      stream->seekg(streamPos + offsets[i] - 1);
      stream->read(data.data(), sizeof(uint8_t), length);
      index[i] = std::move(data);
    }

    stream->seekg(streamPos + offsets[count] - 1);

    return index;
  }

  std::vector<uint32_t> TableCff::readOffsets(pdfout::InputStream *stream, uint32_t sizeOfElements, uint64_t numOfElements) const{
    Endian const endian = getEndian();

    std::vector<uint8_t> offsetsRaw(sizeOfElements * numOfElements, 0);
    stream->read(offsetsRaw.data(), sizeof(uint8_t), offsetsRaw.size());
    void *data = offsetsRaw.data();

    std::vector<uint32_t> offsets(numOfElements, 0);

    switch (sizeOfElements){
      case 1:{
        uint8_t *ptr = (uint8_t *) data;
        for (std::size_t i = 0; i < numOfElements; ++i)
          offsets[i] = (uint32_t) ptr[i];

        break;
      }

      case 2:{
        uint16_t *ptr = (uint16_t *) data;
        if (endian == EndianLittle)
          swapByteOrder(ptr, numOfElements);

        for (std::size_t i = 0; i < numOfElements; ++i)
          offsets[i] = (uint32_t) ptr[i];

        break;
      }

      case 3:{
        uint8_t *ptr = (uint8_t *) data;
        if (endian == EndianLittle){
          for (std::size_t i = 0; i < numOfElements; ++i)
            offsets[i] = (uint32_t) ptr[i * 3 + 2] << 16 | (uint32_t) ptr[i * 3 + 1] << 8 | (uint32_t) ptr[i * 3];
        }
        else{
          for (std::size_t i = 0; i < numOfElements; ++i)
            offsets[i] = (uint32_t) ptr[i * 3] << 16 | (uint32_t) ptr[i * 3 + 1] << 8 | (uint32_t) ptr[i * 3 + 2];
        }

        break;
      }

      case 4:{
        uint32_t *ptr = (uint32_t *) data;
        if (endian == EndianLittle)
          swapByteOrder(ptr, numOfElements);

        memcpy(offsets.data(), ptr, numOfElements * sizeOfElements);
        break;
      }
    }

    return offsets;
  }

  std::string TableCff::unpack(uint8_t const *source, std::size_t length) const{
    std::string out = "";

    uint8_t const *ptr = source;
    while (ptr < source + length){
      uint8_t const b0 = ptr[0];

      if (b0 >= 0 && b0 <= 21){
switch (b0){
  case 0: out += "version "; ++ptr; break;
  case 1: out += "Notice "; ++ptr; break;
  default: ++ptr; break;
}
        // read operator
      }
      else if (b0 == 28 || b0 == 29 || b0 >= 32 && b0 <= 254){
        int32_t operand = 0;
        ptr += readIntegerOperand(ptr, operand);
        out += std::to_string(operand) + " ";
      }
      else if (b0 == 30){
        float operand = 0.f;
        ptr += readRealOperand(ptr, operand);
        out += std::to_string(operand) + " ";
      }
      else{
        RUNTIME_EXCEPTION("Invalid data format");
      }
    }

    return out;
  }

  std::size_t TableCff::readIntegerOperand(uint8_t const *source, int32_t &value) const{
    int32_t const b0 = (int32_t) source[0];

    if (b0 >= 32 && b0 <= 246){
      value = b0 - 139;
      return 1;
    }
    else if (b0 >= 247 && b0 <= 250){
      int32_t const b1 = (int32_t) source[1];

      value = (b0 - 247) * 256 + b1 + 108;
      return 2;
    }
    else if (b0 >= 251 && b0 <= 254){
      int32_t const b1 = (int32_t) source[1];
      int32_t const b2 = (int32_t) source[2];

      value = -(b0 - 251) * 256 - b1 - 108;
      return 3;
    }
    else if (b0 == 28){
      int32_t const b1 = (int32_t) source[1];
      int32_t const b2 = (int32_t) source[2];

      value = b1 << 8 | b2;
      return 3;
    }
    else if (b0 == 29){
      int32_t const b1 = (int32_t) source[1];
      int32_t const b2 = (int32_t) source[2];
      int32_t const b3 = (int32_t) source[3];
      int32_t const b4 = (int32_t) source[4];

      value = b1 << 24 | b2 << 16 | b3 << 8 | b4;
      return 5;
    }

    return 0;
  }

  std::size_t TableCff::readRealOperand(uint8_t const *source, float &value) const{
    uint8_t const b0 = source[0];
    if (b0 != 30)
      return 0;

    auto nibbleToString = [](uint8_t nibble)->std::string{
      switch (nibble){
        case 0x0: return "0";
        case 0x1: return "1";
        case 0x2: return "2";
        case 0x3: return "3";
        case 0x4: return "4";
        case 0x5: return "5";
        case 0x6: return "6";
        case 0x7: return "7";
        case 0x8: return "8";
        case 0x9: return "9";
        case 0xA: return ".";
        case 0xB: return "E";
        case 0xC: return "E-";
        case 0xD: return "";
        case 0xE: return "-";
        case 0xF: return "";
        default: return "";
      }
    };

    std::size_t offset = 1;
    std::string str = "";
    while (true){
      uint8_t const n0 = source[offset] >> 4;
      uint8_t const n1 = source[offset] & 0x0F;

      str += nibbleToString(n0) + nibbleToString(n1);
      if (n0 == 0xF || n1 == 0xF)
        break;

      ++offset;
    }

    char *end = nullptr;
    value = strtof(&str[0], &end);
    return offset + 1;
  }

}}}
