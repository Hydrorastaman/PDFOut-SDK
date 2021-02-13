#pragma once

#include <interactive/action/ActionDictionary.h>

namespace kernel{ namespace interactive{

  enum GoToActionDictionaryKey{
    GoToActionDictionaryKeyD /**< (Required) The destination to jump to. */
  };

  /**
   * @brief Annotation dictionary
   */
  class GoToActionDictionary : public ActionDictionary{
  public:
    GoToActionDictionary(void);
    ~GoToActionDictionary(void);

    using ActionDictionary::addKey;
    void addKey(GoToActionDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<GoToActionDictionaryKey, std::pair<std::string, uint32_t>> mGoToActionDictionaryMap;

  private:
    GoToActionDictionary(GoToActionDictionary const &) = delete;
    GoToActionDictionary &operator=(GoToActionDictionary const &) = delete;
  };

}}
