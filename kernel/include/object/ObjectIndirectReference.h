#pragma once

#include <memory>
#include <string>

#include <object/Object.h>

namespace kernel{

  class ObjectIndirectReference : public Object{
  public:
    ObjectIndirectReference(uint32_t number, uint16_t generation)
    : Object(ObjectTypeIndirectReference, IndirectTypeNonIndirectable),
    mNumber(number),
    mGeneration(generation) {}

    uint32_t getNumber(void) const {return mNumber;}
    uint16_t getGeneration(void) const {return mGeneration;}

    bool operator==(ObjectIndirectReference const &rhs) const {return mNumber == rhs.mNumber && mGeneration == rhs.mGeneration;}

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
      char dest[12 * 2 + 10] = {0};
      sprintf(dest, "%i %i R", mNumber, mGeneration);
      stream->write(dest, sizeof(uint8_t), strlen(dest));
    }

    std::unique_ptr<ObjectIndirectReference> getReference(void) const{
      return std::make_unique<ObjectIndirectReference>(mNumber, mGeneration);
    }

    ObjectIndirectReference *clone(void) const {return new ObjectIndirectReference(*this);}

  protected:
    ObjectIndirectReference(ObjectIndirectReference const &obj)
      : Object(obj),
      mNumber(obj.mNumber),
      mGeneration(obj.mGeneration){
    }

  private:
    ObjectIndirectReference &operator=(ObjectIndirectReference const &) = delete;

  private:
    uint32_t mNumber;
    uint16_t mGeneration;
  };
}
