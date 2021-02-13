#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace interactive{

  enum AnnotationBorderStyleKey{
    AnnotationBorderStyleKeyW, /**< (Optional) The border width in points. If this value is 0, no border is drawn. Default value: 1. */
    AnnotationBorderStyleKeyS, /**< (Optional) The border style:
                                        S(Solid) A solid rectangle surrounding the annotation.
                                        D(Dashed) A dashed rectangle surrounding the annotation. The dash pattern is specified by the D entry (see below).
                                        B(Beveled) A simulated embossed rectangle that appears to be raised above the surface of the page.
                                        I(Inset) A simulated engraved rectangle that appears to be recessed below the surface of the page.
                                        U(Underline) A single line along the bottom of the annotation rectangle. */
    AnnotationBorderStyleKeyD, /**< (Optional) A dash array defining a pattern of dashes and gaps to be used in drawing a dashed border (border style D above). */
    AnnotationBorderStyleKeyBE /**< (Optional) A border effect dictionary that specifies an effect to be applied to the border of the annotations. */
  };

  class AnnotationBorderStyle : public ObjectDictionary{
  public:
    AnnotationBorderStyle(void);
    ~AnnotationBorderStyle(void);

    void addKey(AnnotationBorderStyleKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<AnnotationBorderStyleKey, std::pair<std::string, uint32_t>> mAnnotationBorderStyleDictionaryMap;

  private:
    AnnotationBorderStyle(AnnotationBorderStyle const &) = delete;
    AnnotationBorderStyle &operator=(AnnotationBorderStyle const &) = delete;
  };

}}
