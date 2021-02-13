#pragma once

#include <deque>
#include <array>
#include <memory>
#include <algorithm>

#include "InputStream.h"
#include "OutputStream.h"

#undef min
#undef max

namespace pdfout{

  template<std::size_t ChunkSize = 4096>
  class MemoryIOStream : public InputStream, public OutputStream{
  public:
    MemoryIOStream(void)
      : InputStream(),
      OutputStream(),
      mReadPos(0),
      mWritePos(0),
      mData(),
      mSize(0){
    }

    ~MemoryIOStream(void) {}

    /**
     * Gets the stream size
     * @return Size of the stream in bytes
     */
    uint64_t getSize(void) const {return mSize;}

    /**
     * Gets chunk size
     * @return Stream chunk size in bytes
     */
    std::size_t getChunkSize(void) const {return ChunkSize;}

    uint64_t tellg(void) const {return mReadPos;}

    bool seekg(uint64_t pos){
      if (pos >= mSize)
        return false;

      mReadPos = pos;
      return true;
    }

    uint64_t read(void *data, uint32_t sizeOfElements, uint64_t numOfElements) const{
      if (mReadPos >= mSize)
        return 0;

      uint64_t dataSize = sizeOfElements * numOfElements;
      auto streamRemain = mSize - mReadPos;
      uint64_t read = 0;

      while (read < dataSize){
        if (!streamRemain)
          return read;

        std::size_t currentChunk = (std::size_t) (mReadPos / ChunkSize);
        uint64_t chunkRemain = ChunkSize - mReadPos % ChunkSize;

        std::size_t readBytes = (std::size_t) std::min(dataSize - read, std::min(streamRemain, chunkRemain));
        memcpy(&((unsigned char *)(data))[read], &(mData[currentChunk]->data())[mReadPos % ChunkSize], readBytes);
        mReadPos += readBytes;
        streamRemain -= readBytes;
        read += readBytes;
      }

      return read;
    }

    uint64_t tellp(void) const {return mWritePos;}

    bool seekp(uint64_t pos){
      uint64_t streamSize = mData.size() * ChunkSize;
      if (pos < streamSize){
        mWritePos = pos;
        return true;
      }

      uint64_t diff = pos - streamSize;
      uint64_t additionalChunks = diff / ChunkSize + ((diff % ChunkSize == 0) ? 0 : 1);

      try{
        for (auto i = 0; i < additionalChunks; ++i)
          mData.push_back(std::make_unique<std::array<unsigned char, ChunkSize>>());
      }
      catch (std::bad_alloc &){
        return false;
      }

      mWritePos = pos;
      return true;
    }

    uint64_t write(void const *data, uint32_t sizeOfElements, uint64_t numOfElements){
      uint64_t streamSize = mData.size() * ChunkSize;
      auto streamRemain = streamSize - mWritePos;
      uint64_t dataSize = sizeOfElements * numOfElements;
      uint64_t written = 0;

      while (written < dataSize){
        if (!streamRemain){
          try{
            mData.push_back(std::make_unique<std::array<unsigned char, ChunkSize>>());
          }
          catch (std::bad_alloc &){
            return written;
          }

          streamRemain += ChunkSize;
        }

        std::size_t currentChunk = (std::size_t) (mWritePos / ChunkSize);

        std::size_t writeBytes = (std::size_t) std::min(streamRemain, dataSize - written);
        memcpy(&(mData[currentChunk]->data())[mWritePos % ChunkSize], &((unsigned char const *)(data))[written], writeBytes);
        mWritePos += writeBytes;
        mSize = std::max(mSize, mWritePos);
        streamRemain -= writeBytes;
        written += writeBytes;
      }

      return written;
    }

    void flush(void) {}

  private:
    MemoryIOStream(MemoryIOStream const &);
    MemoryIOStream &operator=(MemoryIOStream const &);

  private:
    mutable uint64_t mReadPos;
    uint64_t mWritePos;
    std::deque<std::unique_ptr<std::array<unsigned char, ChunkSize>>> mData;
    uint64_t mSize;
  };
}
