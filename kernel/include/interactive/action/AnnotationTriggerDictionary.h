#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace interactive{

  enum AnnotationTriggerDictionaryKey{
    AnnotationTriggerDictionaryKeyE,  /**< (Optional; PDF 1.2) An action to be performed when the cursor enters the annotation's active area. */
    AnnotationTriggerDictionaryKeyX,  /**< (Optional; PDF 1.2) An action to be performed when the cursor exits the annotation's active area. */
    AnnotationTriggerDictionaryKeyD,  /**< (Optional; PDF 1.2) An action to be performed when the mouse button is pressed inside the annotation's active area. (The name D stands for “down.”) */
    AnnotationTriggerDictionaryKeyU,  /**< (Optional; PDF 1.2) An action to be performed when the mouse button is released inside the annotation's active area. (The name U stands for “up.”) */
    AnnotationTriggerDictionaryKeyFo, /**< (Optional; PDF 1.2; widget annotations only) An action to be performed when the annotation receives the input focus. */
    AnnotationTriggerDictionaryKeyBl, /**< (Optional; PDF 1.2; widget annotations only) (Uppercase B, lowercase L) An action to be performed when the annotation loses the input focus. (The name Bl stands for “blurred.”) */
    AnnotationTriggerDictionaryKeyPO, /**< (Optional; PDF 1.5) An action to be performed when the page containing the annotation is opened (for example, when the user navigates to it from the next or previous page or by means of a link annotation or outline item). The action is executed after the O action in the page's additional-actions dictionary (see Table 8.45) and the OpenAction entry in the document catalog (see Table 3.25), if such actions are present. */
    AnnotationTriggerDictionaryKeyPC, /**< (Optional; PDF 1.5) An action to be performed when the page containing the annotation is closed (for example, when the user navigates to the next or previous page, or follows a link annotation or outline item). The action is executed before the C action in the page's additional-actions dictionary (see Table 8.45), if present. */
    AnnotationTriggerDictionaryKeyPV, /**< (Optional; PDF 1.5) An action to be performed when the page containing the annotation becomes visible in the viewer application's user interface. */
    AnnotationTriggerDictionaryKeyPI  /**< (Optional; PDF 1.5) An action to be performed when the page containing the annotation is no longer visible in the viewer application's user interface. */
  };

  /**
   * @brief Annotation dictionary
   */
  class AnnotationTriggerDictionary : public ObjectDictionary{
  public:
    AnnotationTriggerDictionary(void);
    ~AnnotationTriggerDictionary(void);

    void addKey(AnnotationTriggerDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<AnnotationTriggerDictionaryKey, std::pair<std::string, uint32_t>> mAnnotationTriggerDictionaryMap;

  private:
    AnnotationTriggerDictionary(AnnotationTriggerDictionary const &) = delete;
    AnnotationTriggerDictionary &operator=(AnnotationTriggerDictionary const &) = delete;
  };

}}
