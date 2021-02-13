#include <graphics/CCITTFaxDecodeDictionary.h>

#include <string>

namespace{
  std::string const keyK = "K";
  std::string const keyEndOfLine = "EndOfLine";
  std::string const keyEncodedByteAlign = "EncodedByteAlign";
  std::string const keyColumns = "Columns";
  std::string const keyRows = "Rows";
  std::string const keyEndOfBlock = "EndOfBlock";
  std::string const keyBlackIs1 = "BlackIs1";
  std::string const keyDamagedRowsBeforeError = "DamagedRowsBeforeError";
}

namespace kernel{

  std::unordered_map<CCITTFaxDecodeKey, std::pair<std::string, uint32_t>> CCITTFaxDecodeDictionary::mCCITTFaxDecodeDictionaryMap{
    {CCITFaxDecodeKeyK,                      {keyK,                      ObjectTypeInteger}},
    {CCITFaxDecodeKeyEndOfLine,              {keyEndOfLine,              ObjectTypeBoolean}},
    {CCITFaxDecodeKeyEncodedByteAlign,       {keyEncodedByteAlign,       ObjectTypeBoolean}},
    {CCITFaxDecodeKeyColumns,                {keyColumns,                ObjectTypeInteger}},
    {CCITFaxDecodeKeyRows,                   {keyRows,                   ObjectTypeInteger}},
    {CCITFaxDecodeKeyEndOfBlock,             {keyEndOfBlock,             ObjectTypeBoolean}},
    {CCITFaxDecodeKeyBlackIs1,               {keyBlackIs1,               ObjectTypeBoolean}},
    {CCITFaxDecodeKeyDamagedRowsBeforeError, {keyDamagedRowsBeforeError, ObjectTypeInteger}}
  };

  CCITTFaxDecodeDictionary::CCITTFaxDecodeDictionary(void)
    : ObjectDictionary(IndirectTypeNonIndirectable){
  }
  
  CCITTFaxDecodeDictionary::~CCITTFaxDecodeDictionary(void){
  }

  void CCITTFaxDecodeDictionary::addKey(CCITTFaxDecodeKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mCCITTFaxDecodeDictionaryMap, key, std::move(value));
  }
}
