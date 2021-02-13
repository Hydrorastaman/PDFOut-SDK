#include <font/TrueType/TableDttf.h>

#include <memory>

namespace kernel{ namespace font{ namespace truetype{
  TableDttf::TableDttf(void)
    : Table(),
    mData(),
    mGlyphIndexArray(){
    memset(&mData, 0, sizeof(TableDttfData));
  }

  TableDttf::~TableDttf(void) {
  }

  void TableDttf::load(pdfout::InputStream *stream) {
    Endian const endian = getEndian();

    stream->read(&mData, sizeof(TableDttfData), 1);
    if (endian == EndianLittle) {
      swapByteOrder(mData.mVersion);
      swapByteOrder(mData.mCheckSum);
      swapByteOrder(mData.mOriginalNumGlyphs);
      swapByteOrder(mData.mMaxGlyphIndexUsed);
      swapByteOrder(mData.mFormat);
      swapByteOrder(mData.mFFlags);
      swapByteOrder(mData.mGlyphCount);
    }

    mGlyphIndexArray.resize(mData.mGlyphCount);
    stream->read(mGlyphIndexArray.data(), sizeof(uint16_t), mData.mGlyphCount);
    if (endian == EndianLittle)
      for (auto &elem : mGlyphIndexArray)
        swapByteOrder(elem);
  }

  void TableDttf::save(pdfout::OutputStream *stream) const {
    Endian const endian = getEndian();

    TableDttfData data;
    memcpy(&data, &mData, sizeof(TableDttfData));

    if (getEndian() == EndianLittle) {
      swapByteOrder(data.mVersion);
      swapByteOrder(data.mCheckSum);
      swapByteOrder(data.mOriginalNumGlyphs);
      swapByteOrder(data.mMaxGlyphIndexUsed);
      swapByteOrder(data.mFormat);
      swapByteOrder(data.mFFlags);
      swapByteOrder(data.mGlyphCount);
    }

    stream->write(&data, sizeof(TableDttfData), 1);

    uint16_t value = 0;
    for (auto const &elem : mGlyphIndexArray){
      value = elem;
      swapByteOrder(value);
      stream->write(&value, sizeof(uint16_t), 1);
    }
  }
}}}
