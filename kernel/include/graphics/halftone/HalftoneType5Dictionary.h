#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{

  enum HalftoneType5DictionaryKey{
    HalftoneType5DictionaryKeyHalftoneName, // Optional
    HalftoneType5DictionaryKeyDefault,      // Required
    HalftoneType5DictionaryKeyGray,         // Optional
    HalftoneType5DictionaryKeyRed,          // Optional
    HalftoneType5DictionaryKeyGreen,        // Optional
    HalftoneType5DictionaryKeyBlue,         // Optional
    HalftoneType5DictionaryKeyCyan,         // Optional
    HalftoneType5DictionaryKeyMagenta,      // Optional
    HalftoneType5DictionaryKeyYellow,       // Optional
    HalftoneType5DictionaryKeyBlack         // Optional
  };

  class HalftoneType5Dictionary : public ObjectDictionary{
  public:
    HalftoneType5Dictionary(void);
    ~HalftoneType5Dictionary(void) {}

    void addKey(HalftoneType5DictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<HalftoneType5DictionaryKey, std::pair<std::string, uint32_t>> mHalftoneType5DictionaryMap;

  private:
    HalftoneType5Dictionary(HalftoneType5Dictionary const &) = delete;
    HalftoneType5Dictionary &operator=(HalftoneType5Dictionary const &) = delete;
  };
}
