#pragma once

#include <string>
#include <vector>
#include <stdio.h>

#include <object/Object.h>

#pragma warning(disable:4996)

namespace kernel{

  class ObjectName : public Object{
  public:
    explicit ObjectName(std::string const &value, IndirectType indirectable = IndirectTypeNonIndirectable) : Object(ObjectTypeName, indirectable), mValue() {
      char buff[4] = {0};
      for (auto const &elem : value)
        if (isSpecial(elem)){
          sprintf(buff, "#%02x", elem);
          mValue += buff;
        }
        else
          mValue += elem;
    }

    explicit ObjectName(std::vector<uint8_t> const &value, IndirectType indirectable = IndirectTypeNonIndirectable) : Object(ObjectTypeName, indirectable), mValue() {
      char buff[4] = {0};
      for (std::size_t i = 0; i < value.size() && value[i]; ++i){
        if (isSpecial(value[i])){
          sprintf(buff, "#%02x", value[i]);
          mValue += buff;
        }
        else
          mValue += value[i];
      }
    }

    ObjectName(uint8_t const *value, std::size_t size, IndirectType indirectable = IndirectTypeNonIndirectable) : Object(ObjectTypeName, indirectable), mValue() {
      char buff[4] = {0};
      for (std::size_t i = 0; i < size && value[i]; ++i){
        if (isSpecial(value[i])){
          sprintf(buff, "#%02x", value[i]);
          mValue += buff;
        }
        else
          mValue += value[i];
      }
    }

    ObjectName &operator=(std::string const &value){
      mValue = value;
      return *this;
    }

    ~ObjectName(void) {}

    std::string const &getValue(void) const {return mValue;}

    void setValue(std::string const &value) {mValue = value;}

    operator std::string(void) const {return mValue;}

    bool operator==(ObjectName const &obj) const {return mValue == obj.mValue;}

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
      stream->write("/", sizeof(uint8_t), 1);
      stream->write(mValue.c_str(), sizeof(uint8_t), mValue.length());
    }

    ObjectName *clone(void) const {return new ObjectName(*this);}

  protected:
    ObjectName(ObjectName const &obj) : Object(obj), mValue(obj.mValue) {}

  private:
    ObjectName &operator=(ObjectName const &) = delete;

  private:
    bool isSpecial(char ch) const{
      return (ch < 33 || ch == '#' || ch == '/' || ch == '[' || ch == ']' || ch == '%' || ch == '{' || ch == '}' || ch == '(' || ch == ')' || ch == '<' || ch == '>');
    }

  private:
    std::string mValue;
  };
}
