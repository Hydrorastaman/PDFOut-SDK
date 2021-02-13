#pragma once

#include <cstdint>
#include <filter/Filter.h>

namespace kernel{ namespace filter{
  class FilterAscii85 : public Filter{
  public:
    FilterAscii85(void) : Filter() {}
    explicit FilterAscii85(std::unique_ptr<Filter> attachment) : Filter(std::move(attachment)) {}
    explicit FilterAscii85(Filter *attachment) : Filter(attachment) {}
    ~FilterAscii85(void) {}

    FilterAscii85 *clone(void) const;

  protected:
    virtual bool encode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream);
    virtual bool decode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream);

  private:
    void valueToWord85(uint32_t value, unsigned char word85[5]) const;
    uint32_t word85ToValue(unsigned char word85[5]) const;
    void valueToAsciiWord(uint32_t value, unsigned char wordAscii[4]) const;
    uint32_t asciiWordToValue(unsigned char wordAscii[4]) const;

  private:
    FilterAscii85(FilterAscii85 const &) = delete;
    FilterAscii85 &operator=(FilterAscii85 const &) = delete;
  };
}}
