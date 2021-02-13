#pragma once

#include <map>
#include <vector>

namespace pdfout{
  class InputStream;
}

namespace kernel{ namespace font{ namespace cff{

  class Index{
  public:
    Index(pdfout::InputStream *stream);
    ~Index();
    Index(Index const &) = delete;
    Index &operator=(Index const &) = delete;

    std::size_t getSize() const noexcept;
    std::string getString(uint16_t sid) const noexcept;
    std::vector<uint8_t> const &operator[](std::size_t index) const noexcept;

  private:
    void load(pdfout::InputStream *stream);
    std::vector<uint32_t> readOffsets(pdfout::InputStream *stream, uint32_t sizeOfElements, uint64_t numOfElements) const;

  private:
    mutable std::map<uint32_t, std::vector<uint8_t>> mData;
  };

}}}
