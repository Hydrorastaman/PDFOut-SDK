#include <graphics/image/SoftMaskImage.h>

#include <ToString.h>
#include <ColorSpace.h>
#include <object/ObjectInteger.h>
#include <graphics/ImageDictionary.h>

using namespace pdfout;

namespace kernel{ namespace graphics{ namespace image{

  SoftMaskImage::SoftMaskImage(uint32_t width, uint32_t height, uint8_t transparency)
    : mWidth(width),
    mHeight(height),
    mTransparency((uint8_t) ((25500 - 255 * transparency) / 100)){
  }

  SoftMaskImage::~SoftMaskImage(void) {}

  std::unique_ptr<ObjectIndirectReference> SoftMaskImage::serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const{
    std::unique_ptr<ImageDictionary> imageDictionary = std::make_unique<ImageDictionary>();
    std::unique_ptr<ObjectIndirectReference> ref = imageDictionary->getReference();

    imageDictionary->addKey(ImageDictionaryKeyWidth, std::make_unique<ObjectInteger>(mWidth));
    imageDictionary->addKey(ImageDictionaryKeyHeight, std::make_unique<ObjectInteger>(mHeight));
    imageDictionary->addKey(ImageDictionaryKeyColorSpace, std::make_unique<ObjectName>(toString(ColorSpaceFamilyDeviceGray)));
    imageDictionary->addKey(ImageDictionaryKeyBitsPerComponent, std::make_unique<ObjectInteger>(8));

    uint32_t const dataSize = mWidth * mHeight;
    std::vector<uint8_t> data(dataSize);
    memset(data.data(), mTransparency, dataSize);
    imageDictionary->write(data.data(), sizeof(uint8_t), dataSize);

    imageDictionary->serialize(stream, params);

    return ref;
  }

}}}
