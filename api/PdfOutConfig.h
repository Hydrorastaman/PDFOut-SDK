#pragma once

#ifdef PDFOUT_API_STATIC
  #define PDFOUT_EXPORTS
#else
  #if (defined WIN32 || defined _WIN32 || defined WINCE || defined __CYGWIN__)
    #if defined PDFOUT_API_EXPORTS
      #define PDFOUT_EXPORTS __declspec(dllexport)
    #else
      #define PDFOUT_EXPORTS __declspec(dllimport)
    #endif
  #elif defined __GNUC__ && __GNUC__ >= 4
    #define PDFOUT_EXPORTS __attribute__ ((visibility ("default")))
  #elif
    #define PDFOUT_EXPORTS
  #endif
#endif

#define PDFOUT_VERSION_MAJOR    0
#define PDFOUT_VERSION_MINOR    6
#define PDFOUT_VERSION_REVISION 12
#define PDFOUT_VERSION_STATUS   ""

#define PDFOUT_STR_EXP(__A)     #__A
#define PDFOUT_STR(__A)         PDFOUT_STR_EXP(__A)

#define PDFOUT_VERSION          PDFOUT_STR(PDFOUT_VERSION_MAJOR) "." PDFOUT_STR(PDFOUT_VERSION_MINOR) "." PDFOUT_STR(PDFOUT_VERSION_REVISION) PDFOUT_VERSION_STATUS


#if defined __GNUC__ || _MSC_VER >= 1600
  #include <cstdint>
#else

  // 7.18.1 Integer types

  // 7.18.1.1 Exact-width integer types
  typedef __int8            int8_t;
  typedef __int16           int16_t;
  typedef __int32           int32_t;
  typedef __int64           int64_t;
  typedef unsigned __int8   uint8_t;
  typedef unsigned __int16  uint16_t;
  typedef unsigned __int32  uint32_t;
  typedef unsigned __int64  uint64_t;

  // 7.18.1.2 Minimum-width integer types
  typedef int8_t    int_least8_t;
  typedef int16_t   int_least16_t;
  typedef int32_t   int_least32_t;
  typedef int64_t   int_least64_t;
  typedef uint8_t   uint_least8_t;
  typedef uint16_t  uint_least16_t;
  typedef uint32_t  uint_least32_t;
  typedef uint64_t  uint_least64_t;

  // 7.18.1.3 Fastest minimum-width integer types
  typedef int8_t    int_fast8_t;
  typedef int16_t   int_fast16_t;
  typedef int32_t   int_fast32_t;
  typedef int64_t   int_fast64_t;
  typedef uint8_t   uint_fast8_t;
  typedef uint16_t  uint_fast16_t;
  typedef uint32_t  uint_fast32_t;
  typedef uint64_t  uint_fast64_t;

  #define nullptr 0

#endif

