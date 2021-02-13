#include <DocumentImpl.h>

#include <PageImpl.h>
#include <ImageImpl.h>
#include <OutlineImpl.h>
#include <PageTreeImpl.h>
#include <AcroFormImpl.h>
#include <ResourceManager.h>

#include <ximage.h>

#include <object/ObjectDate.h>
#include <object/ObjectArray.h>
#include <object/ObjectBoolean.h>
#include <object/ObjectInteger.h>
#include <object/ObjectByteString.h>
#include <object/ObjectUTF16BETextString.h>

#include <document/DocumentPages.h>
#include <document/DocumentHeader.h>
#include <document/DocumentFileTrailer.h>
#include <document/DocumentMetadataStream.h>
#include <document/DocumentOutlineDictionary.h>
#include <document/DocumentCatalogDictionary.h>
#include <document/DocumentCrossReferenceTable.h>
#include <document/DocumentInformationDictionary.h>
#include <document/DocumentMarkInformationDictionary.h>

#include <stream/OutputStream.h>
#include <stream/MemoryOStream.h>
#include <stream/SimpleOutputFileStream.h>

#include <encryption/EncoderRc4.h>
#include <encryption/EncoderAes2.h>
#include <encryption/EncoderAes3.h>
#include <encryption/CryptFilter.h>
#include <encryption/SecurityHandler.h>
#include <encryption/StandardSecurityHandler.h>

#include <filter/FilterAscii85.h>
#include <filter/FilterDeflate.h>
#include <filter/FilterAsciiHex.h>

#include <ToString.h>
#include <Exception.h>
#include <UserObjectHash.h>
#include <UserObjectManager.h>

namespace{
  std::string const trailerStartXref = "startxref\r\n";
  std::string const trailerEOF = "%%EOF\r\n";
}

using namespace kernel;
using namespace kernel::filter;
using namespace kernel::encryption;
using namespace kernel::interactive;

namespace pdfout{ namespace impl{

  DocumentImpl::DocumentImpl(DocumentVersion documentVersion)
    : mDocumentId(getUIID()),
    mDocumentHeader(std::make_unique<DocumentHeader>(documentVersion)),
    mDocumentCatalog(std::make_unique<DocumentCatalogDictionary>()),
    mDocumentMetadataStream(std::make_unique<DocumentMetadataStream>()),
    mDocumentInformation(std::make_unique<DocumentInformationDictionary>()),
    mDocumentMarkInformation(std::make_unique<DocumentMarkInformationDictionary>()),
    mOutlineRoot(std::make_unique<DocumentOutlineDictionary>()),
    mOutline(std::make_unique<OutlineImpl>(mOutlineRoot.get())),
    mResourceManager(std::make_unique<ResourceManager>()),
    mPageTree(std::make_unique<PageTreeImpl>(mResourceManager.get())),
    mAcroForm(std::make_unique<AcroFormImpl>(mResourceManager.get())),
    mImageManager(std::make_unique<UserObjectManager<Image>>()),
    mImages(std::make_unique<UserObjectHash<Image>>()),
    mCompressionFilter{},
    mSecurityHandler(nullptr),
    mEncoder(nullptr),
    mIsFontEmbedded(false),
    mIsFontSubsetting(false),
    mIsMetadataEncrypted(true),
    mIsPDFACompliance(false){

    mDocumentCatalog->addKey(CatalogDictionaryKeyPages, mPageTree->getReference());
    mDocumentCatalog->addKey(CatalogDictionaryKeyOutlines, mOutlineRoot->getReference());
    mDocumentCatalog->addKey(CatalogDictionaryKeyAcroForm, mAcroForm->getReference());
    mDocumentCatalog->addKey(CatalogDictionaryKeyMarkInfo, mDocumentMarkInformation->getReference());

    mDocumentMarkInformation->addKey(MarkInformationDictionaryKeyMarked, std::make_unique<ObjectBoolean>(true));

    mResourceManager->addProcSet(ProcSetPDF);
  }

  DocumentImpl::~DocumentImpl(void){
  }

  std::vector<uint8_t> const &DocumentImpl::getDocumentId(void) const{
    return mDocumentId;
  }

  void DocumentImpl::saveToStream(pdfout::OutputStream *stream) const{

    std::unique_ptr<SerializeParams> params = std::make_unique<SerializeParams>();
    std::unique_ptr<DocumentCrossReferenceTable> xref(std::make_unique<DocumentCrossReferenceTable>());

    params->mXref = xref.get();

    // compression filter
    auto compressionFilter = createFilter(mCompressionFilter);
    params->mCompressionFilter = compressionFilter.get();
    params->mCompressionFilterName = createFilterName(mCompressionFilter);

    // font embedding
    params->mIsFontEmbedded = mIsFontEmbedded;

    // font subsetting
    params->mIsFontSubsetting = mIsFontEmbedded && mIsFontSubsetting;

    // encryption filter for strings
    params->mStrEncoder = mEncoder.get();
    params->mStmEncoder = mEncoder.get();
    params->mFileEncoder = nullptr;

    if (mIsPDFACompliance){
      params->mIsFontEmbedded = true;
    }

    SerializeParams *paramsPtr = params.get();

    mDocumentHeader->serialize(stream, paramsPtr);
    mDocumentInformation->serialize(stream, paramsPtr);
    mDocumentMarkInformation->serialize(stream, paramsPtr);
    mPageTree->serialize(stream, paramsPtr);
    mOutline->serialize(stream, paramsPtr);
    mAcroForm->serialize(stream, paramsPtr);
    mResourceManager->serialize(stream, paramsPtr);

    if (mDocumentMetadataStream->getStream()->getSize()){
      mDocumentCatalog->addKey(CatalogDictionaryKeyMetadata, mDocumentMetadataStream->getReference());
      auto uncompressedParams = removeCompression(paramsPtr);
      mDocumentMetadataStream->serialize(stream, mIsMetadataEncrypted ? paramsPtr : removeEncryption(uncompressedParams.get()).get());
    }

    mDocumentCatalog->serialize(stream, paramsPtr);

    std::unique_ptr<DocumentFileTrailer> trailer(std::make_unique<DocumentFileTrailer>());

    trailer->addKey(DocumentFileTrailerKeySize, std::make_unique<ObjectInteger>(params->mXref->getHighestNumber() + 1));
    trailer->addKey(DocumentFileTrailerKeyRoot, mDocumentCatalog->getReference());
    trailer->addKey(DocumentFileTrailerKeyInfo, mDocumentInformation->getReference());
    if (mSecurityHandler){
      std::unique_ptr<ObjectIndirectReference> trailerEncryptRef = mSecurityHandler->serialize(stream, paramsPtr);
      trailer->addKey(DocumentFileTrailerKeyEncrypt, std::move(trailerEncryptRef));
    }

    params->mXref->serialize(stream, nullptr);

    std::unique_ptr<ObjectArray> fileId = std::make_unique<ObjectArray>(IndirectTypeNonIndirectable);
    fileId->insert(std::make_unique<ObjectByteString>(mDocumentId.data(), mDocumentId.size()));
    fileId->insert(std::make_unique<ObjectByteString>(mDocumentId.data(), mDocumentId.size()));
    trailer->addKey(DocumentFileTrailerKeyID, std::move(fileId));
    // add FileTrailerPrev is more than one xref table
    // add FileTrailerInfo

    trailer->serialize(stream, paramsPtr);

    stream->write(trailerStartXref.c_str(), sizeof(uint8_t), trailerStartXref.length()); 

    std::string startXref = std::to_string(params->mXref->getStartXref()) + "\r\n";
    stream->write(startXref.c_str(), sizeof(uint8_t), startXref.length());
    stream->write(trailerEOF.c_str(), sizeof(uint8_t), trailerEOF.length());
  }

  void DocumentImpl::saveToFile(pdfout::UString const &fileName) const{
    std::unique_ptr<OutputStream> fileStream(std::make_unique<SimpleOutputFileStream>(fileName));
    saveToStream(fileStream.get());
  }

  void DocumentImpl::setTitle(pdfout::UString const &title){
    mDocumentInformation->addKey(InformationDictionaryKeyTitle, std::make_unique<ObjectUTF16BETextString>(title));
  }

  void DocumentImpl::setAuthor(pdfout::UString const &author){
    mDocumentInformation->addKey(InformationDictionaryKeyAuthor, std::make_unique<ObjectUTF16BETextString>(author));
  }

  void DocumentImpl::setSubject(pdfout::UString const &subject){
    mDocumentInformation->addKey(InformationDictionaryKeySubject, std::make_unique<ObjectUTF16BETextString>(subject));
  }

  void DocumentImpl::setKeywords(pdfout::UString const &keywords){
    mDocumentInformation->addKey(InformationDictionaryKeyKeywords, std::make_unique<ObjectUTF16BETextString>(keywords));
  }

  void DocumentImpl::setCreator(pdfout::UString const &creator){
    mDocumentInformation->addKey(InformationDictionaryKeyCreator, std::make_unique<ObjectUTF16BETextString>(creator));
  }

  void DocumentImpl::setProducer(pdfout::UString const &producer){
    mDocumentInformation->addKey(InformationDictionaryKeyProducer, std::make_unique<ObjectUTF16BETextString>(producer));
  }

  void DocumentImpl::setCreationDate(pdfout::DateTime const &creationDate){
    mDocumentInformation->addKey(InformationDictionaryKeyCreationDate, std::make_unique<ObjectDate>(creationDate));
  }

  void DocumentImpl::setModificationDate(pdfout::DateTime const &modificationDate){
    mDocumentInformation->addKey(InformationDictionaryKeyModDate, std::make_unique<ObjectDate>(modificationDate));
  }

  void DocumentImpl::setMetadata(pdfout::XMPDocument const &xmp){
    auto stream = mDocumentMetadataStream->getStream();
    stream->seekp(0);
    xmp.saveToStream(stream);
  }

  void DocumentImpl::setCompressionFilter(FilterName filter){
    mCompressionFilter = std::vector<FilterName>({filter});
  }

  void DocumentImpl::setCompressionFilters(FilterName *filters, std::size_t count){
    mCompressionFilter = std::vector<FilterName>(filters, filters + count);
  }

  void DocumentImpl::setFontEmbedding(bool isEmbedded){
    mIsFontEmbedded = isEmbedded;
  }

  void DocumentImpl::setFontSubsetting(bool isSubsetting){
    mIsFontSubsetting = isSubsetting;
  }

  Page *DocumentImpl::createPage(struct Rectangle const &mediaBox) const{
    Page *page = mPageTree->createPage();
    page->setMediaBox(mediaBox);
    return page;
  }

  Page *DocumentImpl::createPage(PaperSize mediaBox, PaperOrientation paperOrientation) const{
    return createPage(getPaperBoundaries(mediaBox, paperOrientation));
  }

  void DocumentImpl::addPage(Page *page) {return mPageTree->addPage(dynamic_cast<PageImpl *>(page));}
  void DocumentImpl::insertPage(uint32_t index, Page *page) {return mPageTree->insertPage(index, dynamic_cast<PageImpl *>(page));}
  void DocumentImpl::removePage(uint32_t index) {return mPageTree->removePage(index);}
  Page *DocumentImpl::getPage(uint32_t index) {return mPageTree->getPage(index);}
  uint32_t DocumentImpl::getPageCount(void) const {return mPageTree->getPageCount();}

  Image *DocumentImpl::createImage(pdfout::UString const &fileName, bool delayLoad){
    UString utf16be = fileName.encode(Encoding_UTF16BE);
    std::string const sha256 = getSHA256(utf16be.getData(), utf16be.getSize());
    Image *storedImage = mImages->find(sha256);
    if (storedImage)
      return storedImage;

    std::unique_ptr<Image> image(ImageImpl::rawCreateImageFromFile(fileName, delayLoad));
    Image *loadedImage = mImageManager->insert(std::move(image));
    mImages->store(loadedImage, sha256);
    return loadedImage;
  }

  Image *DocumentImpl::createImage(void *imageData, uint64_t size){
    std::string const sha256 = getSHA256(imageData, size);
    Image *storedImage = mImages->find(sha256);
    if (storedImage)
      return storedImage;
 
    std::unique_ptr<Image> image(ImageImpl::rawCreateImageFromMemory(imageData, size));
    Image *loadedImage = mImageManager->insert(std::move(image));
    mImages->store(loadedImage, sha256);
    return loadedImage;
  }

  //Image *DocumentImpl::createImage(std::unique_ptr<CxImage> source){
  //  std::string const sha256 = getSHA256(source->GetDIB(), source->GetSize());
  //  Image *storedImage = mImages->find(sha256);
  //  if (storedImage)
  //    return storedImage;

  //  std::unique_ptr<Image> image(ImageImpl::rawCreateImageFromCxImage(std::move(source)));
  //  Image *loadedImage = mImageManager->insert(std::move(image));
  //  mImages->store(loadedImage, sha256);
  //  return loadedImage;
  //}

  //PushButton *DocumentImpl::createPushButton(struct Rectangle const &rect) {return mAcroForm->createPushButton(rect);}

  void DocumentImpl::setEncryption(EncryptionAlgorithm encryptionAlgorithm, pdfout::UString const &userPassword, pdfout::UString const &ownerPassword, uint32_t userAccess, bool encryptMetadata){
    CryptFilterCfm cfm = CryptFilterCfmNone;
    uint32_t keyLength = 0;
    mIsMetadataEncrypted = encryptMetadata;

    switch (encryptionAlgorithm){
      case EncryptionAlgorithmNone:
        mSecurityHandler.reset(nullptr);
        mEncoder.reset(nullptr);
        return;

      case EncryptionAlgorithmRC4v1:
        mSecurityHandler = std::make_unique<StandardSecurityHandler>(1, userAccess, 40, getDocumentId(), encryptMetadata, userPassword, ownerPassword);
        mEncoder = std::make_unique<EncoderRc4>(mSecurityHandler->getEncryptionKey());
        return;

      case EncryptionAlgorithmRC4v2:{
        mSecurityHandler = std::make_unique<StandardSecurityHandler>(4, userAccess, 128, getDocumentId(), encryptMetadata, userPassword, ownerPassword);
        mEncoder = std::make_unique<EncoderRc4>(mSecurityHandler->getEncryptionKey());
        cfm = CryptFilterCfmV2;
        keyLength = 128;
        break;
      }

      case EncryptionAlgorithmAESv2:{
        mSecurityHandler = std::make_unique<StandardSecurityHandler>(4, userAccess, 128, getDocumentId(), encryptMetadata, userPassword, ownerPassword);
        mEncoder = std::make_unique<EncoderAes2>(mSecurityHandler->getEncryptionKey());
        cfm = CryptFilterCfmAESV2;
        keyLength = 128;
        break;
      }

      case EncryptionAlgorithmAESv3:{
        mSecurityHandler = std::make_unique<StandardSecurityHandler>(5, userAccess, 256, getDocumentId(), encryptMetadata, userPassword, ownerPassword);
        mEncoder = std::make_unique<EncoderAes3>(mSecurityHandler->getEncryptionKey());
        cfm = CryptFilterCfmAESV3;
        keyLength = 256;
        break;
      }

      default:
        RUNTIME_EXCEPTION("Invalid EncryptionAlgorithm value");
    }

    auto stdCF = CryptFilter::makeStandardCF(cfm, keyLength);
    std::string const cfName = stdCF->getName();
    mSecurityHandler->addCryptFilter(std::move(stdCF));
    mSecurityHandler->setStrF(cfName);
    mSecurityHandler->setStmF(cfName);
  }

  void DocumentImpl::setPDFACompliance(bool isCompliance){
    mIsPDFACompliance = isCompliance;
  }

  std::unique_ptr<filter::Filter> DocumentImpl::createFilterByName(FilterName filterName) const{
    switch (filterName){
      case FilterNameASCIIHex: return std::make_unique<FilterAsciiHex>();
      case FilterNameASCII85: return std::make_unique<FilterAscii85>();
      case FilterNameLZW: return nullptr;
      case FilterNameFlate: return std::make_unique<FilterDeflate>();
      case FilterNameRunLength: return nullptr;
      case FilterNameCCITTFax: return nullptr;
      case FilterNameJBIG2: return nullptr;
      case FilterNameDCT: return nullptr;
      case FilterNameJPX: return nullptr;
    }

    return nullptr;
  }

  std::unique_ptr<filter::Filter> DocumentImpl::createFilter(std::vector<FilterName> const &filterNames) const{
    if (filterNames.empty())
      return nullptr;

    std::unique_ptr<filter::Filter> baseFilter = createFilterByName(filterNames[0]);

    std::size_t const size = filterNames.size();
    if (size > 1){
      for (std::size_t i = 1; i < size; ++i){
        std::unique_ptr<Filter> subFilter(createFilterByName(filterNames[i]));
        subFilter->attach(std::move(baseFilter));
        baseFilter = std::move(subFilter);
      }
    }

    return baseFilter;
  }

  std::vector<std::string> DocumentImpl::createFilterName(std::vector<FilterName> const &filterNames) const{
    std::vector<std::string> filterName;
    for (auto const &elem : filterNames)
      filterName.push_back(toString(elem));

    return filterName;
  }

}}
