#pragma once

#include <unordered_map>
#include <string>

#include <object/ObjectDictionary.h>

namespace kernel{

  enum PageLayout{
    PageLayoutSinglePage,     /**< Display one page at a time */
    PageLayoutOneColumn,      /**< Display the pages in one column */
    PageLayoutTwoColumnLeft,  /**< Display the pages in two columns, with odd_numbered pages on the left */
    PageLayoutTwoColumnRight, /**< Display the pages in two columns, with odd_numbered pages on the right */
    PageLayoutTwoPageLeft,    /**< (PDF 1.5) Display the pages two at a time, with odd_numbered pages on the left */
    PageLayoutTwoPageRight    /**< (PDF 1.5) Display the pages two at a time, with odd_numbered pages on the right */
  };

  enum PageMode{
    PageModeUseNone,       /**< Neither document outline nor thumbnail images visible */
    PageModeUseOutlines,   /**< Document outline visible */
    PageModeUseThumbs,     /**< Thumbnail images visible */
    PageModeFullScreen,    /**< Full_screen mode, with no menu bar, window controls, or any other window visible */
    PageModeUseOC,         /**< (PDF 1.5) Optional content group panel visible */
    PageModeUseAttachments /**< (PDF 1.6) Attachments panel visible */
  };

  enum CatalogDictionaryKey{
    CatalogDictionaryKeyVersion,        // Optional; PDF 1.4
    CatalogDictionaryKeyPages,          // Required; must be an indirect reference
    CatalogDictionaryKeyPageLabels,     // Optional; PDF 1.3
    CatalogDictionaryKeyNames,          // Optional; PDF 1.2
    CatalogDictionaryKeyDests,          // Optional; PDF 1.1; must be an indirect reference
    CatalogDictionaryKeyViewerPreferen, // Optional; PDF 1.2
    CatalogDictionaryKeyPageLayout,     // Optional
    CatalogDictionaryKeyPageMode,       // Optional
    CatalogDictionaryKeyOutlines,       // Optional; must be an indirect reference
    CatalogDictionaryKeyThreads,        // Optional; PDF 1.1; must be an indirect reference
    CatalogDictionaryKeyOpenAction,     // Optional; PDF 1.1
    CatalogDictionaryKeyAA,             // Optional; PDF 1.4
    CatalogDictionaryKeyURI,            // Optional; PDF 1.1
    CatalogDictionaryKeyAcroForm,       // Optional; PDF 1.2
    CatalogDictionaryKeyMetadata,       // Optional; PDF 1.4; must be an indirect reference
    CatalogDictionaryKeyStructTreeRoot, // Optional; PDF 1.3
    CatalogDictionaryKeyMarkInfo,       // Optional; PDF 1.4
    CatalogDictionaryKeyLang,           // Optional; PDF 1.4
    CatalogDictionaryKeySpiderInfo,     // Optional; PDF 1.3
    CatalogDictionaryKeyOutputIntents,  // Optional; PDF 1.4
    CatalogDictionaryKeyPieceInfo,      // Optional; PDF 1.4
    CatalogDictionaryKeyOCProperties,   // Optional; PDF 1.5; required if a document contains optional content
    CatalogDictionaryKeyPerms,          // Optional; PDF 1.5
    CatalogDictionaryKeyLegal,          // Optional; PDF 1.5
    CatalogDictionaryKeyRequirements,   // Optional; PDF 1.7
    CatalogDictionaryKeyCollection,     // Optional; PDF 1.7
    CatalogDictionaryKeyNeedsRendering  // Optional; PDF 1.7
  };

  class DocumentCatalogDictionary : public ObjectDictionary{
  private:
    static PageLayout const defaultPageLayout = PageLayoutSinglePage;
    static PageMode const defaultPageMode = PageModeUseNone;

  public:
    DocumentCatalogDictionary(void);
    ~DocumentCatalogDictionary(void) {}

    void addKey(CatalogDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<CatalogDictionaryKey, std::pair<std::string, uint32_t>> mCatalogDictionaryMap;

  private:
    DocumentCatalogDictionary(DocumentCatalogDictionary const &) = delete;
    DocumentCatalogDictionary &operator=(DocumentCatalogDictionary const &) = delete;
  };

}
