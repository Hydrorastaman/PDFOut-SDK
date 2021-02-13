#pragma once

#include <string>
#include <exception>

#include "UString.h"
#include "PdfOutConfig.h"

namespace pdfout{

  class PDFOUT_EXPORTS Exception : public std::exception{
  public:
    Exception(int code, char const *message)
      : mCode(code),
      mMessage(UString(message, Encoding_ASCII)){
    }

    Exception(int code, UString const &message)
      : mCode(code),
      mMessage(message){
    }

    virtual ~Exception(void) {}

    virtual char const *what(void) const{
      return (char const *) mMessage.getData();
    }

    int getCode(void) const {
      return mCode;
    }

  private:
    int mCode;        /**< error code */
    UString mMessage; /**< error description */
  };

}

#define NOT_IMPLEMENTED() \
  do{ \
    std::string msg = std::string(__FUNCTION__) + " at " + __FILE__ + "@" + std::to_string(__LINE__) + " is not implemented."; \
    throw pdfout::Exception(0, msg.c_str()); \
  } \
  while (0)

#define RUNTIME_EXCEPTION(message) \
  do{ \
    std::string const msg = std::string(__FUNCTION__) + " at " + __FILE__ + "@" + std::to_string(__LINE__) + " : " + (message); \
    throw pdfout::Exception(0, msg.c_str()); \
  } \
  while (0)
