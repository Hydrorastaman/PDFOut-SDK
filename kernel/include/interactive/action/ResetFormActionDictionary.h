#pragma once

#include <interactive/action/ActionDictionary.h>

namespace kernel{ namespace interactive{

  enum ResetFormActionDictionaryKey{
    ResetFormActionDictionaryKeyFields, /**< (Optional) An array identifying which fields to reset or which to exclude from resetting, depending on the setting of the Include/Exclude flag in the Flags entry. */
    ResetFormActionDictionaryKeyFlags   /**< (Optional; inheritable) A set of flags specifying various characteristics of the action. */
  };

  /**
   * @brief Annotation dictionary
   */
  class ResetFormActionDictionary : public ActionDictionary{
  public:
    ResetFormActionDictionary(void);
    ~ResetFormActionDictionary(void);

    using ActionDictionary::addKey;
    void addKey(ResetFormActionDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<ResetFormActionDictionaryKey, std::pair<std::string, uint32_t>> mResetFormActionDictionaryMap;

  private:
    ResetFormActionDictionary(ResetFormActionDictionary const &) = delete;
    ResetFormActionDictionary &operator=(ResetFormActionDictionary const &) = delete;
  };

}}
