#pragma once

#include <memory>

#include <Halftone.h>
#include <stream/OutputStream.h>

namespace kernel{
  class Object;
  struct SerializeParams;
}

namespace pdfout{ namespace impl{

  class HalftoneImpl : public Halftone{
  public:
    explicit HalftoneImpl(kernel::Object *HalftoneImpl);
    ~HalftoneImpl(void);

    void serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;
    kernel::Object *getObject(void) const;

  private:
    HalftoneImpl(HalftoneImpl const &obj) = delete;
    HalftoneImpl &operator=(HalftoneImpl const &obj) = delete;

  private:
    std::unique_ptr<kernel::Object> mHalftone;
  };

}}
