#pragma once

#include <stdio.h>
#include <memory>
#include <vector>

#include <Document.h>

namespace pdfout{
  class Page;
  enum PaperSize;
  class OutputStream;
}

namespace kernel{
  class DocumentHeader;
  class DocumentCatalogDictionary;
  class DocumentOutlineDictionary;
  class DocumentMetadataStream;
  class DocumentInformationDictionary;
  class DocumentMarkInformationDictionary;
  class ResourceManager;

  namespace encryption{
    class SecurityHandler;
    class Encoder;
  }

  namespace filter{
    class Filter;
  }

  template <typename T>
  class UserObjectManager;

  template <typename T>
  class UserObjectHash;
}

namespace pdfout{ namespace impl{

  class OutlineImpl;
  class AcroFormImpl;
  class PageTreeImpl;

  class DocumentImpl{
  public:
    DocumentImpl(DocumentVersion = DocumentVersion17);
    ~DocumentImpl(void);

    std::vector<uint8_t> const &getDocumentId(void) const;

    void saveToStream(pdfout::OutputStream *stream) const;
    void saveToFile(pdfout::UString const &fileName) const;

    /**
     * Document information
     */
    void setTitle(pdfout::UString const &title);
    void setAuthor(pdfout::UString const &author);
    void setSubject(pdfout::UString const &subject);
    void setKeywords(pdfout::UString const &keywords);
    void setCreator(pdfout::UString const &creator);
    void setProducer(pdfout::UString const &producer);
    void setCreationDate(pdfout::DateTime const &creationDate);
    void setModificationDate(pdfout::DateTime const &modificationDate);

    /**
     * Metadata
     */
    void setMetadata(pdfout::XMPDocument const &xmp);

    /**
     * Sets compression filters for the document, can be one or more of the ::FilterName values
     */
    void setCompressionFilter(FilterName filter);
    void setCompressionFilters(FilterName *filters, std::size_t count);

    /**
     * Sets font embedding
     */
    void setFontEmbedding(bool isEmbedded);

    /**
     * Sets font subsetting, only for embedded fonts
     */
    void setFontSubsetting(bool isSubsetting);

    // Page interface
    Page *createPage(struct Rectangle const &mediaBox) const;
    Page *createPage(PaperSize mediaBox, PaperOrientation paperOrientation) const;
    void addPage(Page *page);
    void insertPage(uint32_t index, Page *page);
    void removePage(uint32_t index);
    Page *getPage(uint32_t index);
    uint32_t getPageCount(void) const;

    // Image interface
    Image *createImage(pdfout::UString const &fileName, bool delayLoad = true);
    Image *createImage(void *imageData, uint64_t size);

    // Controls interface
    //PushButton *createPushButton(struct Rectangle const &rect);

    // Encryption interface
    void setEncryption(EncryptionAlgorithm encryptionAlgorithm, pdfout::UString const &userPassword, pdfout::UString const &ownerPassword, uint32_t userAccess, bool encryptMetadata = true);

    // Produce a PDF/A compliance document
    void setPDFACompliance(bool isCompliance);

  protected:
    std::unique_ptr<kernel::filter::Filter> createFilterByName(FilterName filterName) const;
    std::unique_ptr<kernel::filter::Filter> createFilter(std::vector<FilterName> const &filterNames) const;
    std::vector<std::string> createFilterName(std::vector<FilterName> const &filterNames) const;

  private:
    std::vector<uint8_t> mDocumentId;
    std::unique_ptr<kernel::DocumentHeader> mDocumentHeader;
    std::unique_ptr<kernel::DocumentCatalogDictionary> mDocumentCatalog;
    std::unique_ptr<kernel::DocumentMetadataStream> mDocumentMetadataStream;
    std::unique_ptr<kernel::DocumentInformationDictionary> mDocumentInformation;
    std::unique_ptr<kernel::DocumentMarkInformationDictionary> mDocumentMarkInformation;

    std::unique_ptr<kernel::DocumentOutlineDictionary> mOutlineRoot;
    std::unique_ptr<OutlineImpl> mOutline;

    std::unique_ptr<kernel::ResourceManager> mResourceManager;
    std::unique_ptr<PageTreeImpl> mPageTree;
    std::unique_ptr<AcroFormImpl> mAcroForm;

    std::unique_ptr<kernel::UserObjectManager<Image>> mImageManager;
    std::unique_ptr<kernel::UserObjectHash<Image>> mImages;
    std::vector<FilterName> mCompressionFilter;

    std::unique_ptr<kernel::encryption::SecurityHandler> mSecurityHandler;
    std::unique_ptr<kernel::encryption::Encoder> mEncoder;

    bool mIsFontEmbedded;
    bool mIsFontSubsetting;
    bool mIsMetadataEncrypted;
    bool mIsPDFACompliance;
  };

}}
