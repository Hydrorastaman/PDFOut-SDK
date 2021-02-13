#pragma once

#include <AcroForm.h>

#include <interactive/acroform/AcroFormFieldDictionary.h>

namespace kernel{ namespace interactive{

  enum AcroFormTextDictionaryKey{
    AcroFormTextDictionaryKeyMaxLen /**< (Optional; inheritable) The maximum length of the field's text, in characters. */
  };

  class AcroFormTextDictionary : public AcroFormFieldDictionary{
  public:
    AcroFormTextDictionary(void);
    ~AcroFormTextDictionary(void);

    using AcroFormFieldDictionary::addKey;
    void addKey(AcroFormTextDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<AcroFormTextDictionaryKey, std::pair<std::string, uint32_t>> mAcroFormTextDictionaryMap;

  private:
    AcroFormTextDictionary(AcroFormTextDictionary const &) = delete;
    AcroFormTextDictionary &operator=(AcroFormTextDictionary const &) = delete;
  };

}}
