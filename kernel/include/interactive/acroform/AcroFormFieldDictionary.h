#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace interactive{

  enum AcroFormFieldDictionaryKey{
    AcroFormFieldDictionaryKeyFT,     /**< (Required for terminal fields; inheritable) The type of field that this dictionary. */
    AcroFormFieldDictionaryKeyParent, /**< (Required if this field is the child of another in the field hierarchy; absent otherwise) The field that is the immediate parent of this one (the field, if any, whose Kids array includes this field). */
    AcroFormFieldDictionaryKeyKids,   /**< (Sometimes required, as described below) An array of indirect references to the immediate children of this field. */
    AcroFormFieldDictionaryKeyT,      /**< (Optional) The partial field name. */
    AcroFormFieldDictionaryKeyTU,     /**< (Optional; PDF 1.3) An alternate field name to be used in place of the actual field name wherever the field must be identified in the user interface (such as in error or status messages referring to the field). */
    AcroFormFieldDictionaryKeyTM,     /**< (Optional; PDF 1.3) The mapping name to be used when exporting interactive form field data from the document. */
    AcroFormFieldDictionaryKeyFf,     /**< (Optional; inheritable) A set of flags specifying various characteristics of the field. */
    AcroFormFieldDictionaryKeyV,      /**< (Optional; inheritable) The field's value, whose format varies depending on the field type. See the descriptions of individual field types for further information. */
    AcroFormFieldDictionaryKeyDV,     /**< (Optional; inheritable) The default value to which the field reverts when a reset-form action is executed. The format of this value is the same as that of V. */
    AcroFormFieldDictionaryKeyAA,     /**< (Optional; PDF 1.2) An additional-actions dictionary defining the field's behavior in response to various trigger events. */

    // for variable text
    AcroFormFieldDictionaryKeyDA,     /**< (Required; inheritable) The default appearance string containing a sequence of valid page-content graphics or text state operators that define such properties as the field's text size and color. */
    AcroFormFieldDictionaryKeyQ,      /**< (Optional; inheritable) A code specifying the form of quadding (justification) to be used in displaying the text. */
    AcroFormFieldDictionaryKeyDS,     /**< (Optional; PDF 1.5) A default style string, as described in "Rich Text Strings". */
    AcroFormFieldDictionaryKeyRV      /**< (Optional; PDF 1.5) A rich text string, as described in “Rich Text Strings” */
  };

  class AcroFormFieldDictionary : public virtual ObjectDictionary{
  public:
    AcroFormFieldDictionary(void);
    ~AcroFormFieldDictionary(void);

    void addKey(AcroFormFieldDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<AcroFormFieldDictionaryKey, std::pair<std::string, uint32_t>> mAcroFormFieldDictionaryMap;

  private:
    AcroFormFieldDictionary(AcroFormFieldDictionary const &) = delete;
    AcroFormFieldDictionary &operator=(AcroFormFieldDictionary const &) = delete;
  };

}}
