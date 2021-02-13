#pragma once
#ifdef __NOT_IMPLEMENTED___

#include <annotations/Annotation.h>

class MarkupAnnotation : public Annotation{
protected:
  MarkupAnnotation(kernel::ResourceManager *resourceManager, struct Rectangle const &rect, pdfout::UString const &name);

public:
  ~MarkupAnnotation(void);

private:
  MarkupAnnotation(MarkupAnnotation const &) = delete;
  MarkupAnnotation &operator=(MarkupAnnotation const &) = delete;

protected:
  void serialize(kernel::interactive::AnnotationDictionary *annotationDictionary) const;

private:
};
#endif
