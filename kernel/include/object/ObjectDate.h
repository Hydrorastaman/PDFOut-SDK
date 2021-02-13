#pragma once

#include <DateTime.h>
#include <object/Object.h>

namespace kernel{

  class ObjectDate : public Object{
  public:
    explicit ObjectDate(pdfout::DateTime const &dateTime)
      : Object(ObjectTypeDate, IndirectTypeNonIndirectable),
      mDateTime(dateTime) {
    }

    ~ObjectDate(void) {}

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
      pdfout::TimeZone const tz = mDateTime.mTimeZone;

      // D:YYYYMMDDHHmmSSOHH'mm'
      char buffer[26] = {0};
      sprintf(buffer, "(D:%04i%02i%02i%02i%02i%02i%s%02i'%02i')",
        mDateTime.mYear, mDateTime.mMonth, mDateTime.mDay,
        mDateTime.mHour, mDateTime.mMinute, mDateTime.mSecond,
        (tz == 0) ? "Z" : (tz > 0) ? "+" : "-",
        (tz >= 0) ? tz / 100 : -tz / 100,
        (tz >= 0) ? tz % 100 : -tz % 100);

      stream->write(buffer, sizeof(char), 26);
    }

    ObjectDate *clone(void) const {return new ObjectDate(*this);}

  protected:
    ObjectDate(ObjectDate const &obj) : Object(obj), mDateTime(obj.mDateTime) {}

  private:
    ObjectDate &operator=(ObjectDate const &) = delete;

  private:
    pdfout::DateTime mDateTime;
  };
}
