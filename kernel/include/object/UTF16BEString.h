#pragma once

#include <vector>
#include <string>

#include <UString.h>
#include <stream/OutputStream.h>

namespace kernel{

  struct SerializeParams;

  class UTF16BEString{
  public:
    /**
     * Creates string from UTF16BE data
     */
    explicit UTF16BEString(pdfout::UString const &str);
    UTF16BEString(uint16_t const *data, std::size_t len);
    ~UTF16BEString(void);

    void serialize(pdfout::OutputStream *stream, SerializeParams *params, bool isBOM) const;

    uint16_t const *getData(void) const{
      return mString.data();
    }

    uint32_t getSize(void) const{
      return (uint32_t) mString.size();
    }

    UTF16BEString *clone(void) const;

  private:
    bool isSpecial(uint16_t ch) const;

  protected:
    UTF16BEString(UTF16BEString const &obj);

  private:
    UTF16BEString &operator=(UTF16BEString const &) = delete;

  private:
    std::vector<uint16_t> mString;
  };

}
