#pragma once

#include <object/ObjectDictionary.h>
#include <interactive/annotation/AnnotationDictionary.h>

namespace kernel{ namespace interactive{

  enum LinkAnnotationDictionaryKey{
    LinkAnnotationDictionaryKeyA,         /**< (Optional; PDF 1.1) An action to be performed when the link annotation is activated. */
    LinkAnnotationDictionaryKeyDest,      /**< (Optional; not permitted if an A entry is present) A destination to be displayed when the annotation is activated. */
    LinkAnnotationDictionaryKeyH,         /**< (Optional; PDF 1.2) The annotation's highlighting mode, the visual effect to be used when the mouse button is pressed or held down inside its active area. */
    LinkAnnotationDictionaryKeyPA,        /**< (Optional; PDF 1.3) A URI action formerly associated with this annotation. */
    LinkAnnotationDictionaryKeyQuadPoints /**< (Optional; PDF 1.6) An array of 8*n numbers specifying the coordinates of nquadrilaterals in default user space that comprise the region in which the link should be activated. */
  };

  /**
   * @brief A link annotation represents either a hypertext link to a destination elsewhere in the document or an action to be performed.
   */
  class LinkAnnotationDictionary : public AnnotationDictionary{
  public:
    LinkAnnotationDictionary(void);
    ~LinkAnnotationDictionary(void);

    using AnnotationDictionary::addKey;
    void addKey(LinkAnnotationDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<LinkAnnotationDictionaryKey, std::pair<std::string , uint32_t>> mLinkAnnotationMap;

  private:
    LinkAnnotationDictionary(LinkAnnotationDictionary const &) = delete;
    LinkAnnotationDictionary &operator=(LinkAnnotationDictionary const &) = delete;
  };

}}
