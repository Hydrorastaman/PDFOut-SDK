#include <Document.h>
#include <DocumentImpl.h>

#include <stream/SimpleInputFileStream.h>

namespace pdfout{
  Document::Document(void)
    : mDocumentImpl(new impl::DocumentImpl()){
  }

  Document::~Document(void){
    if (mDocumentImpl){
      delete mDocumentImpl;
      mDocumentImpl = nullptr;
    }
  }

  void Document::saveToStream(pdfout::OutputStream *stream) const{
    if (mDocumentImpl)
      return mDocumentImpl->saveToStream(stream);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Document::saveToFile(pdfout::UString const &fileName) const{
    if (mDocumentImpl)
      return mDocumentImpl->saveToFile(fileName);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Document::setTitle(pdfout::UString const &title){
    if (mDocumentImpl)
      return mDocumentImpl->setTitle(title);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Document::setAuthor(pdfout::UString const &author){
    if (mDocumentImpl)
      return mDocumentImpl->setAuthor(author);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Document::setSubject(pdfout::UString const &subject){
    if (mDocumentImpl)
      return mDocumentImpl->setSubject(subject);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Document::setKeywords(pdfout::UString const &keywords){
    if (mDocumentImpl)
      return mDocumentImpl->setKeywords(keywords);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Document::setCreator(pdfout::UString const &creator){
    if (mDocumentImpl)
      return mDocumentImpl->setCreator(creator);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Document::setProducer(pdfout::UString const &producer){
    if (mDocumentImpl)
      return mDocumentImpl->setProducer(producer);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Document::setCreationDate(pdfout::DateTime const &creationDate){
    if (mDocumentImpl)
      return mDocumentImpl->setCreationDate(creationDate);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Document::setModificationDate(pdfout::DateTime const &modificationDate){
    if (mDocumentImpl)
      return mDocumentImpl->setModificationDate(modificationDate);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }


  void Document::setMetadata(pdfout::XMPDocument const &xmp){
    if (mDocumentImpl)
      return mDocumentImpl->setMetadata(xmp);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Document::setCompressionFilter(FilterName filter){
    if (mDocumentImpl)
      return mDocumentImpl->setCompressionFilter(filter);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Document::setCompressionFilters(FilterName *filters, std::size_t count){
    if (mDocumentImpl)
      return mDocumentImpl->setCompressionFilters(filters, count);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Document::setFontEmbedding(bool isEmbedded){
    if (mDocumentImpl)
      return mDocumentImpl->setFontEmbedding(isEmbedded);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Document::setFontSubsetting(bool isSubsetting){
    if (mDocumentImpl)
      return mDocumentImpl->setFontSubsetting(isSubsetting);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  Page *Document::createPage(struct Rectangle const &mediaBox) const{
    if (mDocumentImpl)
      return mDocumentImpl->createPage(mediaBox);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  Page *Document::createPage(PaperSize mediaBox, PaperOrientation paperOrientation) const{
    if (mDocumentImpl)
      return mDocumentImpl->createPage(mediaBox, paperOrientation);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Document::addPage(Page *page){
    if (mDocumentImpl)
      return mDocumentImpl->addPage(page);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Document::insertPage(uint32_t index, Page *page){
    if (mDocumentImpl)
      return mDocumentImpl->insertPage(index, page);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Document::removePage(uint32_t index){
    if (mDocumentImpl)
      return mDocumentImpl->removePage(index);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  Page *Document::getPage(uint32_t index){
    if (mDocumentImpl)
      return mDocumentImpl->getPage(index);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  uint32_t Document::getPageCount(void) const{
    if (mDocumentImpl)
      return mDocumentImpl->getPageCount();

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  Image *Document::createImage(pdfout::UString const &fileName, bool delayLoad){
    if (mDocumentImpl)
      return mDocumentImpl->createImage(fileName, delayLoad);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  Image *Document::createImage(void *imageData, uint64_t size){
    if (mDocumentImpl)
      return mDocumentImpl->createImage(imageData, size);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Document::setEncryption(EncryptionAlgorithm encryptionAlgorithm, pdfout::UString const &userPassword, pdfout::UString const &ownerPassword, uint32_t userAccess, bool encryptMetadata){
    if (mDocumentImpl)
      return mDocumentImpl->setEncryption(encryptionAlgorithm, userPassword, ownerPassword, userAccess, encryptMetadata);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Document::setPDFACompliance(bool isCompliance){
    if (mDocumentImpl)
      return mDocumentImpl->setPDFACompliance(isCompliance);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

}
