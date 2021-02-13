#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace interactive{

  enum AcroFormSignatureLockDictionaryKey{
    AcroFormSignatureLockDictionaryKeyAction, /**< (Required) A name which, in conjunction with Fields, indicates the set of fields that should be locked. */
    AcroFormSignatureLockDictionaryKeyFields  /**< (Required if the value of Action is Include or Exclude) An array of text strings containing field names. */
  };

  class AcroFormSignatureLockDictionary : public ObjectDictionary{
  public:
    AcroFormSignatureLockDictionary(void);
    ~AcroFormSignatureLockDictionary(void);

    void addKey(AcroFormSignatureLockDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<AcroFormSignatureLockDictionaryKey, std::pair<std::string, uint32_t>> mAcroFormSignatureLockDictionaryMap;

  private:
    AcroFormSignatureLockDictionary(AcroFormSignatureLockDictionary const &) = delete;
    AcroFormSignatureLockDictionary &operator=(AcroFormSignatureLockDictionary const &) = delete;
  };

}}
