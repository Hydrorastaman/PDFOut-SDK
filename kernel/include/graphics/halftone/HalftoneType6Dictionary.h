#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{

  enum HalftoneType6DictionaryKey{
    HalftoneType6DictionaryKeyHalftoneName,    // Optional
    HalftoneType6DictionaryKeyWidth,           // Required
    HalftoneType6DictionaryKeyHeight,          // Required
    HalftoneType6DictionaryKeyTransferFunction // Optional
  };

  class HalftoneType6Dictionary : public ObjectDictionary{
  public:
    HalftoneType6Dictionary(void);
    ~HalftoneType6Dictionary(void) {}

    void addKey(HalftoneType6DictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<HalftoneType6DictionaryKey, std::pair<std::string, uint32_t>> mHalftoneType6DictionaryMap;

  private:
    HalftoneType6Dictionary(HalftoneType6Dictionary const &) = delete;
    HalftoneType6Dictionary &operator=(HalftoneType6Dictionary const &) = delete;
  };
}
