#pragma once

#include <AcroForm.h>

#include <interactive/acroform/AcroFormFieldDictionary.h>

namespace kernel{ namespace interactive{

  enum AcroFormButtonDictionaryKey{
    AcroFormButtonDictionaryKeyOpt /**< (Optional; inheritable; PDF 1.4) An array containing one entry for each widget annotation in the Kids array of the radio button or check box field. Each entry is a text string representing the on state of the corresponding widget annotation. */
  };

  class AcroFormButtonDictionary : public AcroFormFieldDictionary{
  public:
    AcroFormButtonDictionary(void);
    ~AcroFormButtonDictionary(void);

    using AcroFormFieldDictionary::addKey;
    void addKey(AcroFormButtonDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<AcroFormButtonDictionaryKey, std::pair<std::string, uint32_t>> mAcroFormButtonDictionaryMap;

  private:
    AcroFormButtonDictionary(AcroFormButtonDictionary const &) = delete;
    AcroFormButtonDictionary &operator=(AcroFormButtonDictionary const &) = delete;
  };

}}
