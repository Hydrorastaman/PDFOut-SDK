#pragma once

#include <vector>
#include <string>

#include <object/Object.h>

namespace kernel{

  class ObjectPDFDocEncodedString : public Object{
  public:
    explicit ObjectPDFDocEncodedString(char const *data)
      : Object(ObjectTypePDFDocEncodedString, IndirectTypeNonIndirectable),
      mString(data, data + strlen(data)) {}

    ObjectPDFDocEncodedString(char const *data, std::size_t len)
      : Object(ObjectTypePDFDocEncodedString, IndirectTypeNonIndirectable),
      mString(data, data + len) {}

    explicit ObjectPDFDocEncodedString(std::string const &data)
      : Object(ObjectTypePDFDocEncodedString, IndirectTypeNonIndirectable),
      mString(std::begin(data), std::end(data)) {}

    ~ObjectPDFDocEncodedString(void) {}

    virtual void serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
      stream->write("(", sizeof(uint8_t), 1);

      for (auto const &elem: mString){
        if (isSpecial(elem))
          stream->write("\\", sizeof(uint8_t), 1);

        stream->write(&elem, sizeof(uint8_t), 1);
      }

      stream->write(")", sizeof(uint8_t), 1);
    }

    ObjectPDFDocEncodedString *clone(void) const{
      return new ObjectPDFDocEncodedString(*this);
    }

    void const *getData(void) const{
      return mString.data();
    }

    uint32_t getSize(void) const{
      return mString.size();
    }

  protected:
    ObjectPDFDocEncodedString(ObjectPDFDocEncodedString const &obj)
      : Object(obj),
      mString(obj.mString){
    }

  private:
    ObjectPDFDocEncodedString &operator=(ObjectPDFDocEncodedString const &) = delete;

  private:
    bool isSpecial(char ch) const{
      return (ch == '(' || ch == ')' || ch == '\n' || ch == '\r' || ch == '\t' || ch == '\b' || ch == '\f' || ch == '\\');
    }

  private:
    std::vector<char> mString;
  };
}
