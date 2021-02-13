#pragma once

#include <object/ObjectDictionary.h>
#include <interactive/annotation/AnnotationDictionary.h>

namespace kernel{ namespace interactive{

  enum FreeTextAnnotationDictionaryKey{
    FreeTextAnnotationDictionaryKeyDA, /**< (Required) The default appearance string to be used in formatting the text. */
    FreeTextAnnotationDictionaryKeyQ,  /**< (Optional; PDF 1.4) A code specifying the form of quadding (justification) to be used in displaying the annotation's text. */
    FreeTextAnnotationDictionaryKeyRC, /**< (Optional; PDF 1.5) A rich text string to be used to generate the appearance of the annotation. */
    FreeTextAnnotationDictionaryKeyDS, /**< (Optional; PDF 1.5) A default style string. */
    FreeTextAnnotationDictionaryKeyCL, /**< (Optional; PDF 1.6) An array of four or six numbers specifying a callout line attached to the free text annotation. Six numbers [ x1 y1 x2 y2 x3 y3 ] represent the starting, knee point, and ending coordinates of the line in default user space. Four numbers [ x1 y1 x2 y2 ] represent the starting and ending coordinates of the line. */
    FreeTextAnnotationDictionaryKeyIT, /**< (Optional; PDF 1.6) A name describing the intent of the free text annotation. */
    FreeTextAnnotationDictionaryKeyBE, /**< (Optional; PDF 1.6) A border effect dictionary used in conjunction with the border style dictionary specified by the BS entry */
    FreeTextAnnotationDictionaryKeyRD, /**< (Optional; PDF 1.6) A set of four numbers describing the numerical differences between two rectangles: the Rect entry of the annotation and a rectangle contained within that rectangle. */
    FreeTextAnnotationDictionaryKeyBS, /**< (Optional; PDF 1.6) A border style dictionary specifying the line width and dash pattern to be used in drawing the annotation's border. */
    FreeTextAnnotationDictionaryKeyLE, /**< (Optional; PDF 1.6) An array of two names specifying the line ending styles to be used in drawing the annotation's border. */
  };

  /**
   * @brief A free text annotation displays text directly on the page.
   */
  class FreeTextAnnotationDictionary : public AnnotationDictionary{
  public:
    FreeTextAnnotationDictionary(void);
    ~FreeTextAnnotationDictionary(void);

    using AnnotationDictionary::addKey;
    void addKey(FreeTextAnnotationDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<FreeTextAnnotationDictionaryKey, std::pair<std::string , uint32_t>> mFreeTextAnnotationMap;

  private:
    FreeTextAnnotationDictionary(FreeTextAnnotationDictionary const &) = delete;
    FreeTextAnnotationDictionary &operator=(FreeTextAnnotationDictionary const &) = delete;
  };

}}
