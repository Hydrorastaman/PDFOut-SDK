#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace encryption{

  enum EncryptionDictionaryKey{
    EncryptionDictionaryKeyFilter,    /**< (Required) The name of the preferred security handler for this document. Typically, it is the name of the security handler that was used to encrypt the document. */
    EncryptionDictionaryKeySubFilter, /**< (Optional; PDF 1.3) A name that completely specifies the format and interpretation of the contents of the encryption dictionary. */
    EncryptionDictionaryKeyV,         /**< (Optional but strongly recommended) A code specifying the algorithm to be used in encrypting and decrypting the document. */
    EncryptionDictionaryKeyLength,    /**< (Optional; PDF 1.4; only if V is 2 or 3) The length of the encryption key, in bits. The value must be a multiple of 8, in the range 40 to 128. */
    EncryptionDictionaryKeyCF,        /**< (Optional; meaningful only when the value of V is 4; PDF 1.5) A dictionary whose keys are crypt filter names and whose values are the corresponding crypt filter dictionaries. */
    EncryptionDictionaryKeyStmF,      /**< (Optional; meaningful only when the value of V is 4; PDF 1.5) The name of the crypt filter that is used by default when decrypting streams. */
    EncryptionDictionaryKeyStrF,      /**< (Optional; meaningful only when the value of V is 4; PDF 1.5) The name of the crypt filter that is used when decrypting all strings in the document. */
    EncryptionDictionaryKeyEFF        /**< (Optional; meaningful only when the value of V is 4; PDF 1.6) The name of the crypt filter that should be used by default when encrypting embedded file streams. */
  };

  class EncryptionDictionary : public ObjectDictionary{
  public:
    EncryptionDictionary(void);
    ~EncryptionDictionary(void);

    void addKey(EncryptionDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<EncryptionDictionaryKey, std::pair<std::string, uint32_t>> mEncryptionDictionaryMap;

  private:
    EncryptionDictionary(EncryptionDictionary const &) = delete;
    EncryptionDictionary &operator=(EncryptionDictionary const &) = delete;
  };

}}
