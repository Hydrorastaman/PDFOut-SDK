#include <graphics/color/ColorICCStream.h>

#include <object/ObjectInteger.h>
#include <object/ObjectName.h>

namespace{
  std::string const keyN = "N";

  std::string const keyAlternate = "Alternate";
  std::string const valueAlternateDeviceGray = "DeviceGray"; // for N == 1
  std::string const valueAlternateDeviceRGB = "DeviceRGB";   // for N == 3
  std::string const valueAlternateDeviceCMYK = "DeviceCMYK"; // for N == 4

  std::string const keyRange = "Range";
  std::string const keyMetadata = "Metadata";
}

namespace kernel{ namespace graphics{ namespace color{

  std::unordered_map<ICCStreamDictionaryKey, std::pair<std::string, uint32_t>> ColorICCStream::mColorICCStreamMap{
    {ICCStreamDictionaryKeyN,         {keyN,         ObjectTypeInteger}},
    {ICCStreamDictionaryKeyAlternate, {keyAlternate, ObjectTypeArray | ObjectTypeName}},
    {ICCStreamDictionaryKeyRange,     {keyRange,     ObjectTypeArray}},
    {ICCStreamDictionaryKeyMetadata,  {keyMetadata,  ObjectTypeStream}}
  };

  ColorICCStream::ColorICCStream(void)
    : ObjectStream(){
  }

  ColorICCStream::ColorICCStream(ColorICCStream const &obj)
    : ObjectStream(obj){
  }

  ColorICCStream::~ColorICCStream(void) {}

  void ColorICCStream::addKey(ICCStreamDictionaryKey key, std::unique_ptr<Object> value){
    ObjectStream::addKey(mColorICCStreamMap, key, std::move(value));
  }

  ColorICCStream *ColorICCStream::clone(void) const{
    return new ColorICCStream(*this);
  }

}}}
