#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace interactive{

  enum AcroFormSignatureSVTimeStampDictionaryKey{
    AcroFormSignatureSVTimeStampDictionaryKeyURL, /**< An ASCII string specifying the URL of a time-stamping server, providing a time stamp that is compliant with RFC 3161, Internet X.509 Public Key Infrastructure Time-Stamp Protocol. */
    AcroFormSignatureSVTimeStampDictionaryKeyFf   /**< An integer whose value is 1 (the signature is required to have a time stamp) or 0 (the signature is not required to have a time stamp). */
  };

  class AcroFormSignatureSVTimeStampDictionary : public ObjectDictionary{
  public:
    AcroFormSignatureSVTimeStampDictionary(void);
    ~AcroFormSignatureSVTimeStampDictionary(void);

    void addKey(AcroFormSignatureSVTimeStampDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<AcroFormSignatureSVTimeStampDictionaryKey, std::pair<std::string, uint32_t>> mAcroFormSignatureSVTimeStampDictionaryMap;

  private:
    AcroFormSignatureSVTimeStampDictionary(AcroFormSignatureSVTimeStampDictionary const &) = delete;
    AcroFormSignatureSVTimeStampDictionary &operator=(AcroFormSignatureSVTimeStampDictionary const &) = delete;
  };

}}
