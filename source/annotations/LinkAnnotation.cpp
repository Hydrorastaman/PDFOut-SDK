#ifdef __NOT_IMPLEMENTED___

#include <annotations/LinkAnnotation.h>

#include <unordered_map>

#include <object/ObjectName.h>
#include <object/ObjectArray.h>
#include <interactive/annotation/LinkAnnotationDictionary.h>

using namespace kernel;
using namespace kernel::interactive;

#include <Page.h>

LinkAnnotation::LinkAnnotation(kernel::ResourceManager *resourceManager, struct Rectangle const &rect, pdfout::UString const &name)
  : Annotation(resourceManager, rect, name),
  mExplicitDestination(nullptr),
  mHighlightMode(AnnotationHighlightingModeInvert){
}

LinkAnnotation::~LinkAnnotation(void) {}

std::unique_ptr<kernel::ObjectIndirectReference> LinkAnnotation::serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const{
  std::unique_ptr<LinkAnnotationDictionary> annot = std::make_unique<LinkAnnotationDictionary>();

  Annotation::serialize(annot.get());

  if (mExplicitDestination){
    annot->addKey(LinkAnnotationDictionaryKeyDest, mExplicitDestination->getReference());
    mExplicitDestination->serialize(stream, params);
  }

  annot->addKey(LinkAnnotationDictionaryKeyH, std::make_unique<ObjectName>(annotationHighlightingModeToString(mHighlightMode)));

  annot->serialize(stream, params);

  return annot->getReference();
}

void LinkAnnotation::setDestination(Page const *page, DestinationFit fit, int32_t param0, int32_t param1, int32_t param2, int32_t param3){
  mExplicitDestination = Destination::createExplicitDestination(page, fit, param0, param1, param2, param3);
}

void LinkAnnotation::setHighlighting(AnnotationHighlightingMode mode){
  mHighlightMode = mode;
}

#endif
