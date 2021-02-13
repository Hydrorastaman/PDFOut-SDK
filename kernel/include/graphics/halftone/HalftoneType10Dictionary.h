#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{

  enum HalftoneType10DictionaryKey{
    HalftoneType10DictionaryKeyHalftoneName,    // Optional
    HalftoneType10DictionaryKeyXsquare,         // Required
    HalftoneType10DictionaryKeyYsquare,         // Required
    HalftoneType10DictionaryKeyTransferFunction // Optional
  };

  class HalftoneType10Dictionary : public ObjectDictionary{
  public:
    HalftoneType10Dictionary(void);
    ~HalftoneType10Dictionary(void) {}

    void addKey(HalftoneType10DictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<HalftoneType10DictionaryKey, std::pair<std::string, uint32_t>> mHalftoneType10DictionaryMap;

  private:
    HalftoneType10Dictionary(HalftoneType10Dictionary const &) = delete;
    HalftoneType10Dictionary &operator=(HalftoneType10Dictionary const &) = delete;
  };
}
