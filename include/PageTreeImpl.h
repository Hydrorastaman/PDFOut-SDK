#pragma once

#include <deque>
#include <memory>
#include <mutex>

namespace kernel{
  class DocumentPage;
  struct SerializeParams;
  class ResourceManager;
  class ObjectIndirectReference;

  template <typename T>
  class UserObjectManager;

}

namespace pdfout{
  class OutputStream;
}

namespace pdfout{ namespace impl{

  class PageImpl;

  /**
   * @brief Page tree implementation
   */
  class PageTreeImpl{
  public:
    explicit PageTreeImpl(kernel::ResourceManager *resourceManager);
    ~PageTreeImpl(void);

    std::unique_ptr<kernel::ObjectIndirectReference> getReference(void) const;

    /**
     * Creates new blank page
     * @return Blank page
     */
    PageImpl *createPage(void) const;

    /**
     * Adds new page to the end of document and gets the ownership on the object
     * @param [in] page new page
     */
    void addPage(PageImpl *page);

    /**
     * Inserts new page at selected index and gets the ownership on the object
     * @param [in] index zero_based page index
     * @param [in] page new page
     */
    void insertPage(uint32_t index, PageImpl *page);

    /**
     * Removes page at selected index
     * @params [in] index zero_based page index
     */
    void removePage(uint32_t index);

    /**
     * Gets page at selected index
     * @param [in] index zero_based page index
     * @return If page with selected index exists, returns required page.
     * @return If page with selected index absent, returns nullptr.
     */
    PageImpl *getPage(uint32_t index);

    /**
     * Gets page count
     * @return Page count
     */
    uint32_t getPageCount(void) const;

    void serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

  private:
    PageTreeImpl(PageTreeImpl const &) = delete;
    PageTreeImpl &operator=(PageTreeImpl const &) = delete;

  private:
    mutable std::mutex mAccessMutex;
    mutable std::unique_ptr<kernel::UserObjectManager<PageImpl>> mPagesManager;
    std::deque<PageImpl *> mDocumentPages;
    uint32_t mCount;
    kernel::ResourceManager *mResourceManager;
    std::unique_ptr<kernel::ObjectIndirectReference> mRef;
  };

}}
