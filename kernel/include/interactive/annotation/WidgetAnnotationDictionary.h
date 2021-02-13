#pragma once

#include <object/ObjectDictionary.h>
#include <interactive/annotation/AnnotationDictionary.h>
#include <interactive/acroform/AcroFormButtonDictionary.h>

namespace kernel{ namespace interactive{

  enum WidgetAnnotationDictionaryKey{
    WidgetAnnotationDictionaryKeyH,  /**< (Optional) The annotation's highlighting mode, the visual effect to be used when the mouse button is pressed or held down inside its active area. */
    WidgetAnnotationDictionaryKeyMK, /**< (Optional) An appearance characteristics dictionary to be used in constructing a dynamic appearance stream specifying the annotation's visual presentation on the page. */
    WidgetAnnotationDictionaryKeyA,  /**< (Optional; PDF 1.1) An action to be performed when the annotation is activated. */
    WidgetAnnotationDictionaryKeyAA, /**< (Optional; PDF 1.2) An additional-actions dictionary defining the annotation's behavior in response to various trigger events. */
    WidgetAnnotationDictionaryKeyBS  /**< (Optional; PDF 1.2) A border style dictionary specifying the width and dash pattern to be used in drawing the annotation's border. */
  };

  /**
   * @brief Interactive forms use widget annotations to represent the appearance of fields and to manage user interactions.
   */
  class WidgetAnnotationDictionary : public AnnotationDictionary,
                                     public AcroFormButtonDictionary{
  public:
    WidgetAnnotationDictionary(void);
    ~WidgetAnnotationDictionary(void);

    using AnnotationDictionary::addKey;
    using AcroFormButtonDictionary::addKey;
    void addKey(WidgetAnnotationDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<WidgetAnnotationDictionaryKey, std::pair<std::string , uint32_t>> mWidgetAnnotationMap;

  private:
    WidgetAnnotationDictionary(WidgetAnnotationDictionary const &) = delete;
    WidgetAnnotationDictionary &operator=(WidgetAnnotationDictionary const &) = delete;
  };

}}
