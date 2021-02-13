#pragma once

#include <object/ObjectDictionary.h>
#include <interactive/annotation/AnnotationDictionary.h>

namespace kernel{ namespace interactive{

  enum TextAnnotationDictionaryKey{
    TextAnnotationDictionaryKeyOpen,      /**< (Optional) A flag specifying whether the annotation should initially be displayed open. Default value: false (closed). */
    TextAnnotationDictionaryKeyName,      /**< (Optional) The name of an icon to be used in displaying the annotation. */
    TextAnnotationDictionaryKeyState,     /**< (Optional; PDF 1.5) The state to which the original annotation should be set. */
    TextAnnotationDictionaryKeyStateModel /**< (Required if State is present, otherwise optional; PDF 1.5) The state model corresponding to State. */
  };

  /**
   * @brief A text annotation represents a “sticky note” attached to a point in the PDF document.
   */
  class TextAnnotationDictionary : public AnnotationDictionary{
  public:
    TextAnnotationDictionary(void);
    ~TextAnnotationDictionary(void);

    using AnnotationDictionary::addKey;
    void addKey(TextAnnotationDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<TextAnnotationDictionaryKey, std::pair<std::string , uint32_t>> mTextAnnotationMap;

  private:
    TextAnnotationDictionary(TextAnnotationDictionary const &) = delete;
    TextAnnotationDictionary &operator=(TextAnnotationDictionary const &) = delete;
  };

}}
