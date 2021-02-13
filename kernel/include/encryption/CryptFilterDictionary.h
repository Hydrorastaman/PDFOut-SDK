#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace encryption{

  enum CryptFilterDictionaryKey{
    CryptFilterDictionaryKeyCFM,       /**< (PDF 1.5, Optional) The method used, if any, by the consumer application to decrypt data. */
    CryptFilterDictionaryKeyAuthEvent, /**< (PDF 1.5, Optional) The event to be used to trigger the authorization that is required to access encryption keys used by this filter. If authorization fails, the event should fail. */
    CryptFilterDictionaryKeyLength     /**< (PDF 1.5, Optional) The bit length of the encryption key. It must be a multiple of 8 in the range of 40 to 128. */
  };

  class CryptFilterDictionary : public ObjectDictionary{
  public:
    CryptFilterDictionary(void);
    ~CryptFilterDictionary(void);

    void addKey(CryptFilterDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<CryptFilterDictionaryKey, std::pair<std::string, uint32_t>> mCryptFilterDictionaryMap;

  private:
    CryptFilterDictionary(CryptFilterDictionary const &) = delete;
    CryptFilterDictionary &operator=(CryptFilterDictionary const &) = delete;
  };

}}
