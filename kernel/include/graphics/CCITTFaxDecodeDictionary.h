#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{

  enum CCITTFaxDecodeKey{
    CCITFaxDecodeKeyK,
    CCITFaxDecodeKeyEndOfLine,
    CCITFaxDecodeKeyEncodedByteAlign,
    CCITFaxDecodeKeyColumns,
    CCITFaxDecodeKeyRows,
    CCITFaxDecodeKeyEndOfBlock,
    CCITFaxDecodeKeyBlackIs1,
    CCITFaxDecodeKeyDamagedRowsBeforeError
  };

  /**
   * @brief Representation of alternate image
   */
  class CCITTFaxDecodeDictionary : public ObjectDictionary{
  public:
    CCITTFaxDecodeDictionary(void);
    ~CCITTFaxDecodeDictionary(void);

    void addKey(CCITTFaxDecodeKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<CCITTFaxDecodeKey, std::pair<std::string, uint32_t>> mCCITTFaxDecodeDictionaryMap;

  private:
    CCITTFaxDecodeDictionary(CCITTFaxDecodeDictionary const &) = delete;
    CCITTFaxDecodeDictionary &operator=(CCITTFaxDecodeDictionary const &) = delete;
  };
}
