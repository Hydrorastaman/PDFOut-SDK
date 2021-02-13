#pragma once

#include <string>
#include <cstdint>

#include <Serializable.h>
#include <Indirectable.h>

namespace kernel{

  class Object;

  enum ObjectType{
    ObjectTypeNull                = 0x00001,
    ObjectTypeBoolean             = 0x00002,
    ObjectTypeNumber              = 0x00004,
    ObjectTypeNumberTree          = 0x00008,
    ObjectTypeByteString          = 0x00010,
    ObjectTypeASCIIString         = 0x00020,
    ObjectTypePDFDocEncodedString = 0x00040,
    ObjectTypeUTF16BEString       = 0x00080,
    ObjectTypeName                = 0x00100,
    ObjectTypeNameTree            = 0x00200,
    ObjectTypeArray               = 0x00400,
    ObjectTypeDictionary          = 0x00800,
    ObjectTypeStream              = 0x01000,
    ObjectTypeIndirectReference   = 0x02000,
    ObjectTypeDate                = 0x04000,
    ObjectTypeInteger             = 0x08000,
    ObjectTypeFileSpecification   = 0x10000,
    ObjectTypeRectangle           = 0x20000,
    ObjectTypeFunction            = 0x40000,
    ObjectTypeUTF16BETextString   = 0x80000
  };

  #define OBJECT_TYPE_STRING (ObjectTypeASCIIString | ObjectTypeUTF16BEString | ObjectTypePDFDocEncodedString)
  #define OBJECT_TYPE_TEXT_STRING (ObjectTypeUTF16BETextString | ObjectTypePDFDocEncodedString)

  class Object : public Serializable, public Indirectable{
  public:
    virtual ~Object(void) {}
    ObjectType getType(void) const {return mType;}

    virtual Object *clone(void) const = 0;

  protected:
    Object(ObjectType type, IndirectType indirectable = IndirectTypeIndirectable) : Serializable(), Indirectable(indirectable), mType(type) {}

    Object(Object const &obj) : Serializable(obj), Indirectable(obj), mType(obj.mType) {}

    void writeHeader(pdfout::OutputStream *stream, uint32_t number, uint16_t generation) const{
      char dest[12 * 2 + 10] = {0};
      sprintf(dest, "%i %i obj\r\n", number, generation);
      stream->write(dest, sizeof(uint8_t), strlen(dest));
    }

    void writeFooter(pdfout::OutputStream *stream) const{
      std::string footer = "endobj\r\n\r\n";
      stream->write(footer.c_str(), sizeof(uint8_t), footer.length());
    }

  private:
    Object &operator=(Object const &) = delete;

  private:
    ObjectType mType;
  };
}
