#pragma once
#ifdef __NOT_IMPLEMENTED___

#include <memory>

#include <Types.h>
#include <ContentStream.h>

namespace pdfout{
  class OutputStream;
}

namespace kernel{
  class ResourceManager;
  struct SerializeParams;
  class ObjectIndirectReference;
}

class FormXObject{
public:
  FormXObject(kernel::ResourceManager *resourceManager, struct Rectangle const & rect);
  ~FormXObject(void);

  static FormXObject *createFormXObject(kernel::ResourceManager *resourceManager, struct Rectangle const & rect);

  void serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;
  std::unique_ptr<kernel::ObjectIndirectReference> getReference(void) const;

  ContentStream *getContents(void) const;

  /**
   * Specifying the form matrix, which maps form space into user space.
   */
  void setMatrix(TransformationMatrix const &matrix);

private:
  FormXObject(FormXObject const &) = delete;
  FormXObject &operator=(FormXObject const &) = delete;

private:
  kernel::ResourceManager *mResourceManager;
  struct Rectangle mRect;
  std::unique_ptr<kernel::ObjectIndirectReference> mRef;
  std::unique_ptr<ContentStream> mContentStream;
  TransformationMatrix mMatrix;
};
#endif
