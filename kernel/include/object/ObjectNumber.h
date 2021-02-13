#pragma once

#include <string>

#include <object/Object.h>
#include <InternalTools.h>

namespace kernel{

  class ObjectNumber : public Object{
  public:
    explicit ObjectNumber(float value = 0.f) : Object(ObjectTypeNumber, IndirectTypeNonIndirectable), mValue(value) {}

    ObjectNumber &operator=(float value){
      mValue = value;
      return *this;
    }

    ~ObjectNumber(void) {}

    float getValue(void) const {return mValue;}

    void setValue(float value) {mValue = value;}

    operator float(void) const {return mValue;}

    bool operator==(ObjectNumber const &rhs) const {return mValue == rhs.mValue;}

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
      char buff[MAX_FLOAT_LENGTH + 1] = {0};
      floatToString(buff, mValue);
      stream->write(buff, sizeof(uint8_t), strlen(buff));
    }

    ObjectNumber *clone(void) const {return new ObjectNumber(*this);}

  protected:
    ObjectNumber(ObjectNumber const &obj) : Object(obj), mValue(obj.mValue) {}

  private:
    ObjectNumber &operator=(ObjectNumber const &) = delete;

  private:
    float mValue;
  };
}
