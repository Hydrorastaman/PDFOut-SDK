#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace interactive{

  enum ActionDictionaryKey{
    ActionDictionaryKeyS,   /**< (Required) The type of action that this dictionary describes.*/
    ActionDictionaryKeyNext /**< (Optional; PDF 1.2) The next action or sequence of actions to be performed after the action represented by this dictionary. */
  };

  /**
   * @brief Annotation dictionary
   */
  class ActionDictionary : public ObjectDictionary{
  public:
    ActionDictionary(void);
    ~ActionDictionary(void);

    void addKey(ActionDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<ActionDictionaryKey, std::pair<std::string, uint32_t>> mActionDictionaryMap;

  private:
    ActionDictionary(ActionDictionary const &) = delete;
    ActionDictionary &operator=(ActionDictionary const &) = delete;
  };

}}
