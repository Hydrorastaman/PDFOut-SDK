#include <graphics/ImageDictionary.h>

#include <string>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "XObject";

  std::string const keySubtype = "Subtype";
  std::string const valueSubtype = "Image";

  std::string const keyWidth = "Width";
  std::string const keyHeight = "Height";
  std::string const keyColorSpace = "ColorSpace";
  std::string const keyBitsPerComponent = "BitsPerComponent";
  std::string const keyIntent = "Intent";
  std::string const keyImageMask = "ImageMask";
  std::string const keyMask = "Mask";
  std::string const keyDecode = "Decode";
  std::string const keyInterpolate = "Interpolate";
  std::string const keyAlternates = "Alternates";
  std::string const keySMask = "SMask";
  std::string const keySMaskInData = "SMaskInData";
  std::string const keyName = "Name";
  std::string const keyStructParent = "StructParent";
  std::string const keyID = "ID";
  std::string const keyOPI = "OPI";
  std::string const keyMetadata = "Metadata";
  std::string const keyOC = "OC";              
}

namespace kernel{
  std::unordered_map<ImageDictionaryKey, std::pair<std::string, uint32_t>> ImageDictionary::mImageDictionaryMap{
    {ImageDictionaryKeyWidth,            {keyWidth,            ObjectTypeInteger}},
    {ImageDictionaryKeyHeight,           {keyHeight,           ObjectTypeInteger}},
    {ImageDictionaryKeyColorSpace,       {keyColorSpace,       ObjectTypeName | ObjectTypeArray}},
    {ImageDictionaryKeyBitsPerComponent, {keyBitsPerComponent, ObjectTypeInteger}},
    {ImageDictionaryKeyIntent,           {keyIntent,           ObjectTypeName}},
    {ImageDictionaryKeyImageMask,        {keyImageMask,        ObjectTypeBoolean}},
    {ImageDictionaryKeyMask,             {keyMask,             ObjectTypeStream | ObjectTypeArray}},
    {ImageDictionaryKeyDecode,           {keyDecode,           ObjectTypeArray}},
    {ImageDictionaryKeyInterpolate,      {keyInterpolate,      ObjectTypeBoolean}},
    {ImageDictionaryKeyAlternates,       {keyAlternates,       ObjectTypeArray}},
    {ImageDictionaryKeySMask,            {keySMask,            ObjectTypeStream}},
    {ImageDictionaryKeySMaskInData,      {keySMaskInData,      ObjectTypeInteger}},
    {ImageDictionaryKeyName,             {keyName,             ObjectTypeName}},
    {ImageDictionaryKeyStructParent,     {keyStructParent,     ObjectTypeInteger}},
    {ImageDictionaryKeyID,               {keyID,               ObjectTypeByteString}},
    {ImageDictionaryKeyOPI,              {keyOPI,              ObjectTypeDictionary}},
    {ImageDictionaryKeyMetadata,         {keyMetadata,         ObjectTypeStream}},
    {ImageDictionaryKeyOC,               {keyOC,               ObjectTypeDictionary}}
  };

  ImageDictionary::ImageDictionary(void)
    : ObjectStream(){
    ObjectStream::insert(keyType, std::make_unique<ObjectName>(valueType));
    ObjectStream::insert(keySubtype, std::make_unique<ObjectName>(valueSubtype));
  }

  void ImageDictionary::addKey(ImageDictionaryKey key, std::unique_ptr<Object> value){
    ObjectStream::addKey(mImageDictionaryMap, key, std::move(value));
  }
}
