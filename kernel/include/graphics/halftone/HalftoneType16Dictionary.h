#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{

  enum HalftoneType16DictionaryKey{
    HalftoneType16DictionaryKeyHalftoneName,    // Optional
    HalftoneType16DictionaryKeyWidth,           // Required
    HalftoneType16DictionaryKeyHeight,          // Required
    HalftoneType16DictionaryKeyWidth2,          // Optional
    HalftoneType16DictionaryKeyHeight2,         // Optional
    HalftoneType16DictionaryKeyTransferFunction // Optional
  };

  class HalftoneType16Dictionary : public ObjectDictionary{
  public:
    HalftoneType16Dictionary(void);
    ~HalftoneType16Dictionary(void) {}

    void addKey(HalftoneType16DictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<HalftoneType16DictionaryKey, std::pair<std::string, uint32_t>> mHalftoneType16DictionaryMap;

  private:
    HalftoneType16Dictionary(HalftoneType16Dictionary const &) = delete;
    HalftoneType16Dictionary &operator=(HalftoneType16Dictionary const &) = delete;
  };
}
