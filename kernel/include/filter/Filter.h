#pragma once

#include <utility>
#include <memory>

namespace pdfout{
  class InputStream;
  class OutputStream;
}

namespace kernel{ namespace filter{
  class Filter{
  public:
    virtual ~Filter(void) {}

    bool encode(pdfout::InputStream *istream, pdfout::OutputStream *ostream);
    bool decode(pdfout::InputStream *istream, pdfout::OutputStream *ostream);
    void attach(std::unique_ptr<Filter> attachment);

    virtual Filter *clone(void) const = 0;

  protected:
    Filter(void);
    explicit Filter(std::unique_ptr<Filter> attachment);
    explicit Filter(Filter *attachment);

    virtual bool encode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream) = 0;
    virtual bool decode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream) = 0;

  private:
    Filter(Filter const &) = delete;
    Filter &operator=(Filter const &) = delete;

  protected:
    std::unique_ptr<Filter> mAttachment;
  };
}}
