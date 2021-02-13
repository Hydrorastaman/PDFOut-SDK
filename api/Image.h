#pragma once

#include <memory>
#include <string>

#include "UString.h"
#include "PdfOutConfig.h"

namespace pdfout{

  /**
   * Image Bits per pixel
   */
  enum ImageBpp{
    ImageBpp1 =  1, /**< 1 Bpp */
    ImageBpp4 =  4, /**< 4 Bpp */
    ImageBpp8 =  8, /**< 8 Bpp */
    ImageBpp24 = 24 /**< 24 Bpp */
  };

  /**
    * Image dithering algorithm
    */
  enum ImageDithering{
    ImageDitheringFloydSteinberg = 0, /**< Floyd-Steinberg algorithm */
    ImageDitheringOrderedDithering,   /**< Ordered dithering algorithm, fastest */
    ImageDitheringBurkes,             /**< Burkes algorithm */
    ImageDitheringStucki,             /**< Stucki algorithm */
    ImageDitheringJarvisJudiceNinke,  /**< Jarvis-Judice-Ninke algorithm */
    ImageDitheringSierra,             /**< Sierra algorithm */
    ImageDitheringStevensonArce,      /**< Stevenson-Arce algorithm */
    ImageDitheringBayer               /**< Bayer algorithm */
  };

  /**
    * Image compression algorithm
    */
  enum ImageCompression{
    ImageCompressionCCITTFaxGroup3, /**< CCITT Fax Group 3*/
    ImageCompressionCCITTFaxGroup4, /**< CCITT Fax Group 4*/
    ImageCompressionJBIG2,          /**< JBIG2 */
    ImageCompressionJpegDCT,        /**< Jpeg DCT */
    ImageCompressionJPX,            /**< Jpeg 2000 */
    ImageCompressionPNG,            /**< PNG */
    ImageCompressionIndexed         /**< Indexed images */
  };

  class PDFOUT_EXPORTS Image{
  protected:
    Image(void) {}

  public:
    virtual ~Image(void) {}

    /**
     * Creates image from file
     */
    static Image *createImage(pdfout::UString const &fileName, bool delayLoad = true);

    /**
     * Creates image from memory
     */
    static Image *createImage(void *imageData, uint64_t size);

    /**
     * Sets image bits per pixel, can be one of the ::ImageBpp value
     */
    virtual void setBpp(ImageBpp imageBpp) = 0;

    /**
     * Sets image compression, can be one of the ::ImageCompression value
     */
    virtual void setCompression(ImageCompression imageCompression) = 0;

    /**
     * Sets image dithering, can be one of the ::ImageDithering value
     */
    virtual void setDithering(ImageDithering imageDithering) = 0;

    /**
     * Set JPEG quality, for JPEG compression only
     * @param jpegQuality [in] JPEG quality in range [0 .. 100]
     */
    virtual void setJpegQuality(uint8_t jpegQuality) = 0;

    /**
     * Set image transparency
     * @param transparency [in] Image transparency in range [0 .. 100], where 0 - opaque, 100 - transparent
     */
    virtual void setTransparency(uint8_t transparency) = 0;

    /**
     * Gets image bpp
     */
    virtual ImageBpp getBpp(void) const = 0;

    /**
     * Gets image compression
     */
    virtual ImageCompression getCompression(void) const = 0;

    /**
     * Gets image dithering
     */
    virtual ImageDithering getDithering(void) const = 0;

    /**
     * Gets iamge width
     */
    virtual uint32_t getWidth(void) const = 0;

    /**
     * Gets image height
     */
    virtual uint32_t getHeight(void) const = 0;

    /**
     * Gets JPEG quiality, for JPEG compression only
     */
    virtual uint8_t getJpegQuality(void) const = 0;

    /**
     * Gets image transparency
     */
    virtual uint8_t getTransparency(void) const = 0;
    virtual bool isTransparency(void) const = 0;

  private:
    Image(Image const &);
    Image &operator=(Image const &);
  };

}
