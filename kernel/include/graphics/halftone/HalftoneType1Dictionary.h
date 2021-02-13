#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{

  enum HalftoneType1DictionaryKey{
    HalftoneType1DictionaryKeyHalftoneName,    // Optional
    HalftoneType1DictionaryKeyFrequency,       // Required
    HalftoneType1DictionaryKeyAngle,           // Required
    HalftoneType1DictionaryKeySpotFunction,    // Required
    HalftoneType1DictionaryKeyAccurateScreens, // Optional
    HalftoneType1DictionaryKeyTransferFunction // Optional
  };

  class HalftoneType1Dictionary : public ObjectDictionary{
  public:
    HalftoneType1Dictionary(void);
    ~HalftoneType1Dictionary(void) {}

    void addKey(HalftoneType1DictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<HalftoneType1DictionaryKey, std::pair<std::string, uint32_t>> mHalftoneType1DictionaryMap;

  private:
    HalftoneType1Dictionary(HalftoneType1Dictionary const &) = delete;
    HalftoneType1Dictionary &operator=(HalftoneType1Dictionary const &) = delete;
  };
}
