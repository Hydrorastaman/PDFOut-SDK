#ifdef __NOT_IMPLEMENTED___

#include <annotations/MarkupAnnotation.h>

MarkupAnnotation::MarkupAnnotation(kernel::ResourceManager *resourceManager, struct Rectangle const &rect, pdfout::UString const &name)
  : Annotation(resourceManager, rect, name){
}

MarkupAnnotation::~MarkupAnnotation(void) {}

void MarkupAnnotation::serialize(kernel::interactive::AnnotationDictionary *annotationDictionary) const{
  Annotation::serialize(annotationDictionary);
}

#endif
