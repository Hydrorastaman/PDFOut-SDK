#pragma once

#include <cstdint>
#include <vector>

#include <aes.h>
#include <modes.h>

#include <filter/Filter.h>

namespace kernel{ namespace filter{

  class FilterAes : public Filter{
  public:
    FilterAes(uint8_t const *key, std::size_t length);
    FilterAes(std::vector<uint8_t> const &key, std::unique_ptr<Filter> attachment);
    FilterAes(std::vector<uint8_t> const &key, Filter *attachment);
    ~FilterAes(void);

    FilterAes *clone(void) const;

  protected:
    virtual bool encode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream);
    virtual bool decode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream);

  private:
    FilterAes(FilterAes const &) = delete;
    FilterAes &operator=(FilterAes const &) = delete;

  private:
    CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption mEncryptor;
    CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption mDecryptor;
    std::vector<uint8_t> mKey;
  };

}}
