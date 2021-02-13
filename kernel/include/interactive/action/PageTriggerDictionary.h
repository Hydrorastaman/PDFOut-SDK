#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace interactive{

  enum PageTriggerDictionaryKey{
    PageTriggerDictionaryKeyO, /**< (Optional; PDF 1.2) An action to be performed when the page is opened (for example, when the user navigates to it from the next or previous page or by means of a link annotation or outline item). This action is independent of any that may be defined by the OpenAction entry in the document catalog and is executed after such an action. */
    PageTriggerDictionaryKeyC  /**< (Optional; PDF 1.2) An action to be performed when the page is closed (for example, when the user navigates to the next or previous page or follows a link annotation or an outline item). This action applies to the page being closed and is executed before any other page is opened. */
  };

  /**
   * @brief Annotation dictionary
   */
  class PageTriggerDictionary : public ObjectDictionary{
  public:
    PageTriggerDictionary(void);
    ~PageTriggerDictionary(void);

    void addKey(PageTriggerDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<PageTriggerDictionaryKey, std::pair<std::string, uint32_t>> mPageTriggerDictionaryMap;

  private:
    PageTriggerDictionary(PageTriggerDictionary const &) = delete;
    PageTriggerDictionary &operator=(PageTriggerDictionary const &) = delete;
  };

}}
