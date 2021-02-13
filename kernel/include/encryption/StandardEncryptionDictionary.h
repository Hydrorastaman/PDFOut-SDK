#pragma once

#include <encryption/EncryptionDictionary.h>

namespace kernel{ namespace encryption{

  enum StandardEncryptionDictionaryKey{
    StandardEncryptionDictionaryKeyR,               /**< (Required) A number specifying which revision of the standard security handler should be used to interpret this dictionary. */
    StandardEncryptionDictionaryKeyO,               /**< (Required) A 32-byte string, based on both the owner and user passwords, that is used in computing the encryption key and in determining whether a valid owner password was entered. */
    StandardEncryptionDictionaryKeyU,               /**< (Required) A 32-byte string, based on the user password, that is used in determining whether to prompt the user for a password and, if so, whether a valid user or owner password was entered. */
    StandardEncryptionDictionaryKeyP,               /**< (Required) A set of flags specifying which operations are permitted when the document is opened with user access. */
    StandardEncryptionDictionaryKeyEncryptMetadata, /**< (Optional; meaningful only when the value of V is 4; PDF 1.5) Indicates whether the document-level metadata stream is to be encrypted. */
    StandardEncryptionDictionaryKeyOE,              /**< (ExtensionLevel 3; required if R is 5) A 32-byte string, based on the owner and user passwords, that is used in computing the encryption key. */
    StandardEncryptionDictionaryKeyUE,              /**< (ExtensionLevel 3; required if R is 5) A 32-byte string, based on the user password, that is used in computing the encryption key. */
    StandardEncryptionDictionaryKeyPerms            /**< (ExtensionLevel 3; required if R is 5) A16-byte string, encrypted with the file encryption key, that contains an encrypted copy of the permission flags. */
  };

  class StandardEncryptionDictionary : public EncryptionDictionary{
  public:
    StandardEncryptionDictionary(void);
    ~StandardEncryptionDictionary(void);

    using EncryptionDictionary::addKey;
    void addKey(StandardEncryptionDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<StandardEncryptionDictionaryKey, std::pair<std::string, uint32_t>> mStandardEncryptionDictionaryMap;

  private:
    StandardEncryptionDictionary(StandardEncryptionDictionary const &) = delete;
    StandardEncryptionDictionary &operator=(StandardEncryptionDictionary const &) = delete;
  };

}}
