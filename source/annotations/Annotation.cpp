#ifdef __NOT_IMPLEMENTED___

#include <annotations/Annotation.h>

#include <unordered_map>

#include <FormXObject.h>
#include <InternalTools.h>
#include <object/ObjectInteger.h>
#include <object/ObjectRectangle.h>
#include <object/ObjectUTF16BETextString.h>
#include <interactive/annotation/AnnotationDictionary.h>

using namespace kernel;
using namespace kernel::interactive;

std::string const &annotationHighlightingModeToString(AnnotationHighlightingMode mode){
  static std::unordered_map<AnnotationHighlightingMode, std::string> const highlightModeToString{
    {AnnotationHighlightingModeNone,    "None"},
    {AnnotationHighlightingModeInvert,  "Invert"},
    {AnnotationHighlightingModeOutline, "Outline"},
    {AnnotationHighlightingModePush,    "Push"}
  };

  auto iter = highlightModeToString.find(mode);
  if (iter != std::end(highlightModeToString))
    return iter->second;

  RUNTIME_EXCEPTION("unknown AnnotationHighlightingMode value");
}

Annotation::Annotation(kernel::ResourceManager *resourceManager, struct Rectangle const &rect, pdfout::UString const &name)
  : mResourceManager(resourceManager),
  mRect(rect),
  mName(name),
  mContents(""),
  mFlags(0){
}

Annotation::~Annotation(void) {}

//std::unique_ptr<kernel::ObjectIndirectReference> Annotation::getReference(void) const{
//  return mRef->getReference();
//}

struct Rectangle Annotation::getRect(void) const{
  return mRect;
}

void Annotation::setContents(pdfout::UString const &contents){
  mContents = contents;
}

void Annotation::setFlags(uint32_t flags){
  mFlags = flags;
}

void Annotation::serialize(kernel::interactive::AnnotationDictionary *annotationDictionary) const{
  annotationDictionary->addKey(AnnotationDictionaryKeyRect, std::make_unique<ObjectRectangle>(mRect));
  annotationDictionary->addKey(AnnotationDictionaryKeyNM, std::make_unique<ObjectUTF16BETextString>(mName));
  annotationDictionary->addKey(AnnotationDictionaryKeyContents, std::make_unique<ObjectUTF16BETextString>(mContents));
  annotationDictionary->addKey(AnnotationDictionaryKeyF, std::make_unique<ObjectInteger>(mFlags));
}

#endif
