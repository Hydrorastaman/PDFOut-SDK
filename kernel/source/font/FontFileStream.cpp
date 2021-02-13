#include <font/FontFileStream.h>

#include <string>

#include <object/ObjectName.h>
#include <object/ObjectDictionary.h>
#include <object/ObjectInteger.h>

namespace{
  std::string const keyLength1 = "Length1";
  std::string const keyLength2 = "Length2";
  std::string const keyLength3 = "Length3";
  std::string const keySubtype = "Subtype";
  std::string const keyMetadata = "Metadata";
}

namespace kernel{ namespace font{

  std::unordered_map<FontFileStreamKey, std::pair<std::string, uint32_t>> FontFileStream::mFontFileStreamMap{
    {FontFileStreamKeyLength1,  {keyLength1,  ObjectTypeInteger}},
    {FontFileStreamKeyLength2,  {keyLength2,  ObjectTypeInteger}},
    {FontFileStreamKeyLength3,  {keyLength3,  ObjectTypeInteger}},
    {FontFileStreamKeySubtype,  {keySubtype,  ObjectTypeName}},
    {FontFileStreamKeyMetadata, {keyMetadata, ObjectTypeStream}}
  };

  FontFileStream::FontFileStream(void)
    : ObjectStream(){

    //FilterName filters[2] = {FilterNameASCIIHex, FilterNameFlate};
    //setFilter(filters, 2);
  }

  void FontFileStream::addKey(FontFileStreamKey key, std::unique_ptr<Object> value){
    ObjectStream::addKey(mFontFileStreamMap, key, std::move(value));
  }
}}
