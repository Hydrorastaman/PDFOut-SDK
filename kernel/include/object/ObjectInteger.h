#pragma once

#include <string>
#include <cstddef>

#include <object/Object.h>

namespace kernel{

  class ObjectInteger : public Object{
  public:
    ObjectInteger(int32_t value = 0)
      : Object(ObjectTypeInteger, IndirectTypeNonIndirectable),
      mValue(value){
    }

    ObjectInteger &operator=(int value){
      mValue = value;
      return *this;
    }

    ~ObjectInteger(void) {}

    int32_t getValue(void) const {return mValue;}

    void setValue(int32_t value) {mValue = value;}

    operator int32_t(void) const {return mValue;}

    bool operator==(ObjectInteger const &rhs) const {return mValue == rhs.mValue;}

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
      char dest[13] = {0};
      sprintf(dest, "%i", mValue);
      stream->write(dest, sizeof(uint8_t), strlen(dest));
    }

    ObjectInteger *clone(void) const {return new ObjectInteger(*this);}

  protected:
    ObjectInteger(ObjectInteger const &obj) : Object(obj), mValue(obj.mValue) {}

  private:
    ObjectInteger &operator=(ObjectInteger const &) = delete;

  private:
    int32_t mValue;
  };
}
