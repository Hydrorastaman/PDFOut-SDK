#include <interactive/annotation/AnnotationBorderEffectDictionary.h>

namespace{
  std::string const keyS = "S";
  std::string const keyI = "I";
}

namespace kernel{ namespace interactive{

  std::unordered_map<AnnotationBorderEffectKey, std::pair<std::string, uint32_t>> AnnotationBorderEffectDictionary::mAnnotationBorderEffectDictionaryMap{
    {AnnotationBorderEffectKeyS, {keyS, ObjectTypeName}},
    {AnnotationBorderEffectKeyI, {keyI,  ObjectTypeNumber}}
  };

  AnnotationBorderEffectDictionary::AnnotationBorderEffectDictionary(void)
    : ObjectDictionary(IndirectTypeNonIndirectable){
  }

  AnnotationBorderEffectDictionary::~AnnotationBorderEffectDictionary(void) {}

  void AnnotationBorderEffectDictionary::addKey(AnnotationBorderEffectKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mAnnotationBorderEffectDictionaryMap, key, std::move(value));
  }
}}
