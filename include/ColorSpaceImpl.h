#pragma once

#include <memory>
#include <string>

#include <ColorSpace.h>

namespace pdfout{
  class OutputStream;
}

namespace kernel{
  class ObjectIndirectReference;
  struct SerializeParams;
}

namespace pdfout{ namespace impl{

  class ColorSpaceImpl : public ColorSpace{
  protected:
    explicit ColorSpaceImpl(ColorSpaceFamily family);

  public:
    virtual ~ColorSpaceImpl(void);

    std::string const &getUID(void) const;
    ColorSpaceFamily getFamily(void) const;

    virtual std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const = 0;

  private:
    static std::string generateUID(ColorSpaceFamily family);

  private:
    ColorSpaceImpl(ColorSpaceImpl const &) = delete;
    ColorSpaceImpl &operator=(ColorSpaceImpl const &) = delete;

  private:
    std::string mColorUID;
    ColorSpaceFamily mFamily;
  };

}}
