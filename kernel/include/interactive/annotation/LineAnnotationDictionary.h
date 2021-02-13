#pragma once

#include <object/ObjectDictionary.h>
#include <interactive/annotation/AnnotationDictionary.h>

namespace kernel{ namespace interactive{

  enum LineAnnotationDictionaryKey{
    LineAnnotationDictionaryKeyL,       /**< (Required) An array of four numbers, [ x1 y1 x2 y2 ], specifying the starting and ending coordinates of the line in default user space. */
    LineAnnotationDictionaryKeyBS,      /**< (Optional) A border style dictionary specifying the width and dash pattern to be used in drawing the line. */
    LineAnnotationDictionaryKeyLE,      /**< (Optional; PDF 1.4) An array of two names specifying the line ending styles to be used in drawing the line. The first and second elements of the array specify the line ending styles for the endpoints defined, respectively, by the first and second pairs of coordinates, (x1 , y1 ) and (x2 , y2 ), in the L array. */
    LineAnnotationDictionaryKeyIC,      /**< (Optional; PDF 1.4) An array of numbers in the range 0.0 to 1.0 specifying the interior color with which to fill the annotation's line endings. */
    LineAnnotationDictionaryKeyLL,      /**< (Required if LLE is present, otherwise optional; PDF 1.6) The length of leader linesin default user space that extend from each endpoint of the line perpendicular to the line itself. */
    LineAnnotationDictionaryKeyLLE,     /**< (Optional; PDF 1.6) A non-negative number representing the length of leader line extensions that extend from the line proper 180 degrees from the leader lines. */
    LineAnnotationDictionaryKeyCap,     /**< (Optional; PDF 1.6) If true, the text specified by the Contents or RC entries should be replicated as a caption in the appearance of the line. */
    LineAnnotationDictionaryKeyIT,      /**< (Optional; PDF 1.6) A name describing the intent of the line annotation. Valid values are LineArrow, which means that the annotation is intended to function as an arrow, and LineDimension, which means that the annotation is intended to function as a dimension line. */
    LineAnnotationDictionaryKeyLLO,     /**< (Optional; PDF 1.7) A non-negative number representing the length of the leader line offset, which is the amount of empty space between the endpoints of the annotation and the beginning of the leader lines. */
    LineAnnotationDictionaryKeyCP,      /**< (Optional; meaningful only if Cap is true; PDF 1.7) A name describing the annotation's caption positioning. Valid values are Inline, meaning the caption will be centered inside the line, and Top, meaning the caption will be on top of the line. */
    LineAnnotationDictionaryKeyMeasure, /**< (Optional; PDF 1.7) A measure dictionary that specifies the scale and units that apply to the line annotation. */
    LineAnnotationDictionaryKeyCO       /**< (Optional; meaningful only if Cap is true; PDF 1.7) An array of two numbers specifying the offset of the caption text from its normal position. The first value is the horizontal offset along the annotation line from its midpoint, with a positive value indicating offset to the right and a negative value indicating offset to the left. The second value is the vertical offset perpendicular to the annotation line, with a positive value indicating a shift up and a negative value indicating a shift down. */
  };

  /**
   * @brief A line annotation displays a single straight line on the page.
   */
  class LineAnnotationDictionary : public AnnotationDictionary{
  public:
    LineAnnotationDictionary(void);
    ~LineAnnotationDictionary(void);

    using AnnotationDictionary::addKey;
    void addKey(LineAnnotationDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<LineAnnotationDictionaryKey, std::pair<std::string , uint32_t>> mLineAnnotationMap;

  private:
    LineAnnotationDictionary(LineAnnotationDictionary const &) = delete;
    LineAnnotationDictionary &operator=(LineAnnotationDictionary const &) = delete;
  };

}}
