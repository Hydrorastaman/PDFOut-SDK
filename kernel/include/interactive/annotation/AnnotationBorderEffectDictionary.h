#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace interactive{

  enum AnnotationBorderEffectKey{
    AnnotationBorderEffectKeyS, /**< (Optional) A name representing the border effect to apply. Possible values are: S - No effect: the border is as described by the annotation dictionary's BS entry. C - The border should appear “cloudy”. The width and dash array specified by BS are honored. Default value: S. */
    AnnotationBorderEffectKeyI  /**< (Optional; valid only if the value of S is C) A number describing the intensity of the effect. Suggested values range from 0 to 2. Default value: 0. */
  };

  class AnnotationBorderEffectDictionary : public ObjectDictionary{
  public:
    AnnotationBorderEffectDictionary(void);
    ~AnnotationBorderEffectDictionary(void);

    void addKey(AnnotationBorderEffectKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<AnnotationBorderEffectKey, std::pair<std::string, uint32_t>> mAnnotationBorderEffectDictionaryMap;

  private:
    AnnotationBorderEffectDictionary(AnnotationBorderEffectDictionary const &) = delete;
    AnnotationBorderEffectDictionary &operator=(AnnotationBorderEffectDictionary const &) = delete;
  };

}}
