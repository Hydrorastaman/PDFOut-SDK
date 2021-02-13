#include <graphics/image/ImageIndexedEncoder.h>

#include <ColorSpaceImpl.h>
#include <filter/Filter.h>
#include <graphics/ImageDictionary.h>

#include <object/ObjectName.h>
#include <object/ObjectInteger.h>

#include <ximage.h>

#undef min
#undef max
#include <algorithm>

using namespace pdfout;
using namespace pdfout::impl;

namespace kernel{ namespace graphics{ namespace image{

  ImageIndexedEncoder::ImageIndexedEncoder(ImageImpl const *image)
    : ImageEncoder(image){
  }

  ImageIndexedEncoder::~ImageIndexedEncoder(void) {}

  std::unique_ptr<ObjectIndirectReference> ImageIndexedEncoder::serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const{
    std::unique_ptr<ImageDictionary> imageDictionary = std::make_unique<ImageDictionary>();
    CxImage const *image = getImage();
    if (!image || !image->IsIndexed())
      return nullptr;

    std::unique_ptr<ObjectIndirectReference> ref = imageDictionary->getReference();

    imageDictionary->addKey(ImageDictionaryKeyWidth, std::make_unique<ObjectInteger>(image->GetWidth()));
    imageDictionary->addKey(ImageDictionaryKeyHeight, std::make_unique<ObjectInteger>(image->GetHeight()));

    uint16_t const bpp = std::min(image->GetBpp(), (uint16_t) 8);
    imageDictionary->addKey(ImageDictionaryKeyBitsPerComponent, std::make_unique<ObjectInteger>(bpp));

    if (!params->mCompressionFilterName.empty())
      imageDictionary->addKey(StreamDictionaryKeyFilter, std::make_unique<ObjectName>(compressionFilterToString(params->mCompressionFilterName)));

    RGBQUAD *palette = image->GetPalette();
    uint32_t const paletteSize = image->GetPaletteSize() / sizeof(RGBQUAD);

    std::vector<uint8_t> colors(paletteSize * 3);
    for (uint32_t i = 0; i < paletteSize; ++i){
      colors[i * 3 + 0] = palette[i].rgbRed;
      colors[i * 3 + 1] = palette[i].rgbGreen;
      colors[i * 3 + 2] = palette[i].rgbBlue;
    }

    ColorSpaceImpl *cs = dynamic_cast<ColorSpaceImpl *>(ColorSpace::createIndexed(ColorSpaceFamilyDeviceRGB, paletteSize - 1, colors.data(), 3));
    std::unique_ptr<ObjectIndirectReference> csRef = cs->serialize(stream, params);
    imageDictionary->addKey(ImageDictionaryKeyColorSpace, std::move(csRef));

    uint8_t *imageBuffer = nullptr;
    int32_t imageSize = 0;
    uint32_t const width = image->GetWidth();
    uint32_t const height = image->GetHeight();
    uint32_t const effWidth = image->GetEffWidth();
    uint8_t const *source = image->GetBits();

    for (int32_t i = height - 1; i >= 0; --i)
      imageDictionary->write(source + i * effWidth, sizeof(uint8_t), width);

    imageDictionary->serialize(stream, params);

    return ref;
  }

}}}
