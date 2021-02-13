#pragma once

#include <map>
#include <vector>
#include <cstdint>

#include <InternalTools.h>
#include <font/TrueType/Table.h>

namespace pdfout{

  class InputStream;
  class OutputStream;

}

namespace kernel{ namespace font { namespace truetype {

  /**
  * @brief CFF Header.
  */
  PACK(struct CffHeader{
    uint8_t mVersionMajor; /**< Format major version */
    uint8_t mVersionMinor; /**< Format minor version */
    uint8_t mHeaderSize;   /**< Header size in bytes */
    uint8_t mOffSize;      /**< Absolute offset size */
  });

  /**
   * @brief CFF Index Data header
   */
  PACK(struct CffIndex{
    uint16_t mCount ; /**< Number of objects stored in INDEX */
    uint8_t mOffSize; /**< Offset array element size */
    // offset array
    // object data
  });

  class TableCff : public Table {
  private:
    using Index = std::map<uint32_t, std::vector<uint8_t>>;

    enum class Type{
      Integer,
      Real,
      Boolean,
      SID,
      Array,
      Delta,
    };

  public:
    TableCff(void);
    ~TableCff(void);

    void load(pdfout::InputStream *stream);
    void save(pdfout::OutputStream *stream) const;

  private:
    Index readIndex(pdfout::InputStream *stream) const;
    std::vector<uint32_t> readOffsets(pdfout::InputStream *stream, uint32_t sizeOfElements, uint64_t numOfElements) const;

    std::string unpack(uint8_t const *source, std::size_t length) const;
    std::size_t readIntegerOperand(uint8_t const *source, int32_t &value) const;
    std::size_t readRealOperand(uint8_t const *source, float &value) const;

  private:
    TableCff(TableCff const &) = delete;
    TableCff &operator=(TableCff const &) = delete;
  };

}}}
