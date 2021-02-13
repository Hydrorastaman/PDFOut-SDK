#pragma once

#include <memory>
#include <object/ObjectIndirectReference.h>

class CxImage;

namespace pdfout{ namespace impl{
  class ImageImpl;
}}

namespace kernel{ namespace graphics{ namespace image{

  class ImageEncoder{
  protected:
    /**
      * CxImage compression algorithm
      */
    enum CxImageCompression{
      CxImageCompressionAutomatic = 0,              /**< Automatic */
      CxImageCompressionNone = 1,                   /**< None */
      CxImageCompressionCCITTHuffmanRLE = 2,        /**< CCITT Huffman RLE */
      CxImageCompressionCCITTGroup3FaxEncoding = 3, /**< CCITT Group 3 Fax Encoding */
      CxImageCompressionCCITTGroup4FaxEncoding = 4, /**< CCITT Group 4 Fax Encoding */
      CxImageCompressionLZW = 5,                    /**< LZW */
      CxImageCompressionJpegDCT = 7,                /**< Jpeg DCT */
      CxImageCompressionAdobeDeflate = 8,           /**< Adobe deflate */
      CxImageCompressionMacintoshRLE = 32773,       /**< Macintosh RLE */
      CxImageCompressionDeflate = 32946             /**< Deflate */
    };

  protected:
    explicit ImageEncoder(pdfout::impl::ImageImpl const *image);
    CxImage const *getImage(void) const;
    pdfout::impl::ImageImpl const *getImageHandler(void) const;

  public:
    virtual ~ImageEncoder(void);
    virtual std::unique_ptr<ObjectIndirectReference> serialize(pdfout::OutputStream *stream, SerializeParams *params) const = 0;

  private:
    ImageEncoder(ImageEncoder const &) = delete;
    ImageEncoder &operator=(ImageEncoder const &) = delete;

  private:
    pdfout::impl::ImageImpl const *mImage;
  };

  enum CxImageColorType{
    CxImageColorTypeIndexed = 1,
    CxImageColorTypeRGB = 2,
    CxImageColorTypeRGBA = 4
  };

}}}
