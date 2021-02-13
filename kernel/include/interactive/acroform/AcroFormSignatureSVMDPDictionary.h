#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace interactive{

  enum AcroFormSignatureSVMDPDictionaryKey{
    AcroFormSignatureSVMDPDictionaryKeyP /**< (Optional) A value of 0 defines the signature as an ordinary (non-author) signature. The values 1 through 3 are used for author signatures and correspond to the value of P in a DocMDP transform parameters dictionary */
  };

  class AcroFormSignatureSVMDPDictionary : public ObjectDictionary{
  public:
    AcroFormSignatureSVMDPDictionary(void);
    ~AcroFormSignatureSVMDPDictionary(void);

    void addKey(AcroFormSignatureSVMDPDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<AcroFormSignatureSVMDPDictionaryKey, std::pair<std::string, uint32_t>> mAcroFormSignatureSVMDPDictionaryMap;

  private:
    AcroFormSignatureSVMDPDictionary(AcroFormSignatureSVMDPDictionary const &) = delete;
    AcroFormSignatureSVMDPDictionary &operator=(AcroFormSignatureSVMDPDictionary const &) = delete;
  };

}}
