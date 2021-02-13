#pragma once

#include <string>

#include <object/Object.h>

namespace kernel{

  class ObjectBoolean : public Object{
  public:
    explicit ObjectBoolean(bool value = true) : Object(ObjectTypeBoolean, IndirectTypeNonIndirectable), mValue(value) {}

    ObjectBoolean &operator=(bool value){
      mValue = value;
      return *this;
    }

    ~ObjectBoolean(void) {}

    bool getValue(void) const {return mValue;}

    void setValue(bool value) {mValue = value;}

    operator bool(void) const {return mValue;}

    bool operator==(ObjectBoolean const &obj) const {return mValue == obj.mValue;}

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
      if (mValue)
        stream->write("true", sizeof(uint8_t), 4);
      else
        stream->write("false", sizeof(uint8_t), 5);
    }

    ObjectBoolean *clone(void) const{
      return new ObjectBoolean(*this);
    }

  protected:
    ObjectBoolean(ObjectBoolean const &obj) : Object(obj), mValue(obj.mValue) {}

  private:
    ObjectBoolean &operator=(ObjectBoolean const &) = delete;

  private:
    bool mValue;
  };
}
