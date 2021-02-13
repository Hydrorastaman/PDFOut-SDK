#pragma once

#include <object/ObjectStream.h>

namespace kernel{

  enum ImageDictionaryKey{
    ImageDictionaryKeyWidth,            /**< (Required) The width of the image, in samples. */
    ImageDictionaryKeyHeight,           /**< (Required) The height of the image, in samples. */
    ImageDictionaryKeyColorSpace,       /**< (Required for images, except those that use the JPXDecode filter; not allowed for image masks) The color space in which image samples are specified; it can be any type of color space except Pattern. */
    ImageDictionaryKeyBitsPerComponent, /**< (Required except for image masks and images that use the JPXDecode filter)The number of bits used to represent each color component. */
    ImageDictionaryKeyIntent,           /**< (Optional; PDF 1.1) The name of a color rendering intent to be used in rendering the image. */
    ImageDictionaryKeyImageMask,        /**< (Optional) A flag indicating whether the image is to be treated as an image mask. */
    ImageDictionaryKeyMask,             /**< (Optional except for image masks; not allowed for image masks; PDF 1.3)An image XObject defining an image mask to be applied to this image. */
    ImageDictionaryKeyDecode,           /**< (Optional) An array of numbers describing how to map image samples into the range of values appropriate for the image's color space. */
    ImageDictionaryKeyInterpolate,      /**< (Optional) A flag indicating whether image interpolation is to be performed. */
    ImageDictionaryKeyAlternates,       /**< (Optional; PDF 1.3) An array of alternate image dictionaries for this image. */
    ImageDictionaryKeySMask,            /**< (Optional; PDF 1.4) A subsidiary image XObject defining a soft-mask image. */
    ImageDictionaryKeySMaskInData,      /**< (Optional for images that use the JPXDecode filter, meaningless otherwise; PDF 1.5) A code specifying how soft-mask information encoded with image samples should be used. */
    ImageDictionaryKeyName,             /**< (Required in PDF 1.0; optional otherwise) The name by which this image XObject is referenced in the XObject subdictionary of the current resource dictionary. */
    ImageDictionaryKeyStructParent,     /**< (Required if the image is a structural content item; PDF 1.3) The integer key of the image's entry in the structural parent tree. */
    ImageDictionaryKeyID,               /**< (Optional; PDF 1.3; indirect reference preferred) The digital identifier of the image's parent Web Capture content set. */
    ImageDictionaryKeyOPI,              /**< (Optional; PDF 1.2) An OPI version dictionary for the image. */
    ImageDictionaryKeyMetadata,         /**< (Optional; PDF 1.4) A metadata stream containing metadata for the image. */
    ImageDictionaryKeyOC                /**< (Optional; PDF 1.5) An optional content group or optional content membership dictionary. */
  };

  /**
   * @brief Representation of image XObject
   */
  class ImageDictionary : public ObjectStream{
  public:
    ImageDictionary(void);
    ~ImageDictionary(void) {}

    using ObjectStream::addKey;
    void addKey(ImageDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<ImageDictionaryKey, std::pair<std::string, uint32_t>> mImageDictionaryMap;

  private:
    ImageDictionary(ImageDictionary const &) = delete;
    ImageDictionary &operator=(ImageDictionary const &) = delete;
  };
}
