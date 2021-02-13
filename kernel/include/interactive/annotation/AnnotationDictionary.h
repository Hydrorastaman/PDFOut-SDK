#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace interactive{

  enum AnnotationDictionaryKey{
    AnnotationDictionaryKeySubtype,      /**< (Required) The type of annotation that this dictionary describes. */
    AnnotationDictionaryKeyRect,         /**< (Required) The annotation rectangle, defining the location of the annotation on the page in default user space units. */
    AnnotationDictionaryKeyContents,     /**< (Optional) Text to be displayed for the annotation. */
    AnnotationDictionaryKeyP,            /**< (Optional; PDF 1.3; not used in FDF files) An indirect reference to the page object with which this annotation is associated. */
    AnnotationDictionaryKeyNM,           /**< (Optional; PDF 1.4) The annotation name, a text string uniquely identifying it among all the annotations on its page. */
    AnnotationDictionaryKeyM,            /**< (Optional; PDF 1.1) The date and time when the annotation was most recently modified. */
    AnnotationDictionaryKeyF,            /**< (Optional; PDF 1.1) A set of flags specifying various characteristics of the annotation. */
    AnnotationDictionaryKeyAP,           /**< (Optional; PDF 1.2) An appearance dictionary specifying how the annotation is presented visually on the page. */
    AnnotationDictionaryKeyAS,           /**< (Required if the appearance dictionary AP contains one or more subdictionaries; PDF 1.2) The annotation's appearance state, which selects the applicable appearance stream from an appearance subdictionary. */
    AnnotationDictionaryKeyBorder,       /**< (Optional) An array specifying the characteristics of the annotation's border. The border is specified as a rounded rectangle. */
    AnnotationDictionaryKeyC,            /**< (Optional; PDF 1.1) An array of numbers in the range 0.0 to 1.0, representing a color used. */
    AnnotationDictionaryKeyStructParent, /**< (Required if the annotation is a structural content item; PDF 1.3) The integer key of the annotation's entry in the structural parent tree. */
    AnnotationDictionaryKeyOC,           /**< (Optional; PDF 1.5) An optional content group or optional content membership dictionary (see Section 4.10, “Optional Content”) specifying the optional content properties for the annotation. */
  };

  enum MarkupAnnotationDictionaryKey{
    MarkupAnnotationDictionaryKeyT,            /**< (Optional; PDF 1.1) The text label to be displayed in the title bar of the annotation's pop-up window when open and active. By convention, this entry identifies the user who added the annotation. */
    MarkupAnnotationDictionaryKeyPopup,        /**< (Optional; PDF 1.3) An indirect reference to a pop-up annotation for entering or editing the text associated with this annotation. */
    MarkupAnnotationDictionaryKeyCA,           /**< (Optional; PDF 1.4) The constant opacity value to be used in painting the annotation. This value applies to all visible elements of the annotation in its closed state (including its background and border) but not to the pop-up window that appears when the annotation is opened. */
    MarkupAnnotationDictionaryKeyRC,           /**< (Optional; PDF 1.5) A rich text string to be displayed in the pop-up window when the annotation is opened. */
    MarkupAnnotationDictionaryKeyCreationDate, /**< (Optional; PDF 1.5) The date and time (Section 3.8.3, “Dates”) when the annotation was created. */
    MarkupAnnotationDictionaryKeyIRT,          /**< (Required if an RT entry is present, otherwise optional; PDF 1.5) A reference to the annotation that this annotation is “in reply to.” */
    MarkupAnnotationDictionaryKeySubj,         /**< (Optional; PDF 1.5) Text representing a short description of the subject being addressed by the annotation. */
    MarkupAnnotationDictionaryKeyRT,           /**< (Optional; meaningful only if IRT is present; PDF 1.6) A name specifying the relationship (the “reply type”) between this annotation and one specified by IRT. */
    MarkupAnnotationDictionaryKeyIT,           /**< (Optional; PDF 1.6) A name describing the intent of the markup annotation. */
    MarkupAnnotationDictionaryKeyExData        /**< (Optional; PDF 1.7) An external data dictionary specifying data to be associated with the annotation. */
  };

  /**
   * @brief Annotation dictionary
   */
  class AnnotationDictionary : public virtual ObjectDictionary{
  public:
    AnnotationDictionary(void);
    ~AnnotationDictionary(void);

    void addKey(AnnotationDictionaryKey key, std::unique_ptr<Object> value);
    void addKey(MarkupAnnotationDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<AnnotationDictionaryKey, std::pair<std::string, uint32_t>> mAnnotationDictionaryMap;
    static std::unordered_map<MarkupAnnotationDictionaryKey, std::pair<std::string, uint32_t>> mMarkupAnnotationDictionaryMap;

  private:
    AnnotationDictionary(AnnotationDictionary const &) = delete;
    AnnotationDictionary &operator=(AnnotationDictionary const &) = delete;
  };

}}
