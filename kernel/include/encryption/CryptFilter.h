#pragma once

#include <memory>
#include <string>
#include <cstdint>

namespace pdfout{
  class OutputStream;
}

namespace kernel{
  struct SerializeParams;
  class ObjectIndirectReference;
}

namespace kernel{ namespace encryption{

  class CryptFilterDictionary;

  enum CryptFilterCfm{
    CryptFilterCfmNone = 0, /**< The application does not decrypt data but directs the input stream to the security handler for decryption. */
    CryptFilterCfmV2,       /**< The application asks the security handler for the encryption key and implicitly decrypts data with Algorithm 3.1, using the RC4 algorithm. */
    CryptFilterCfmAESV2,    /**< (PDF 1.6) The application asks the security handler for the encryption key and implicitly decrypts data with Algorithm 3.1, using the AES algorithm in Cipher Block Chaining (CBC) mode with a 16-byte block size and an initialization vector that is randomly generated and placed as the first 16 bytes in the stream or string. The key size (Length) shall be 128 bits.*/
    CryptFilterCfmAESV3     /**< (PDF 1.7 Ext 3) The application asks the security handler for the encryption key and implicitly decrypts data with Algorithm 3.1a, using the AES-256 algorithm in Cipher Block Chaining (CBC) with padding mode with a 16-byte block size and an initialization vector that is randomly generated and placed as the first 16 bytes in the stream or string. The key size (Length) shall be 256 bits. */
  };

  enum CryptFilterAuthEvent{
    CryptFilterAuthEventDocOpen, /**< Authorization is required when a document is opened. */
    CryptFilterAuthEventEFOpen   /**< Authorization is required when accessing embedded files. */
  };

  class CryptFilter{
  protected:
    CryptFilter(std::string const &name, CryptFilterCfm cfm, CryptFilterAuthEvent authEvent, uint32_t keyLength);

  public:
    ~CryptFilter(void);

    std::string getName(void) const;
    std::unique_ptr<ObjectIndirectReference> serialize(pdfout::OutputStream *stream, SerializeParams *params) const;

    //static std::unique_ptr<CryptFilter> makeIdentityCF(void);
    static std::unique_ptr<CryptFilter> makeStandardCF(CryptFilterCfm cfm, uint32_t length);

  private:
    CryptFilter(CryptFilter const &) = delete;
    CryptFilter &operator=(CryptFilter const &) = delete;

  private:
    std::string mName;
    std::unique_ptr<CryptFilterDictionary> mCryptFilterDictionary;
  };

}}
