#include <AcroFormImpl.h>

#include <memory>

#include <ResourceManager.h>
#include <UserObjectManager.h>

#include <object/ObjectArray.h>
#include <object/ObjectIndirectReference.h>
#include <document/DocumentCrossReferenceTable.h>
#include <interactive/acroform/AcroFormDictionary.h>
#include <interactive/acroform/AcroFormFieldDictionary.h>

#include <controls/Control.h>
#include <controls/PushButton.h>

using namespace kernel;
using namespace kernel::interactive;

namespace pdfout{ namespace impl{

  AcroFormImpl::AcroFormImpl(kernel::ResourceManager *resourceManager)
    : AcroForm(),
    mAccessMutex(),
    mRef(Indirectable::generateReference()),
    mResourceManager(resourceManager){
    //mControlManager(){
  }

  AcroFormImpl::~AcroFormImpl(void) {}

  void AcroFormImpl::serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const{
    std::lock_guard<std::mutex> lock(mAccessMutex);

    std::unique_ptr<ObjectArray> fields = std::make_unique<ObjectArray>(IndirectTypeNonIndirectable);
    //for (auto const &elem : mControlManager){
    //  std::unique_ptr<ObjectIndirectReference> ref(elem->serialize(stream, params));
    //  fields->insert(std::move(ref));
    //}

    std::unique_ptr<AcroFormDictionary> acroFormDict = std::make_unique<AcroFormDictionary>();
    acroFormDict->setReference(mRef.get());
    acroFormDict->addKey(AcroFormDictionaryKeyDR, mResourceManager->getReference());
    acroFormDict->addKey(AcroFormDictionaryKeyFields, std::move(fields));

    acroFormDict->serialize(stream, params);
  }

  std::unique_ptr<kernel::ObjectIndirectReference> AcroFormImpl::getReference(void) const{
    return mRef->getReference();
  }

  //PushButton *AcroFormImpl::createPushButton(struct Rectangle const &rect){
  //  std::unique_ptr<Control> ctrl(new PushButton(mResourceManager, rect));
  //  PushButton *rawControl = (PushButton*) ctrl.get();  

  //  std::lock_guard<std::mutex> lock(mAccessMutex);
  //  mControlManager.push_back(std::move(ctrl));
  //  return rawControl;
  //}

}}
