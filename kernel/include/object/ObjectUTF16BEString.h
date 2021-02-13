#pragma once

#include <string>
#include <vector>

#include <UString.h>
#include <object/Object.h>
#include <object/UTF16BEString.h>

namespace kernel{

  class ObjectUTF16BEString : public Object, public UTF16BEString{
  public:
    explicit ObjectUTF16BEString(pdfout::UString const &str);
    ObjectUTF16BEString(uint16_t const *data, std::size_t len);
    ~ObjectUTF16BEString(void);

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const;
    ObjectUTF16BEString *clone(void) const;

  protected:
    ObjectUTF16BEString(ObjectUTF16BEString const &obj);

  private:
    ObjectUTF16BEString &operator=(ObjectUTF16BEString const &) = delete;
  };

}
