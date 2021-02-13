#pragma once
#ifdef __NOT_IMPLEMENTED___

#include <memory>

#include <annotations/Annotation.h>
#include <Destination.h>

namespace kernel{
  class ObjectArray;
}

class Page;

class LinkAnnotation : public Annotation{
  friend class Page;

protected:
  LinkAnnotation(kernel::ResourceManager *resourceManager, struct Rectangle const &rect, pdfout::UString const &name);

public:
  ~LinkAnnotation(void);

  std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

  void setDestination(Page const *page, DestinationFit fit, int32_t param0 = 0, int32_t param1 = 0, int32_t param2 = 0, int32_t param3 = 0);
  void setHighlighting(AnnotationHighlightingMode mode);

private:
  LinkAnnotation(LinkAnnotation const &) = delete;
  LinkAnnotation &operator=(LinkAnnotation const &) = delete;

private:
  std::unique_ptr<kernel::ObjectArray> mExplicitDestination;
  AnnotationHighlightingMode mHighlightMode;
};
#endif
