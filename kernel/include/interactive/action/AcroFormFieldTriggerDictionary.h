#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace interactive{

  enum AcroFormFieldTriggerDictionaryKey{
    AcroFormFieldTriggerDictionaryKeyK, /**< (Optional; PDF 1.3) A JavaScript action to be performed when the user types a keystroke into a text field or combo box or modifies the selection in a scrollable list box. This action can check the keystroke for validity and reject or modify it. */
    AcroFormFieldTriggerDictionaryKeyF, /**< (Optional; PDF 1.3) A JavaScript action to be performed before the field is formatted to display its current value. This action can modify the field's value before formatting. */
    AcroFormFieldTriggerDictionaryKeyV, /**< (Optional; PDF 1.3) A JavaScript action to be performed when the field's value is changed. This action can check the new value for validity. (The name V stands for “validate.”) */
    AcroFormFieldTriggerDictionaryKeyC  /**< (Optional; PDF 1.3) A JavaScript action to be performed to recalculate the value of this field when that of another field changes. (The name C stands for “calculate.”) The order in which the document's fields are recalculated is defined by the CO entry in the interactive form dictionary. */
  };

  /**
   * @brief Annotation dictionary
   */
  class AcroFormFieldTriggerDictionary : public ObjectDictionary{
  public:
    AcroFormFieldTriggerDictionary(void);
    ~AcroFormFieldTriggerDictionary(void);

    void addKey(AcroFormFieldTriggerDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<AcroFormFieldTriggerDictionaryKey, std::pair<std::string, uint32_t>> mAcroFormFieldTriggerDictionaryMap;

  private:
    AcroFormFieldTriggerDictionary(AcroFormFieldTriggerDictionary const &) = delete;
    AcroFormFieldTriggerDictionary &operator=(AcroFormFieldTriggerDictionary const &) = delete;
  };

}}
