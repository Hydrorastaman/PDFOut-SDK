#include <ImageImpl.h>
#include <ResourceUid.h>

#include <InternalTools.h>
#include <UserObjectHash.h>
#include <UserObjectManager.h>

#include <graphics/ImageDictionary.h>
#include <graphics/image/ImageJpegEncoder.h>
#include <graphics/image/ImageTiffEncoder.h>
#include <graphics/image/ImageIndexedEncoder.h>

#include <object/ObjectStream.h>
#include <stream/SimpleInputFileStream.h>

#include <ximage.h>

#undef min
#undef max
#include <algorithm>

namespace{
  uint8_t const defaultJpegQuality = 100;
  uint8_t const defaultTransparency = 0;
}

namespace kernel{
  typedef UserObjectManager<pdfout::impl::ImageImpl> ImageManager;
}

using namespace kernel;
using namespace kernel::graphics::image;

namespace pdfout{ namespace impl{

  std::unique_ptr<kernel::UserObjectHash<ImageImpl>> ImageImpl::mImages(std::make_unique<kernel::UserObjectHash<ImageImpl>>());

  ImageImpl::ImageImpl(pdfout::UString const &imageFileName)
    : Image(),
    mUID(ResourceUid::getResourceUid(ResourceTypeImage)),
    mFileName(imageFileName),
    mImage(nullptr),
    mImageBpp(ImageBpp24),
    mImageCompression(ImageCompressionJpegDCT),
    mImageDithering(ImageDitheringFloydSteinberg),
    mJpegQuality(defaultJpegQuality),
    mTransparency(defaultTransparency){
  }

  ImageImpl::ImageImpl(std::unique_ptr<CxImage> image)
    : Image(),
    mUID(ResourceUid::getResourceUid(ResourceTypeImage)),
    mFileName(""),
    mImage(std::move(image)),
    mImageCompression(ImageCompressionJpegDCT),
    mJpegQuality(defaultJpegQuality),
    mTransparency(defaultTransparency){
  }

  ImageImpl::~ImageImpl(void) {}

  std::string const &ImageImpl::getUID(void) const {return mUID;}

  std::unique_ptr<ObjectIndirectReference> ImageImpl::serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const{
    if (!mImage)
      loadFromFile();

    std::unique_ptr<ImageEncoder> encoder(makeImageEncoder(mImageCompression));
    if (encoder)
      return encoder->serialize(stream, params);
    else
      RUNTIME_EXCEPTION("Invalid ImageCompression value");
  }

  ImageImpl *ImageImpl::createImage(pdfout::UString const &fileName, bool delayLoad){
    UString utf16be = fileName.encode(Encoding_UTF16BE);
    std::string const sha256 = getSHA256(utf16be.getData(), utf16be.getSize());
    ImageImpl *storedImage = findImage(sha256);
    if (storedImage)
      return storedImage;

    std::unique_ptr<ImageImpl> image(rawCreateImageFromFile(fileName, delayLoad));
    ImageImpl *loadedImage = ImageManager::getInstance().insert(std::move(image));
    storeImage(loadedImage, sha256);
    return loadedImage;
  }

  ImageImpl *ImageImpl::createImage(void *imageData, uint64_t size){
    std::string const sha256 = getSHA256(imageData, size);
    ImageImpl *storedImage = findImage(sha256);
    if (storedImage)
      return storedImage;
 
    std::unique_ptr<ImageImpl> image(rawCreateImageFromMemory(imageData, size));
    ImageImpl *loadedImage = ImageManager::getInstance().insert(std::move(image));
    storeImage(loadedImage, sha256);
    return loadedImage;
  }

  ImageImpl *ImageImpl::createImage(std::unique_ptr<CxImage> source){
    std::string const sha256 = getSHA256(source->GetDIB(), source->GetSize());
    ImageImpl *storedImage = findImage(sha256);
    if (storedImage)
      return storedImage;

    std::unique_ptr<ImageImpl> image(rawCreateImageFromCxImage(std::move(source)));
    ImageImpl *loadedImage = ImageManager::getInstance().insert(std::move(image));
    storeImage(loadedImage, sha256);
    return loadedImage;
  }

  void ImageImpl::setBpp(ImageBpp imageBpp) {mImageBpp = imageBpp;}

  void ImageImpl::setCompression(ImageCompression imageCompression) {mImageCompression = imageCompression;}

  void ImageImpl::setDithering(ImageDithering imageDithering) {mImageDithering = imageDithering;}

  void ImageImpl::setJpegQuality(uint8_t jpegQuality) {mJpegQuality = std::min(jpegQuality, (uint8_t) 100);}

  void ImageImpl::setTransparency(uint8_t transparency) {mTransparency = std::min(transparency, (uint8_t) 100);}

  ImageBpp ImageImpl::getBpp(void) const {return mImageBpp;}

  ImageCompression ImageImpl::getCompression(void) const {return mImageCompression;}

  ImageDithering ImageImpl::getDithering(void) const {return mImageDithering;}

  uint32_t ImageImpl::getWidth(void) const{
    if (!mImage)
      loadFromFile();

    return mImage->GetWidth();
  }

  uint32_t ImageImpl::getHeight(void) const{
    if (!mImage)
      loadFromFile();

    return mImage->GetHeight();
  }

  uint8_t ImageImpl::getJpegQuality(void) const {return mJpegQuality;}

  uint8_t ImageImpl::getTransparency(void) const {return mTransparency;}

  bool ImageImpl::isTransparency(void) const {return mTransparency != 0;}

  CxImage const *ImageImpl::getImage(void) const {return mImage.get();}

  void ImageImpl::loadFromFile(void) const{
    std::unique_ptr<SimpleInputFileStream> fstream = std::make_unique<SimpleInputFileStream>(mFileName);
    mImage = std::make_unique<CxImage>(fstream->getHandle(), CXIMAGE_FORMAT_UNKNOWN);
  }

  ImageImpl *ImageImpl::rawCreateImageFromFile(pdfout::UString const &fileName, bool delayLoad){
    std::unique_ptr<ImageImpl> image(new ImageImpl(fileName));
    if (!delayLoad)
      image->loadFromFile();

    return image.release();
  }

  ImageImpl *ImageImpl::rawCreateImageFromMemory(void *imageData, uint64_t size){
    if (!imageData || !size)
      RUNTIME_EXCEPTION("Invalid image source");

    std::unique_ptr<CxImage> source = std::make_unique<CxImage>((uint8_t *) imageData, size, CXIMAGE_FORMAT_UNKNOWN);
    std::unique_ptr<ImageImpl> image(new ImageImpl(std::move(source)));

    return image.release();
  }

  ImageImpl *ImageImpl::rawCreateImageFromCxImage(std::unique_ptr<CxImage> source){
    std::unique_ptr<ImageImpl> image(new ImageImpl(std::move(source)));
    return image.release();
  }

  ImageImpl *ImageImpl::findImage(std::string const &sha256){
    return mImages->find(sha256);
  }

  void ImageImpl::storeImage(ImageImpl *image, std::string const &sha256){
    mImages->store(image, sha256);
  }

  std::unique_ptr<ImageEncoder> ImageImpl::makeImageEncoder(ImageCompression imageCompression) const{
    switch (imageCompression){
      case ImageCompressionJpegDCT:
        return std::make_unique<ImageJpegEncoder>(this);

      case ImageCompressionCCITTFaxGroup3:
        return std::make_unique<ImageTiffEncoder>(this, ImageCompressionCCITTFaxGroup3, ImageDitheringFloydSteinberg);

      case ImageCompressionCCITTFaxGroup4:
        return std::make_unique<ImageTiffEncoder>(this, ImageCompressionCCITTFaxGroup4, ImageDitheringFloydSteinberg);

      case ImageCompressionIndexed:
        return std::make_unique<ImageIndexedEncoder>(this);

      default:
        NOT_IMPLEMENTED();
    }

    return nullptr;
  }

}}
