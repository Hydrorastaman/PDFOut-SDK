#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{

  enum SoftMaskDictionaryKey{
    SoftMaskDictionaryKeyS,  // Required
    SoftMaskDictionaryKeyG,  // Required
    SoftMaskDictionaryKeyBC, // Optional
    SoftMaskDictionaryKeyTR  // Optional
  };

  class SoftMaskDictionary : public ObjectDictionary{
  public:
    SoftMaskDictionary(void);
    ~SoftMaskDictionary(void);

    void addKey(SoftMaskDictionaryKey key, std::unique_ptr<Object> value);

    SoftMaskDictionary *clone(void) const;

  private:
    static std::unordered_map<SoftMaskDictionaryKey, std::pair<std::string, uint32_t>> mSoftMaskDictionaryMap;

  private:
    SoftMaskDictionary(SoftMaskDictionary const &obj);
    SoftMaskDictionary &operator=(SoftMaskDictionary const &) = delete;
  };
}
