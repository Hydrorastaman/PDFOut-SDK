#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace interactive{

  enum AcroFormSignatureSVDictionaryKey{
    AcroFormSignatureSVDictionaryKeyFilter,           /**< (Optional) The signature handler to be used to sign the signature field. */
    AcroFormSignatureSVDictionaryKeySubFilter,        /**< (Optional) An array of names indicating encodings to use when signing. */
    AcroFormSignatureSVDictionaryKeyDigestMethod,     /**< (Optional; PDF 1.7) An array of names indicating acceptable digest algorithms to use while signing. */
    AcroFormSignatureSVDictionaryKeyV,                /**< (Optional) The minimum required capability of the signature field seed value dictionary parser. */
    AcroFormSignatureSVDictionaryKeyCert,             /**< (Optional) A certificate seed value dictionary containing information about the certificate to be used when signing. */
    AcroFormSignatureSVDictionaryKeyReasons,          /**< (Optional) An array of text strings that specifying possible reasons for signing a document. */
    AcroFormSignatureSVDictionaryKeyMDP,              /**< (Optional; PDF 1.6) A dictionary containing a single entry whose key is P and whose value is an integer between 0 and 3. */
    AcroFormSignatureSVDictionaryKeyTimeStamp,        /**< (Optional; PDF 1.6) A time stamp dictionary. */
    AcroFormSignatureSVDictionaryKeyLegalAttestation, /**< (Optional; PDF 1.6) An array of text strings specifying possible legal attestations. */
    AcroFormSignatureSVDictionaryKeyAddRevInfo,       /**< (Optional; PDF 1.7) A flag indicating whether revocation checking should be carried out. */
    AcroFormSignatureSVDictionaryKeyFf                /**< (Optional) A set of bit flags specifying the interpretation of specific entries in this dictionary. */
  };

  class AcroFormSignatureSVDictionary : public ObjectDictionary{
  public:
    AcroFormSignatureSVDictionary(void);
    ~AcroFormSignatureSVDictionary(void);

    void addKey(AcroFormSignatureSVDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<AcroFormSignatureSVDictionaryKey, std::pair<std::string, uint32_t>> mAcroFormSignatureSVDictionaryMap;

  private:
    AcroFormSignatureSVDictionary(AcroFormSignatureSVDictionary const &) = delete;
    AcroFormSignatureSVDictionary &operator=(AcroFormSignatureSVDictionary const &) = delete;
  };

}}
