#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace interactive{

  enum AcroFormDictionaryKey{
    AcroFormDictionaryKeyFields,          /**< (Required) An array of references to the document's root fields. */
    AcroFormDictionaryKeyNeedAppearances, /**< (Optional) A flag specifying whether to construct appearance streams and appearance dictionaries for all widget annotations in the document. */
    AcroFormDictionaryKeySigFlags,        /**< (Optional; PDF 1.3) A set of flags specifying various document-level characteristics related to signature fields. */
    AcroFormDictionaryKeyCO,              /**< (Required if any fields in the document have additional-actions dictionaries containing a C entry; PDF 1.3) An array of indirect references to field dictionaries with calculation actions, defining the calculation order in which their values will be recalculated when the value of any field changes. */
    AcroFormDictionaryKeyDR,              /**< (Optional) A resource dictionary containing default resources (such as fonts, patterns, or color spaces) to be used by form field appearance streams. */
    AcroFormDictionaryKeyDA,              /**< (Optional) A document-wide default value for the DA attribute of variable text fields. */
    AcroFormDictionaryKeyQ,               /**< (Optional) A document-wide default value for the Q attribute of variable text fields. */
    AcroFormDictionaryKeyXFA              /**< (Optional; PDF 1.5) A stream or array containing an XFA resource, whose format is described by the Data Package (XDP) Specification. */
  };

  class AcroFormDictionary : public ObjectDictionary{
  public:
    AcroFormDictionary(void);
    ~AcroFormDictionary(void);

    void addKey(AcroFormDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<AcroFormDictionaryKey, std::pair<std::string, uint32_t>> mAcroFormDictionaryMap;

  private:
    AcroFormDictionary(AcroFormDictionary const &) = delete;
    AcroFormDictionary &operator=(AcroFormDictionary const &) = delete;
  };

}}
