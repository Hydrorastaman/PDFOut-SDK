#pragma once

#include <encryption/EncryptionDictionary.h>

namespace kernel{ namespace encryption{

  enum PublicKeyEncryptionDictionaryKey{
    PublicKeyEncryptionDictionaryKeyRecipients /**< (Required when SubFilter is adbe.pkcs7.s3 or adbe.pkcs7.s4; PDF 1.3) An array of byte-strings, where each string is a PKCS#7 object listing recipients who have been granted equal access rights to the document. */
  };

  class PublicKeyEncryptionDictionary : public EncryptionDictionary{
  public:
    PublicKeyEncryptionDictionary(void);
    ~PublicKeyEncryptionDictionary(void);

    using EncryptionDictionary::addKey;
    void addKey(PublicKeyEncryptionDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<PublicKeyEncryptionDictionaryKey, std::pair<std::string, uint32_t>> mPublicKeyEncryptionDictionaryMap;

  private:
    PublicKeyEncryptionDictionary(PublicKeyEncryptionDictionary const &) = delete;
    PublicKeyEncryptionDictionary &operator=(PublicKeyEncryptionDictionary const &) = delete;
  };

}}
