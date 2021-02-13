#pragma once

#include <AcroForm.h>

#include <interactive/acroform/AcroFormFieldDictionary.h>

namespace kernel{ namespace interactive{

  enum AcroFormChoiceDictionaryKey{
    AcroFormChoiceDictionaryKeyOpt, /**< (Optional) An array of options to be presented to the user. Each element of the array is either a text string representing one of the available options or an array consisting of two text strings: the option's export value and the text to be displayed as the name of the option. */
    AcroFormChoiceDictionaryKeyTI,  /**< (Optional) For scrollable list boxes, the top index (the index in the Opt array of the first option visible in the list). */
    AcroFormChoiceDictionaryKeyI    /**< (Sometimes required, otherwise optional; PDF 1.4) For choice fields that allow multiple selection (MultiSelect flag set), an array of integers, sorted in ascending order, representing the zero-based indices in the Opt array of the currently selected option items. This entry is required when two or more elements in the Opt array have different names but the same export value or when the value of the choice field is an array. In other cases, the entry is permitted but not required. If the items identified by this entry differ from those in the V entry of the field dictionary (see below), the V entry takes precedence. */
  };

  class AcroFormChoiceDictionary : public AcroFormFieldDictionary{
  public:
    AcroFormChoiceDictionary(void);
    ~AcroFormChoiceDictionary(void);

    using AcroFormFieldDictionary::addKey;
    void addKey(AcroFormChoiceDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<AcroFormChoiceDictionaryKey, std::pair<std::string, uint32_t>> mAcroFormChoiceDictionaryMap;

  private:
    AcroFormChoiceDictionary(AcroFormChoiceDictionary const &) = delete;
    AcroFormChoiceDictionary &operator=(AcroFormChoiceDictionary const &) = delete;
  };

}}
