#include <graphics/image/InlineImageStream.h>

#include <string>

namespace{
  std::string const keyBPC = "BPC";
  std::string const keyCS = "CS";
  std::string const keyD = "D";
  std::string const keyDP = "DP";
  std::string const keyF = "F";
  std::string const keyH = "H";
  std::string const keyIM = "IM";
  std::string const keyIntent = "Intent";
  std::string const keyI = "I";
  std::string const keyW = "W";
}

namespace kernel{ namespace graphics{ namespace image{

  std::unordered_map<InlineImageStreamKey, std::pair<std::string, uint32_t>> InlineImageStream::mInlineImageStreamMap = {
    {InlineImageStreamKeyBPC,     {keyBPC,    ObjectTypeInteger}},
    {InlineImageStreamKeyCS,      {keyCS,     ObjectTypeName | ObjectTypeArray}},
    {InlineImageStreamKeyD,       {keyD,      ObjectTypeArray}},
    {InlineImageStreamKeyDP,      {keyDP,     ObjectTypeDictionary | ObjectTypeArray}},
    {InlineImageStreamKeyF,       {keyF,      ObjectTypeName | ObjectTypeArray}},
    {InlineImageStreamKeyH,       {keyH,      ObjectTypeInteger}},
    {InlineImageStreamKeyIM,      {keyIM,     ObjectTypeBoolean}},
    {InlineImageStreamKeyIntent,  {keyIntent, ObjectTypeName}},
    {InlineImageStreamKeyI,       {keyI,      ObjectTypeBoolean}},
    {InlineImageStreamKeyW,       {keyW,      ObjectTypeInteger}}
  };

  InlineImageStream::InlineImageStream(void)
    : ObjectStream(IndirectTypeNonIndirectable){
  }
  
  InlineImageStream::~InlineImageStream(void){
  }

  void InlineImageStream::addKey(InlineImageStreamKey key, std::unique_ptr<Object> value){
    ObjectStream::addKey(mInlineImageStreamMap, key, std::move(value));
  }

}}}
