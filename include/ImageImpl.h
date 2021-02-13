#pragma once

#include <Image.h>

class CxImage;

namespace pdfout{
  class OutputStream;
}

namespace kernel{
  struct SerializeParams;
  class ObjectStream;
  class ObjectIndirectReference;

  template<typename T>
  class UserObjectHash;

  namespace graphics{ namespace image{
    class ImageEncoder;
  }}
}

namespace pdfout{ namespace impl{

  class ImageImpl : public Image{
  protected:
    explicit ImageImpl(pdfout::UString const &imageFileName);
    explicit ImageImpl(std::unique_ptr<CxImage> image);

  public:
    virtual ~ImageImpl(void);

    std::string const &getUID(void) const;

    std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

    /**
     * Creates image from file
     */
    static ImageImpl *createImage(pdfout::UString const &fileName, bool delayLoad = true);

    /**
     * Creates image from memory
     */
    static ImageImpl *createImage(void *imageData, uint64_t size);

    /**
     * Create image from CxImage object
     */
    static ImageImpl *createImage(std::unique_ptr<CxImage> image);

    /**
     * Sets image bits per pixel, can be one of the ::ImageBpp value
     */
    void setBpp(ImageBpp imageBpp);

    /**
     * Sets image compression, can be one of the ::ImageCompression value
     */
    void setCompression(ImageCompression imageCompression);

    /**
     * Sets image dithering, can be one of the ::ImageDithering value
     */
    void setDithering(ImageDithering imageDithering);

    /**
     * Set JPEG quality, for JPEG compression only
     * @param jpegQuality [in] JPEG quality in range [0 .. 100]
     */
    void setJpegQuality(uint8_t jpegQuality);

    /**
     * Set image transparency
     * @param transparency [in] Image transparency in range [0 .. 100], where 0 - opaque, 100 - transparent
     */
    void setTransparency(uint8_t transparency);

    /**
     * Gets image bpp
     */
    ImageBpp getBpp(void) const;

    /**
     * Gets image compression
     */
    ImageCompression getCompression(void) const;

    /**
     * Gets image dithering
     */
    ImageDithering getDithering(void) const;

    /**
     * Gets iamge width
     */
    uint32_t getWidth(void) const;

    /**
     * Gets image height
     */
    uint32_t getHeight(void) const;

    /**
     * Gets JPEG quiality, for JPEG compression only
     */
    uint8_t getJpegQuality(void) const;

    /**
     * Gets Image transparency
     */
    uint8_t getTransparency(void) const;
    bool isTransparency(void) const;

    CxImage const *getImage(void) const;

    static ImageImpl *rawCreateImageFromFile(pdfout::UString const &fileName, bool delayLoad = true);
    static ImageImpl *rawCreateImageFromMemory(void *imageData, uint64_t size);
    static ImageImpl *rawCreateImageFromCxImage(std::unique_ptr<CxImage> source);

  protected:
    void loadFromFile(void) const;

  private:
    static ImageImpl *findImage(std::string const &sha256);
    static void storeImage(ImageImpl *image, std::string const &sha256);

  private:
    ImageImpl(ImageImpl const &) = delete;
    ImageImpl &operator=(ImageImpl const &) = delete;

  private:
    std::unique_ptr<kernel::graphics::image::ImageEncoder> makeImageEncoder(ImageCompression imageCompression) const;

  protected:
    std::string mUID;

  private:
    UString mFileName;
    mutable std::unique_ptr<CxImage> mImage;
    ImageBpp mImageBpp;
    ImageCompression mImageCompression;
    ImageDithering mImageDithering;
    uint8_t mJpegQuality;
    uint8_t mTransparency;

  protected:
    static std::unique_ptr<kernel::UserObjectHash<ImageImpl>> mImages;
  };

}}
