#pragma once

#include <filter/Filter.h>
#include <cstdint>

namespace kernel{ namespace filter{
  class FilterDeflate : public Filter{
  public:
    FilterDeflate(void) : Filter() {}
    explicit FilterDeflate(std::unique_ptr<Filter> attachment) : Filter(std::move(attachment)) {}
    explicit FilterDeflate(Filter *attachment) : Filter(attachment) {}
    ~FilterDeflate(void) {}

    FilterDeflate *clone(void) const;

  protected:
    virtual bool encode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream);
    virtual bool decode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream);

  private:
    FilterDeflate(FilterDeflate const &) = delete;
    FilterDeflate &operator=(FilterDeflate const &) = delete;
  };
}}
