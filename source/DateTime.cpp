#include <DateTime.h>

#include <time.h>
#include <stdio.h>

namespace pdfout{

  DateTime::DateTime(TimeZone timezone){
    time_t now = time(0);
    struct tm *timeinfo = gmtime(&now);

    mYear = 1900 + timeinfo->tm_year;
    mMonth = 1 + timeinfo->tm_mon;
    mDay = timeinfo->tm_mday;
    mHour = timeinfo->tm_hour;
    mMinute = timeinfo->tm_min;
    mSecond = timeinfo->tm_sec;
    mTimeZone = timezone;
  }

  DateTime::DateTime(uint64_t seconds, TimeZone timezone){
    struct tm *timeinfo = gmtime((time_t *) &seconds);

    mYear = 1900 + timeinfo->tm_year;
    mMonth = 1 + timeinfo->tm_mon;
    mDay = timeinfo->tm_mday;
    mHour = timeinfo->tm_hour;
    mMinute = timeinfo->tm_min;
    mSecond = timeinfo->tm_sec;
    mTimeZone = timezone;
  }

  DateTime::~DateTime(){
  }

}
