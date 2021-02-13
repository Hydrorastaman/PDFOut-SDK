#pragma once

#include "PdfOutConfig.h"

namespace pdfout{

  /**
   @brief Pre-defined time zones
   */
  enum TimeZone{
    TimeZoneUTC1200M = -1200, /**< UTC-12:00 */
    TimeZoneUTC1100M = -1100, /**< UTC-11:00 */
    TimeZoneUTC1000M = -1000, /**< UTC-10:00 */
    TimeZoneUTC0930M = -930,  /**< UTC-09:30 */
    TimeZoneUTC0900M = -900,  /**< UTC-09:00 */
    TimeZoneUTC0800M = -800,  /**< UTC-08:00 */
    TimeZoneUTC0700M = -700,  /**< UTC-07:00 */
    TimeZoneUTC0600M = -600,  /**< UTC-06:00 */
    TimeZoneUTC0500M = -500,  /**< UTC-05:00 */
    TimeZoneUTC0400M = -400,  /**< UTC-04:00 */
    TimeZoneUTC0330M = -330,  /**< UTC-03:30 */
    TimeZoneUTC0300M = -300,  /**< UTC-03:00 */
    TimeZoneUTC0200M = -200,  /**< UTC-02:00 */
    TimeZoneUTC0100M = -100,  /**< UTC-01:00 */
    TimeZoneUTC0000Z = 0,     /**< UTC+00:00 */
    TimeZoneUTC0100P = 100,   /**< UTC+01:00 */
    TimeZoneUTC0200P = 200,   /**< UTC+02:00 */
    TimeZoneUTC0300P = 300,   /**< UTC+03:00 */
    TimeZoneUTC0330P = 330,   /**< UTC+03:30 */
    TimeZoneUTC0400P = 400,   /**< UTC+04:00 */
    TimeZoneUTC0430P = 430,   /**< UTC+04:30 */
    TimeZoneUTC0500P = 500,   /**< UTC+05:00 */
    TimeZoneUTC0530P = 530,   /**< UTC+05:30 */
    TimeZoneUTC0545P = 545,   /**< UTC+05:45 */
    TimeZoneUTC0600P = 600,   /**< UTC+06:00 */
    TimeZoneUTC0630P = 630,   /**< UTC+06:30 */
    TimeZoneUTC0700P = 700,   /**< UTC+07:00 */
    TimeZoneUTC0800P = 800,   /**< UTC+08:00 */
    TimeZoneUTC0845P = 845,   /**< UTC+08:45 */
    TimeZoneUTC0900P = 900,   /**< UTC+09:00 */
    TimeZoneUTC0930P = 930,   /**< UTC+09:30 */
    TimeZoneUTC1000P = 1000,  /**< UTC+10:00 */
    TimeZoneUTC1030P = 1030,  /**< UTC+10:30 */
    TimeZoneUTC1100P = 1100,  /**< UTC+11:00 */
    TimeZoneUTC1200P = 1200,  /**< UTC+12:00 */
    TimeZoneUTC1245P = 1245,  /**< UTC+12:45 */
    TimeZoneUTC1300P = 1300,  /**< UTC+13:00 */
    TimeZoneUTC1400P = 1400,  /**< UTC+14:00 */
  };

  struct PDFOUT_EXPORTS DateTime{
    uint16_t mYear;     /**< years */
    uint16_t mMonth;    /**< months [01-12] */
    uint16_t mDay;      /**< days [01-31] */
    uint16_t mHour;     /**< hours [00-23] */
    uint16_t mMinute;   /**< minutes [00-59] */
    uint16_t mSecond;   /**< seconds [00-59] */
    TimeZone mTimeZone; /**< timezone */

    /**
     Creates DateTime object with current time.

     @param timezone Pre-defined time zone;

     @code {.cpp}
       DateTime const now = DateTime(TimeZoneUTC0300P);
     @endcode
     */
    explicit DateTime(TimeZone timezone = TimeZoneUTC0000Z);

    /**
     Creates DateTime object from specified time point

     @param seconds Seconds since 1 Jan 1970;
     @param timezone Pre-defined time zone;

     @code {.cpp}
       DateTime const dateTime = DateTime(1563457170, TimeZoneUTC0300P);
     @endcode
     */
    explicit DateTime(uint64_t seconds, TimeZone timezone = TimeZoneUTC0000Z);
    ~DateTime();
  };

}
