#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace interactive{

  enum AppearanceDictionaryKey{
    AppearanceDictionaryKeyN, /**< (Required) The annotation's normal appearance. */
    AppearanceDictionaryKeyR, /**< (Optional) The annotation's rollover appearance. Default value: the value of the N entry.*/
    AppearanceDictionaryKeyD  /**< (Optional) The annotation's down appearance. Default value: the value of the N entry. */
  };

  class AppearanceDictionary : public ObjectDictionary{
  public:
    AppearanceDictionary(void);
    ~AppearanceDictionary(void);

    void addKey(AppearanceDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<AppearanceDictionaryKey, std::pair<std::string, uint32_t>> mAppearanceDictionaryMap;

  private:
    AppearanceDictionary(AppearanceDictionary const &) = delete;
    AppearanceDictionary &operator=(AppearanceDictionary const &) = delete;
  };

}}
