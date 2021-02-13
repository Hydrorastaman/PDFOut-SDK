#pragma once

#include <object/ObjectDictionary.h>
#include <interactive/annotation/AnnotationDictionary.h>

namespace kernel{ namespace interactive{

  enum CircleAnnotationDictionaryKey{
    CircleAnnotationDictionaryKeyBS, /**< (Optional) A border style dictionary specifying the line width and dash pattern to be used in drawing the ellipse. */
    CircleAnnotationDictionaryKeyIC, /**< (Optional; PDF 1.4) An array of numbers in the range 0.0 to 1.0 specifying the interior color with which to fill the annotation's ellipse. */
    CircleAnnotationDictionaryKeyBE, /**< (Optional; PDF 1.5) A border effect dictionary describing an effect applied to the border described by the BS entry */
    CircleAnnotationDictionaryKeyRD  /**< (Optional; PDF 1.5) A set of four numbers describing the numerical differences between two rectangles: the Rect entry of the annotation and the actual boundaries of the underlying circle. */
  };

  /**
   * @brief A square annotations display a rectangle on the page.
   */
  class CircleAnnotationDictionary : public AnnotationDictionary{
  public:
    CircleAnnotationDictionary(void);
    ~CircleAnnotationDictionary(void);

    using AnnotationDictionary::addKey;
    void addKey(CircleAnnotationDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<CircleAnnotationDictionaryKey, std::pair<std::string , uint32_t>> mCircleAnnotationMap;

  private:
    CircleAnnotationDictionary(CircleAnnotationDictionary const &) = delete;
    CircleAnnotationDictionary &operator=(CircleAnnotationDictionary const &) = delete;
  };

}}
