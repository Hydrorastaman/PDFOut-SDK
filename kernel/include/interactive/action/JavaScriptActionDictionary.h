#pragma once

#include <interactive/action/ActionDictionary.h>

namespace kernel{ namespace interactive{

  enum JavaScriptActionDictionaryKey{
    JavaScriptActionDictionaryKeyJS /**< (Required) A text string or text stream containing the JavaScript script to be executed. */
  };

  /**
   * @brief Annotation dictionary
   */
  class JavaScriptActionDictionary : public ActionDictionary{
  public:
    JavaScriptActionDictionary(void);
    ~JavaScriptActionDictionary(void);

    using ActionDictionary::addKey;
    void addKey(JavaScriptActionDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<JavaScriptActionDictionaryKey, std::pair<std::string, uint32_t>> mJavaScriptActionDictionaryMap;

  private:
    JavaScriptActionDictionary(JavaScriptActionDictionary const &) = delete;
    JavaScriptActionDictionary &operator=(JavaScriptActionDictionary const &) = delete;
  };

}}
