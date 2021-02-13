#include <PageImpl.h>

#include <ResourceManager.h>
#include <document/DocumentPage.h>
#include <document/DocumentPages.h>
#include <document/DocumentCrossReferenceTable.h>

#include <object/ObjectArray.h>
#include <object/ObjectNumber.h>
#include <object/ObjectStream.h>
#include <object/ObjectInteger.h>
#include <object/ObjectRectangle.h>
#include <object/ObjectIndirectReference.h>

using namespace kernel;

namespace pdfout{ namespace impl{

  PageImpl::PageImpl(ResourceManager *resourceManager)
    : Page(),
    mPage(std::make_unique<DocumentPage>()),
    mResourceManager(resourceManager),
    mContentStream(std::make_unique<ContentStreamImpl>(mResourceManager)),
    mControlsRef(){
    //mAnnotations(){
  }

  PageImpl::~PageImpl(void) {}

  void PageImpl::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
    if (mContentStream){
      mPage->addKey(DocumentPageKeyContents, std::make_unique<ObjectStream>());
      ObjectStream *stream = (ObjectStream *) mPage->find(DocumentPageKeyContents);
      mContentStream->serialize(stream);
    }

    // serialize controls and annotations
    std::unique_ptr<ObjectArray> annots = std::make_unique<ObjectArray>(IndirectTypeNonIndirectable);

    for (auto const &elem : mControlsRef)
      annots->insert(elem->getReference());

    //for (auto const &elem : mAnnotations){
    //  std::unique_ptr<ObjectIndirectReference> ref(elem->serialize(stream, params));
    //  annots->insert(std::move(ref));
    //}

    if (!annots->isEmpty())
      mPage->addKey(DocumentPageKeyAnnots, std::move(annots));

    mPage->serialize(stream, params);
  }

  std::unique_ptr<kernel::ObjectIndirectReference> PageImpl::getReference(void) const{
    return mPage->getReference();
  }

  void PageImpl::setParent(kernel::DocumentPagesDictionary *parent){
    mPage->addKey(DocumentPageKeyParent, parent->getReference());
  }

  void PageImpl::setMediaBox(Rectangle const &mediaBox){
    mPage->addKey(DocumentPageKeyMediaBox, std::make_unique<ObjectRectangle>(mediaBox));
  }

  bool PageImpl::getMediaBox(Rectangle &mediaBox) const{
    return getBox(DocumentPageKeyMediaBox, mediaBox);
  }

  void PageImpl::setCropBox(Rectangle const &cropBox){
    mPage->addKey(DocumentPageKeyCropBox, std::make_unique<ObjectRectangle>(cropBox));
  }

  bool PageImpl::getCropBox(Rectangle &cropBox) const{
    if (!getBox(DocumentPageKeyCropBox, cropBox))
      return getMediaBox(cropBox);

    return true;
  }

  void PageImpl::setBleedBox(Rectangle const &bleedBox){
    mPage->addKey(DocumentPageKeyBleedBox, std::make_unique<ObjectRectangle>(bleedBox));
  }

  bool PageImpl::getBleedBox(Rectangle &bleedBox) const{
    if (!getBox(DocumentPageKeyBleedBox, bleedBox))
      return getCropBox(bleedBox);

    return true;
  }

  void PageImpl::setTrimBox(Rectangle const &trimBox){
    mPage->addKey(DocumentPageKeyTrimBox, std::make_unique<ObjectRectangle>(trimBox));
  }

  bool PageImpl::getTrimBox(Rectangle &trimBox) const{
    if (!getBox(DocumentPageKeyTrimBox, trimBox))
      return getCropBox(trimBox);

    return true;
  }

  void PageImpl::setArtBox(Rectangle const &artBox){
    mPage->addKey(DocumentPageKeyArtBox, std::make_unique<ObjectRectangle>(artBox));
  }

  bool PageImpl::getArtBox(Rectangle &artBox) const{
    if (!getBox(DocumentPageKeyArtBox, artBox))
      return getCropBox(artBox);

    return true;
  }

  ContentStream *PageImpl::getContents(void) const{
    return mContentStream.get();
  }

  void PageImpl::setRotate(PageRotate rotate){
    mPage->addKey(DocumentPageKeyRotate, std::make_unique<ObjectInteger>(rotate));
  }

  PageRotate PageImpl::getRotate(void) const{
    ObjectInteger *rotate = dynamic_cast<ObjectInteger *>(mPage->find(DocumentPageKeyRotate));
    if (rotate != nullptr){
      switch (rotate->getValue()){
        case 0 : return PageRotate0;
        case 90 : return PageRotate90;
        case 180 : return PageRotate180;
        case 270 : return PageRotate270;
      }
    }

    return PageRotate0;
  }

  void PageImpl::setDur(float duration){
    mPage->addKey(DocumentPageKeyDur, std::make_unique<ObjectNumber>(duration));
  }

  bool PageImpl::getDur(float &duration) const{
    return getNumber(DocumentPageKeyDur, duration);
  }

  void PageImpl::setMetadata(XMPDocument const &xmp){
    auto metadataStream = std::make_unique<DocumentMetadataStream>();
    xmp.saveToStream(metadataStream->getStream());
    mPage->addKey(DocumentPageKeyMetadata, std::move(metadataStream));
  }

  void PageImpl::setUserUnit(float userUnit){
    mPage->addKey(DocumentPageKeyUserUnit, std::make_unique<ObjectNumber>(userUnit));
  }

  bool PageImpl::getUserUnit(float &userUnit) const{
    return getNumber(DocumentPageKeyUserUnit, userUnit);
  }

  bool PageImpl::getBox(DocumentPageKey key, Rectangle &box) const{
    ObjectRectangle *rect = dynamic_cast<ObjectRectangle *>(mPage->find(key));
    if (rect != nullptr){
      box = rect->getValue();
      return true;
    }

    return false;
  }

  bool PageImpl::getNumber(kernel::DocumentPageKey key, float &value)   const{
    ObjectNumber *obj = dynamic_cast<ObjectNumber *>(mPage->find(key));
    if (obj != nullptr){
      value = obj->getValue();
      return true;
    }

    return false;
  }

  //void PageImpl::addControl(Control *control){
  //  mControlsRef.push_back(std::move(control->getReference()));
  //}

  //TextAnnotation *PageImpl::addTextAnnotation(struct Rectangle const &rect, pdfout::UString const &name){
  //  std::unique_ptr<TextAnnotation> annot(new TextAnnotation(mResourceManager, rect, name));
  //  return (TextAnnotation *) addAnnotation(std::move(annot));
  //}

  //LinkAnnotation *PageImpl::addLinkAnnotation(struct Rectangle const &rect, pdfout::UString const &name){
  //  std::unique_ptr<LinkAnnotation> annot(new LinkAnnotation(mResourceManager, rect, name));
  //  return (LinkAnnotation *) addAnnotation(std::move(annot));
  //}

  //Annotation *PageImpl::addAnnotation(std::unique_ptr<Annotation> annot){
  //  Annotation *ptr = annot.get();
  //  mAnnotations.push_back(std::move(annot));

  //  return ptr;
  //}

}}
