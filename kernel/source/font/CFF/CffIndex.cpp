#include <font/CFF/CffIndex.h>

#include <InternalTools.h>
#include <stream/InputStream.h>
#include <font/CFF/StandardSID.h>

namespace kernel{ namespace font{ namespace cff{

  Index::Index(pdfout::InputStream *stream)
    : mData(){

    load(stream);
  }

  Index::~Index(){}

  std::size_t Index::getSize() const noexcept{
    return mData.size();
  }

  std::string Index::getString(uint16_t sid) const noexcept{
    static uint16_t const standardSIDCount = getStandardSIDCount();

    if (sid < standardSIDCount)
      return getStandardSID(sid);

    uint16_t const index = sid - standardSIDCount;
    auto const &data = mData[index];
    return std::string(std::begin(data), std::end(data));
  }

  std::vector<uint8_t> const &Index::operator[](std::size_t index) const noexcept{
    return mData[index];
  }

  void Index::load(pdfout::InputStream *stream){
    Endian const endian = getEndian();

    uint16_t count = 0;
    stream->read(&count, sizeof(uint16_t), 1);
    if (endian == EndianLittle)
      swapByteOrder(count);

    if (!count)
      return;

    uint8_t offSize = 0;
    stream->read(&offSize, sizeof(uint8_t), 1);

    auto const offsets = readOffsets(stream, offSize, count + 1);
    auto const streamPos = stream->tellg();

    for (std::size_t i = 0; i < count; ++i){
      auto length = offsets[i + 1] - offsets[i];
      std::vector<uint8_t> data(length, 0);
      stream->seekg(streamPos + offsets[i] - 1);
      stream->read(data.data(), sizeof(uint8_t), length);
      mData[i] = std::move(data);
    }

    stream->seekg(streamPos + offsets[count] - 1);
  }

  std::vector<uint32_t> Index::readOffsets(pdfout::InputStream *stream, uint32_t sizeOfElements, uint64_t numOfElements) const{
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

}}}
