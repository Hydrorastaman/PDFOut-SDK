#pragma once

#include <memory>
#include <vector>
#include <algorithm>

#include "OutputStream.h"
#include "../PdfOutConfig.h"

#undef min
#undef max

namespace pdfout{

  class PDFOUT_EXPORTS MemoryOStream : public OutputStream{
  public:
    MemoryOStream(void)
      : OutputStream(),
      mWritePos(0),
      mData(nullptr),
      mDataSize(0),
      mSize(0){

      resize(4096);

    }

    ~MemoryOStream(void){
      delete[] mData;
      mData = nullptr;
      mDataSize = 0;
    }

    /**
     * Gets the stream size
     * @return Size of the stream in bytes
     */
    uint64_t getSize(void) const {return mSize;}

    uint64_t tellp(void) const {return mWritePos;}

    bool seekp(uint64_t pos){
      if (pos < mDataSize){
        mWritePos = pos;
        return true;
      }

      try{
        std::size_t size = mDataSize * 2;
        while (size < pos)
          size *= 2;

        resize(size);
      }
      catch (std::bad_alloc &){
        return false;
      }

      mWritePos = pos;
      return true;
    }

    uint64_t write(void const *data, uint32_t sizeOfElements, uint64_t numOfElements){
      uint64_t const streamRemain = mDataSize - mWritePos;
      uint64_t const dataSize = sizeOfElements * numOfElements;

      if (!dataSize)
        return 0;

      if (streamRemain <= dataSize){
        try{
          std::size_t size = mDataSize * 2;
          while (size < mDataSize + dataSize)
            size *= 2;

          resize(size);
        }
        catch (std::bad_alloc &){
          return 0;
        }
      }

      memcpy(&mData[mWritePos], data, dataSize);
      mWritePos += dataSize;
      mSize = (std::max)(mSize, mWritePos);
      return dataSize;
    }

    void flush(void) {}

    void *getData(void) const{
      return (void *) mData;
    }

  private:
    void resize(std::size_t size){
      uint8_t *newData = new uint8_t[size];
      memset(newData, 0, size);
      memcpy(newData, mData, mDataSize);

      delete[] mData;
      mData = newData;
      mDataSize = size;
    }

  private:
    MemoryOStream(MemoryOStream const &);
    MemoryOStream &operator=(MemoryOStream const &);

  private:
    uint64_t mWritePos;
    uint8_t *mData;
    std::size_t mDataSize;
    uint64_t mSize;
  };

}
