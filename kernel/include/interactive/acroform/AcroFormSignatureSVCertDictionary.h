#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace interactive{

  enum AcroFormSignatureSVCertDictionaryKey{
    AcroFormSignatureSVCertDictionaryKeySubject,   /**< (Optional) An array of byte strings containing DER-encoded X.509v3 certificates that are acceptable for signing. X.509v3 certificates are described in RFC 3280, Internet X.509 Public Key Infrastructure, Certificate and Certificate Revocation List (CRL) Profile (see the Bibliography). The value of the corresponding flag in the Ff entry indicates whether this is a required constraint. */
    AcroFormSignatureSVCertDictionaryKeySubjectDN, /**< (Optional; PDF 1.7) An array of dictionaries, where each dictionary contains key value pairs, that specify the Subject Distinguished Name (DN) that must be present within the certificate for it to be acceptable for signing. */
    AcroFormSignatureSVCertDictionaryKeyKeyUsage,  /**< (Optional; PDF 1.7) An array of ASCII strings, where each string specifies an acceptable key-usage extension that must be present in the signing certificate. Multiple strings specify a range of acceptable key-usage extensions. The key-usage extension is described in RFC 3280. */
    AcroFormSignatureSVCertDictionaryKeyIssuer,    /**< (Optional) An array of byte strings containing DER-encoded X.509v3 certificates of acceptable issuers. */
    AcroFormSignatureSVCertDictionaryKeyOID,       /**< (Optional) An array of byte strings that contain Object Identifiers (OIDs) of the certificate policies that must be present in the signing certificate. */
    AcroFormSignatureSVCertDictionaryKeyURL,       /**< (Optional) A URL, the use for which is defined by the URLType entry. */
    AcroFormSignatureSVCertDictionaryKeyURLType,   /**< (Optional; PDF 1.7) A name indicating the usage of the URL entry. */
    AcroFormSignatureSVCertDictionaryKeyFf         /**< (Optional) A set of bit flags specifying the interpretation of specific entries in this dictionary. */
  };

  class AcroFormSignatureSVCertDictionary : public ObjectDictionary{
  public:
    AcroFormSignatureSVCertDictionary(void);
    ~AcroFormSignatureSVCertDictionary(void);

    void addKey(AcroFormSignatureSVCertDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<AcroFormSignatureSVCertDictionaryKey, std::pair<std::string, uint32_t>> mAcroFormSignatureSVCertDictionaryMap;

  private:
    AcroFormSignatureSVCertDictionary(AcroFormSignatureSVCertDictionary const &) = delete;
    AcroFormSignatureSVCertDictionary &operator=(AcroFormSignatureSVCertDictionary const &) = delete;
  };

}}
