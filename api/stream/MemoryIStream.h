#pragma once

#include "InputStream.h"

namespace pdfout{

  class MemoryIStream : public InputStream{
  public:
    MemoryIStream(void const *data, uint64_t size)
      : InputStream(),
      mReadPos(0),
      mData((uint8_t const *) data),
      mSize(size){
    }

    ~MemoryIStream(void) {}

    uint64_t getSize(void) const {return mSize;}

    uint64_t tellg(void) const {return mReadPos;}

    bool seekg(uint64_t pos){
      if (!mData || pos >= mSize)
        return false;

      mReadPos = pos;
      return true;
    }

    uint64_t read(void *data, uint32_t sizeOfElements, uint64_t numOfElements) const{
      if (!mData || mReadPos >= mSize || !sizeOfElements || !numOfElements)
        return 0;

      auto read = sizeOfElements * numOfElements <  mSize - mReadPos ? sizeOfElements * numOfElements : mSize - mReadPos;
      memcpy((uint8_t *)(data), &mData[mReadPos], read);
      mReadPos += read;
      return read;
    }

  private:
    MemoryIStream(MemoryIStream const &);
    MemoryIStream &operator=(MemoryIStream const &);

  private:
    mutable uint64_t mReadPos;
    uint8_t const *mData;
    uint64_t mSize;
  };

}
