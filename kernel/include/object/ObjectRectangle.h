#pragma once

#include <string>
#include <memory>

#include <Types.h>
#include <object/Object.h>

namespace kernel{

  using namespace pdfout;

  class ObjectRectangle : public Object{
  public:
    ObjectRectangle(float lowerLeftX, float lowerLeftY, float upperRightX, float upperRightY)
      : Object(ObjectTypeRectangle, IndirectTypeNonIndirectable),
      mValue(){

      setValue(lowerLeftX, lowerLeftY, upperRightX, upperRightY);
    }

    ObjectRectangle(struct pdfout::Rectangle const &value)
      : Object(ObjectTypeRectangle, IndirectTypeNonIndirectable),
      mValue(value){
    }

    struct pdfout::Rectangle getValue(void) const{
      return mValue;
    }

    void setValue(struct pdfout::Rectangle const &value){
      mValue = value;
    }

    void setValue(float lowerLeftX, float lowerLeftY, float upperRightX, float upperRightY){
      mValue.mLowerLeftX = lowerLeftX;
      mValue.mLowerLeftY = lowerLeftY;
      mValue.mUpperRightX = upperRightX;
      mValue.mUpperRightY = upperRightY;
    }

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
      char dest[MAX_FLOAT_LENGTH * 4 + 10] = {0};
      floatToString(&dest[strlen(dest)], mValue.mLowerLeftX);
      floatToString(&dest[strlen(dest)], mValue.mLowerLeftY);
      floatToString(&dest[strlen(dest)], mValue.mUpperRightX);
      floatToString(&dest[strlen(dest)], mValue.mUpperRightY);

      stream->write("[", sizeof(uint8_t), 1);
      stream->write(dest, sizeof(uint8_t), strlen(dest));
      stream->write("]", sizeof(uint8_t), 1);
    }

    ObjectRectangle *clone(void) const{
      return new ObjectRectangle(*this);
    }

  protected:
    ObjectRectangle(ObjectRectangle const &obj)
      :Object(obj),
      mValue(obj.mValue){
    }

  private:
    ObjectRectangle &operator=(ObjectRectangle const &) = delete;

  private:
    struct pdfout::Rectangle mValue;
  };
}
