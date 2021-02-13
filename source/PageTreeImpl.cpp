#include <PageTreeImpl.h>

#include <PageImpl.h>
#include <ResourceManager.h>
#include <UserObjectManager.h>

#include <document/DocumentPage.h>
#include <document/DocumentPages.h>
#include <document/DocumentCrossReferenceTable.h>

#include <object/ObjectArray.h>
#include <object/ObjectInteger.h>
#include <object/ObjectIndirectReference.h>

using namespace kernel;

namespace pdfout{ namespace impl{

  PageTreeImpl::PageTreeImpl(ResourceManager *resourceManager)
    : mAccessMutex(),
    mPagesManager(std::make_unique<UserObjectManager<PageImpl>>()),
    mDocumentPages(),
    mCount(0),
    mResourceManager(resourceManager),
    mRef(Indirectable::generateReference()){
  }

  PageTreeImpl::~PageTreeImpl(void) {}

  std::unique_ptr<kernel::ObjectIndirectReference> PageTreeImpl::getReference(void) const{
    return mRef->getReference();
  }

  PageImpl *PageTreeImpl::createPage(void) const{
    std::unique_ptr<PageImpl> page(std::make_unique<PageImpl>(mResourceManager));
    return mPagesManager->insert(std::move(page));
  }

  void PageTreeImpl::addPage(PageImpl *page){
    std::lock_guard<std::mutex> lock(mAccessMutex);
    mDocumentPages.push_back(page);
    ++mCount;
  }

  void PageTreeImpl::insertPage(uint32_t index, PageImpl *page){
    std::lock_guard<std::mutex> lock(mAccessMutex);

    if (index >= mDocumentPages.size())
      mDocumentPages.resize(index + 1);

    if (mDocumentPages[index])
      --mCount;

    mDocumentPages[index] = page;
    ++mCount;
  }

  void PageTreeImpl::removePage(uint32_t index){
    std::lock_guard<std::mutex> lock(mAccessMutex);

    if (index + 1 > mDocumentPages.size())
      return;

    if (mDocumentPages[index]){
      mDocumentPages[index] = nullptr;
      --mCount;
    }
  }

  PageImpl *PageTreeImpl::getPage(uint32_t index){
    std::lock_guard<std::mutex> lock(mAccessMutex);

    if (mDocumentPages.size() > index)
      return mDocumentPages[index];

    return nullptr;
  }

  uint32_t PageTreeImpl::getPageCount(void) const{
    return mCount;
  }

  void PageTreeImpl::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
    std::lock_guard<std::mutex> lock(mAccessMutex);

    std::unique_ptr<ObjectArray> kids = std::make_unique<ObjectArray>(IndirectTypeNonIndirectable);

    for (auto const &iter : mDocumentPages)
      if (iter)
        kids->insert(iter->getReference());

    std::unique_ptr<kernel::DocumentPagesDictionary> documentPages = std::make_unique<kernel::DocumentPagesDictionary>();
    documentPages->setReference(mRef.get());

    documentPages->addKey(DocumentPagesDictionaryKeyCount, std::make_unique<ObjectInteger>(kids->getSize()));
    documentPages->addKey(DocumentPagesDictionaryKeyKids, std::move(kids));
    documentPages->addKey(DocumentPagesDictionaryKeyResources, mResourceManager->getReference());

    documentPages->serialize(stream, params);

    for (auto const &iter : mDocumentPages)
      if (iter){
        iter->setParent(documentPages.get());
        iter->serialize(stream, params);
      }
  }
}}
