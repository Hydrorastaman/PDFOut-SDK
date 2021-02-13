#pragma once

#include <interactive/action/ActionDictionary.h>

namespace kernel{ namespace interactive{

  enum SubmitFormActionDictionaryKey{
    SubmitFormActionDictionaryKeyF,      /**< (Required) A URL file specification giving the uniform resource locator (URL) of the script at the Web server that will process the submission. */
    SubmitFormActionDictionaryKeyFields, /**< (Optional) An array identifying which fields to include in the submission or which to exclude, depending on the setting of the Include/Exclude flag in the Flags entry (see Table 8.86). Each element of the array is either an indirect reference to a field dictionary or (PDF 1.3) a text string representing the fully qualified name of a field. Elements of both kinds may be mixed in the same array. */
    SubmitFormActionDictionaryKeyFlags   /**< (Optional; inheritable) A set of flags specifying various characteristics of the action. */
  };

  /**
   * @brief Annotation dictionary
   */
  class SubmitFormActionDictionary : public ActionDictionary{
  public:
    SubmitFormActionDictionary(void);
    ~SubmitFormActionDictionary(void);

    using ActionDictionary::addKey;
    void addKey(SubmitFormActionDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<SubmitFormActionDictionaryKey, std::pair<std::string, uint32_t>> mSubmitFormActionDictionaryMap;

  private:
    SubmitFormActionDictionary(SubmitFormActionDictionary const &) = delete;
    SubmitFormActionDictionary &operator=(SubmitFormActionDictionary const &) = delete;
  };

}}
