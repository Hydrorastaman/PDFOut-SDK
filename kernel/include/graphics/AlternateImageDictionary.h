#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{

  enum AlternateImageDictionaryKey{
    AlternateImageDictionaryKeyImage,              /**< (Required) The image XObject for the alternate image. */
    AlternateImageDictionaryKeyDefaultForPrinting, /**< (Optional) A flag indicating whether this alternate image is the default version to be used for printing. */
    AlternateImageDictionaryKeyOC,                 /**< (Optional; PDF 1.5) An optional content group or optional content membership dictionary that facilitates the selection of which alternate image to use. */
  };

  /**
   * @brief Representation of alternate image
   */
  class AlternateImageDictionary : public ObjectDictionary{
  public:
    AlternateImageDictionary(void);
    ~AlternateImageDictionary(void) {}

    void addKey(AlternateImageDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<AlternateImageDictionaryKey, std::pair<std::string, uint32_t>> mAlternateImageDictionaryMap;

  private:
    AlternateImageDictionary(AlternateImageDictionary const &) = delete;
    AlternateImageDictionary &operator=(AlternateImageDictionary const &) = delete;
  };
}
