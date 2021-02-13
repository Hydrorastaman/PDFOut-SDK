#include <object/ObjectUTF16BETextString.h>

namespace kernel{
  ObjectUTF16BETextString::ObjectUTF16BETextString(pdfout::UString const &str)
    : Object(ObjectTypeUTF16BETextString, IndirectTypeNonIndirectable),
    UTF16BEString(str){
  }

  ObjectUTF16BETextString::ObjectUTF16BETextString(uint16_t const *data, std::size_t len)
    : Object(ObjectTypeUTF16BETextString, IndirectTypeNonIndirectable),
    UTF16BEString(data, len){
  }

  ObjectUTF16BETextString::ObjectUTF16BETextString(ObjectUTF16BETextString const &obj)
    : Object(obj),
    UTF16BEString(obj){
  }

  ObjectUTF16BETextString::~ObjectUTF16BETextString(void) {}

  void ObjectUTF16BETextString::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
    UTF16BEString::serialize(stream, params, true);
  }

  ObjectUTF16BETextString *ObjectUTF16BETextString::clone(void) const{
    return new ObjectUTF16BETextString(*this);
  }
}
