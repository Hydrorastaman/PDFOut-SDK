#pragma once

#include <AcroForm.h>

#include <interactive/acroform/AcroFormFieldDictionary.h>

namespace kernel{ namespace interactive{

  enum AcroFormSignatureDictionaryKey{
    AcroFormSignatureDictionaryKeyLock, /**< (Optional; must be an indirect reference; PDF 1.5) A signature field lock dictionary that specifies a set of form fields to be locked when this signature field is signed. Table 8.82lists the entries in this dictionary. */
    AcroFormSignatureDictionaryKeySV    /**< (Optional; must be an indirect reference; PDF 1.5) A seed value dictionary containing information that constrains the properties of a signature that is applied to this field. */
  };

  class AcroFormSignatureDictionary : public AcroFormFieldDictionary{
  public:
    AcroFormSignatureDictionary(void);
    ~AcroFormSignatureDictionary(void);

    using AcroFormFieldDictionary::addKey;
    void addKey(AcroFormSignatureDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<AcroFormSignatureDictionaryKey, std::pair<std::string, uint32_t>> mAcroFormSignatureDictionaryMap;

  private:
    AcroFormSignatureDictionary(AcroFormSignatureDictionary const &) = delete;
    AcroFormSignatureDictionary &operator=(AcroFormSignatureDictionary const &) = delete;
  };

}}
