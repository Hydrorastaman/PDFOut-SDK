#pragma once

#include <vector>
#include <cstdint>

#include <font/TrueType/Table.h>
#include <font/TrueType/Glyph.h>

namespace pdfout{

  class InputStream;
  class OutputStream;

}

namespace kernel{ namespace font{ namespace truetype{

  class TableDttf;

  /**
   * @brief Stores glyph index and it offset in the font file
   */
  class TableLoca : public Table{
  public:
    TableLoca(uint16_t numGlyphs, int16_t indexToLocFormat);
    ~TableLoca(void);

    void load(pdfout::InputStream *stream);
    void save(pdfout::OutputStream *stream) const;

    void unpack(TableDttf *dttf);

    std::unique_ptr<TableLoca> createSubset(std::vector<uint16_t> const &gids) const;
    std::unique_ptr<TableLoca> createSuperset(TableLoca const *loca) const;

    std::vector<uint32_t> getGidOffset(std::vector<uint16_t> const &gids) const;
    uint32_t getGidOffset(uint16_t gid) const;

  private:
    //TableLoca(TableLoca const &) = delete;
    TableLoca &operator=(TableLoca const &) = delete;

  private:
    template<typename T>
    void loadOffsets(pdfout::InputStream *stream){
      auto const count = mNumGlyphs + 1;
      std::vector<T> offsets(count);
      stream->read(offsets.data(), sizeof(T), count);

      Endian const endian = getEndian();
      if (endian == EndianLittle){
        T *ptr = (T *) offsets.data();
        for (std::size_t i = 0; i < count; ++i)
          swapByteOrder(ptr[i]);
      }

      uint32_t const mult = mIndexToLocFormat == 0 ? 2 : 1;

      for (std::size_t i = 0; i < count; ++i)
        mOffsets[i] = offsets[i] * mult;
    }

    template<typename T>
    void saveOffsets(pdfout::OutputStream *stream) const{
      auto const count = mNumGlyphs + 1;
      uint32_t const div = mIndexToLocFormat == 0 ? 2 : 1;

      std::vector<T> data(count);
      for (std::size_t i = 0; i < count; ++i)
        data[i] = (T) (mOffsets[i] / div);

      Endian const endian = getEndian();
      if (endian == EndianLittle){
        for (auto &elem : data)
          swapByteOrder(elem);
      }

      stream->write(data.data(), sizeof(T), data.size());
    }

  public:
    uint16_t mNumGlyphs;
    int16_t mIndexToLocFormat;
    std::vector<uint32_t> mOffsets;
  };

}}}
