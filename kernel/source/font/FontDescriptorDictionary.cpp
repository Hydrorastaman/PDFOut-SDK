#include <font/FontDescriptorDictionary.h>

#include <string>

#include <object/ObjectName.h>
#include <object/ObjectRectangle.h>
#include <object/ObjectNumber.h>
#include <object/ObjectStream.h>
#include <object/ObjectByteString.h>
#include <object/ObjectASCIIString.h>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "FontDescriptor";

  std::string const keyFontName = "FontName";
  std::string const keyFontFamily = "FontFamily";
  std::string const keyFontStretch = "FontStretch";
  std::string const keyFontWeight = "FontWeight";
  std::string const keyFlags = "Flags";
  std::string const keyFontBBox = "FontBBox";
  std::string const keyItalicAngle = "ItalicAngle";
  std::string const keyAscent = "Ascent";
  std::string const keyDescent = "Descent";
  std::string const keyLeading = "Leading";
  std::string const keyCapHeight = "CapHeight";
  std::string const keyXHeight = "XHeight";
  std::string const keyStemV = "StemV";
  std::string const keyStemH = "StemH";
  std::string const keyAvgWidth = "AvgWidth";
  std::string const keyMaxWidth = "MaxWidth";
  std::string const keyMissingWidth = "MissingWidth";
  std::string const keyFontFile = "FontFile";
  std::string const keyFontFile2 = "FontFile2";
  std::string const keyFontFile3 = "FontFile3";
  std::string const keyCharSet = "CharSet";
  std::string const keyStyle = "Style";
  std::string const keyLang = "Lang";
  std::string const keyFD = "FD";
  std::string const keyCIDSet = "CIDSet";
}

namespace kernel{ namespace font{

  std::unordered_map<FontDescriptorDictionaryKey, std::pair<std::string, uint32_t>> FontDescriptorDictionary::mFontDescriptorDictionaryMap{
    {FontDescriptorDictionaryKeyFontName,     {keyFontName,     ObjectTypeName}},
    {FontDescriptorDictionaryKeyFontFamily,   {keyFontFamily,   ObjectTypeByteString}},
    {FontDescriptorDictionaryKeyFontStretch,  {keyFontStretch,  ObjectTypeName}},
    {FontDescriptorDictionaryKeyFontWeight,   {keyFontWeight,   ObjectTypeNumber}},
    {FontDescriptorDictionaryKeyFlags,        {keyFlags,        ObjectTypeInteger}},
    {FontDescriptorDictionaryKeyFontBBox,     {keyFontBBox,     ObjectTypeRectangle}},
    {FontDescriptorDictionaryKeyItalicAngle,  {keyItalicAngle,  ObjectTypeNumber}},
    {FontDescriptorDictionaryKeyAscent,       {keyAscent,       ObjectTypeNumber}},
    {FontDescriptorDictionaryKeyDescent,      {keyDescent,      ObjectTypeNumber}},
    {FontDescriptorDictionaryKeyLeading,      {keyLeading,      ObjectTypeNumber}},
    {FontDescriptorDictionaryKeyCapHeight,    {keyCapHeight,    ObjectTypeNumber}},
    {FontDescriptorDictionaryKeyXHeight,      {keyXHeight,      ObjectTypeNumber}},
    {FontDescriptorDictionaryKeyStemV,        {keyStemV,        ObjectTypeNumber}},
    {FontDescriptorDictionaryKeyStemH,        {keyStemH,        ObjectTypeNumber}},
    {FontDescriptorDictionaryKeyAvgWidth,     {keyAvgWidth,     ObjectTypeNumber}},
    {FontDescriptorDictionaryKeyMaxWidth,     {keyMaxWidth,     ObjectTypeNumber}},
    {FontDescriptorDictionaryKeyMissingWidth, {keyMissingWidth, ObjectTypeNumber}},
    {FontDescriptorDictionaryKeyFontFile,     {keyFontFile,     ObjectTypeStream}},
    {FontDescriptorDictionaryKeyFontFile2,    {keyFontFile2,    ObjectTypeStream}},
    {FontDescriptorDictionaryKeyFontFile3,    {keyFontFile3,    ObjectTypeStream}},
    {FontDescriptorDictionaryKeyCharSet,      {keyCharSet,      ObjectTypeASCIIString | ObjectTypeByteString}},
    {FontDescriptorDictionaryKeyStyle,        {keyStyle,        ObjectTypeDictionary}},
    {FontDescriptorDictionaryKeyLang,         {keyLang,         ObjectTypeName}},
    {FontDescriptorDictionaryKeyFD,           {keyFD,           ObjectTypeDictionary}},
    {FontDescriptorDictionaryKeyCIDSet,       {keyCIDSet,       ObjectTypeStream}}
  };

  FontDescriptorDictionary::FontDescriptorDictionary(void)
    : ObjectDictionary(IndirectTypeIndirectable){

    ObjectDictionary::insert(keyType, std::make_unique<ObjectName>(valueType));
  }

  void FontDescriptorDictionary::addKey(FontDescriptorDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mFontDescriptorDictionaryMap, key, std::move(value));
  }
}}
