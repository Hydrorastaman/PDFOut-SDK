#pragma once

#include <map>
#include <memory>
#include <vector>
#include <cstdint>

#include <Encryption.h>

namespace pdfout{
  class OutputStream;
}

namespace kernel{
  struct SerializeParams;
  class ObjectIndirectReference;
}

namespace kernel{ namespace encryption{

  class EncryptionDictionary;
  class CryptFilter;

  class SecurityHandler{
  protected:
    SecurityHandler(uint32_t v, uint32_t keyLength);

  public:
    virtual ~SecurityHandler(void);

    void addCryptFilter(std::unique_ptr<CryptFilter> cryptFilter);
    void setStrF(std::string const &filterName);
    void setStmF(std::string const &filterName);
    void setEFF(std::string const &filterName);

    virtual std::unique_ptr<ObjectIndirectReference> serialize(pdfout::OutputStream *stream, SerializeParams *params) const = 0;
    virtual std::vector<uint8_t> getEncryptionKey(void) const = 0;

  protected:
    void serialize(kernel::encryption::EncryptionDictionary *encryptionDictionary, pdfout::OutputStream *stream, SerializeParams *params) const;
    void validateFilterName(std::string const &filterName) const;

  private:
    SecurityHandler(SecurityHandler const &) = delete;
    SecurityHandler &operator=(SecurityHandler const &) = delete;

  protected:
    uint32_t mV;
    uint32_t mKeyLength;
    std::map<std::string, std::unique_ptr<CryptFilter>> mCryptFilters;
    std::string mStrF;
    std::string mStmF;
    std::string mEFF;
  };
}}
