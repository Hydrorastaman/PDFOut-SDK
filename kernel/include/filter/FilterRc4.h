#pragma once

#include <vector>
#include <cstdint>

#include <arc4.h>
#include <filter/Filter.h>

namespace kernel{ namespace filter{

  class FilterRc4 : public Filter{
  public:
    FilterRc4(uint8_t const *key, std::size_t length);
    explicit FilterRc4(std::vector<uint8_t> const &key, std::unique_ptr<Filter> attachment);
    explicit FilterRc4(std::vector<uint8_t> const &key, Filter *attachment);
    ~FilterRc4(void);

    FilterRc4 *clone(void) const;

  protected:
    virtual bool encode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream);
    virtual bool decode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream);
    bool processing(pdfout::InputStream *istream, pdfout::OutputStream *ostream);

  private:
    void reset(void);

  private:
    FilterRc4(FilterRc4 const &) = delete;
    FilterRc4 &operator=(FilterRc4 const &) = delete;

  private:
    std::vector<uint8_t> mKey;
    CryptoPP::ARC4 mEncryptor;
  };

}}
