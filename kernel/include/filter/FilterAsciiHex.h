#pragma once

#include <filter/Filter.h>

namespace kernel{ namespace filter{
  class FilterAsciiHex : public Filter{
  public:
    FilterAsciiHex(void) : Filter() {}
    explicit FilterAsciiHex(std::unique_ptr<Filter> attachment) : Filter(std::move(attachment)) {}
    explicit FilterAsciiHex(Filter *attachment) : Filter(attachment) {}
    ~FilterAsciiHex(void) {}

    FilterAsciiHex *clone(void) const;

  protected:
    virtual bool encode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream);
    virtual bool decode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream);

  private:
    FilterAsciiHex(FilterAsciiHex const &) = delete;
    FilterAsciiHex &operator=(FilterAsciiHex const &) = delete;
  };
}}
