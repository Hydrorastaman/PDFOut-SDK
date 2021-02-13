#include <graphics/image/ImageJpegEncoder.h>

#include <ToString.h>
#include <ImageImpl.h>
#include <ColorSpace.h>
#include <filter/Filter.h>
#include <graphics/ImageDictionary.h>
#include <graphics/image/SoftMaskImage.h>

#include <object/ObjectName.h>
#include <object/ObjectInteger.h>

#include <ximage.h>

using namespace pdfout;
using namespace pdfout::impl;

namespace kernel{ namespace graphics{ namespace image{

  ImageJpegEncoder::ImageJpegEncoder(ImageImpl const *image)
    : ImageEncoder(image){
  }

  ImageJpegEncoder::~ImageJpegEncoder(void) {}

  std::unique_ptr<ObjectIndirectReference> ImageJpegEncoder::serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const{
    CxImage const *image = getImage();
    if (!image)
      return nullptr;

    ImageImpl const *imageHandler = getImageHandler();
    image->SetJpegQuality(imageHandler->getJpegQuality());

    std::unique_ptr<ObjectIndirectReference> imageMaskRef(nullptr);
    if (imageHandler->isTransparency()){
      std::unique_ptr<SoftMaskImage> softMaskImage = std::make_unique<SoftMaskImage>(imageHandler->getWidth(), imageHandler->getHeight(), imageHandler->getTransparency());
      imageMaskRef = softMaskImage->serialize(stream, params);
    }

    std::unique_ptr<ImageDictionary> imageDictionary = std::make_unique<ImageDictionary>();
    std::unique_ptr<ObjectIndirectReference> ref = imageDictionary->getReference();

    imageDictionary->addKey(ImageDictionaryKeyWidth, std::make_unique<ObjectInteger>(image->GetWidth()));
    imageDictionary->addKey(ImageDictionaryKeyHeight, std::make_unique<ObjectInteger>(image->GetHeight()));

    if (image->IsGrayScale())
      imageDictionary->addKey(ImageDictionaryKeyColorSpace, std::make_unique<ObjectName>(toString(ColorSpaceFamilyDeviceGray)));
    else
      imageDictionary->addKey(ImageDictionaryKeyColorSpace, std::make_unique<ObjectName>(toString(ColorSpaceFamilyDeviceRGB)));

    imageDictionary->addKey(ImageDictionaryKeyBitsPerComponent, std::make_unique<ObjectInteger>(8));
    imageDictionary->addKey(StreamDictionaryKeyFilter, std::make_unique<ObjectName>(toString(FilterNameDCT)));

    if (imageMaskRef)
      imageDictionary->addKey(ImageDictionaryKeySMask, std::move(imageMaskRef));

    uint8_t *imageBuffer = nullptr;
    int32_t imageSize = 0;

    try{
      if (image->Encode(imageBuffer, imageSize, CXIMAGE_FORMAT_JPG)){
        imageDictionary->write(imageBuffer, sizeof(uint8_t), imageSize);
        image->FreeMemory(imageBuffer);
      }
      else
        throw std::runtime_error("");
    }
    catch(...){
      if (imageBuffer)
        image->FreeMemory(imageBuffer);

      RUNTIME_EXCEPTION(std::string("failed to encode image to JPEG: ") + image->GetLastError());
    }

    imageDictionary->serialize(stream, removeCompression(params).get());

    return ref;
  }

}}}
