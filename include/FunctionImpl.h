#pragma once

#include <memory>

#include <Function.h>
#include <stream/OutputStream.h>

namespace kernel{
  class Object;
  struct SerializeParams;
}

namespace pdfout{ namespace impl{

  class FunctionImpl : public Function{
  public:
    explicit FunctionImpl(kernel::Object *function);
    ~FunctionImpl(void);

    void serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;
    kernel::Object const *getObject(void) const;

  private:
    FunctionImpl(FunctionImpl const &) = delete;
    FunctionImpl &operator=(FunctionImpl const &) = delete;

  private:
    std::unique_ptr<kernel::Object> mFunction;
  };

}}
