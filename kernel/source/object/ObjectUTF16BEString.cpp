#include <object/ObjectUTF16BEString.h>

namespace kernel{
  ObjectUTF16BEString::ObjectUTF16BEString(pdfout::UString const &str)
    : Object(ObjectTypeUTF16BEString, IndirectTypeNonIndirectable),
    UTF16BEString(str){
  }

  ObjectUTF16BEString::ObjectUTF16BEString(uint16_t const *data, std::size_t len)
    : Object(ObjectTypeUTF16BEString, IndirectTypeNonIndirectable),
    UTF16BEString(data, len){
  }

  ObjectUTF16BEString::ObjectUTF16BEString(ObjectUTF16BEString const &obj)
    : Object(obj),
    UTF16BEString(obj){
  }

  ObjectUTF16BEString::~ObjectUTF16BEString(void) {}

  void ObjectUTF16BEString::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
    UTF16BEString::serialize(stream, params, false);
  }

  ObjectUTF16BEString *ObjectUTF16BEString::clone(void) const{
    return new ObjectUTF16BEString(*this);
  }
}
