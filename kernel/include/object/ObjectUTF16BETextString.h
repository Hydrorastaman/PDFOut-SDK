#pragma once

#include <vector>
#include <string>

#include <UString.h>
#include <object/Object.h>
#include <object/UTF16BEString.h>

namespace kernel{

  class ObjectUTF16BETextString : public Object, public UTF16BEString{
  public:
    explicit ObjectUTF16BETextString(pdfout::UString const &str);
    ObjectUTF16BETextString(uint16_t const *data, std::size_t len);
    ~ObjectUTF16BETextString(void);

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const;
    ObjectUTF16BETextString *clone(void) const;

  protected:
    ObjectUTF16BETextString(ObjectUTF16BETextString const &obj);

  private:
    ObjectUTF16BETextString &operator=(ObjectUTF16BETextString const &) = delete;
  };

}
