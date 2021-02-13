#pragma once

#include <memory>

namespace kernel{
  class DocumentOutlineDictionary;
  struct SerializeParams;
};

namespace pdfout{
  class OutputStream;
}

namespace pdfout{ namespace impl{

  class OutlineImpl{
  public:
    explicit OutlineImpl(kernel::DocumentOutlineDictionary *root);
    ~OutlineImpl(void);

    void serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

  private:
    OutlineImpl(OutlineImpl const &) = delete;
    OutlineImpl &operator=(OutlineImpl const &) = delete;

  private:
    kernel::DocumentOutlineDictionary *mRoot;
  };

}}
