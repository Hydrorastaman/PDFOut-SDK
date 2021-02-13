#include <graphics/image/ImageTiffEncoder.h>

#include <ToString.h>
#include <ImageImpl.h>
#include <filter/Filter.h>
#include <ColorSpaceImpl.h>
#include <graphics/ImageDictionary.h>
#include <graphics/CCITTFaxDecodeDictionary.h>

#include <object/ObjectName.h>
#include <object/ObjectArray.h>
#include <object/ObjectInteger.h>
#include <object/ObjectBoolean.h>
#include <filter/FilterDeflate.h>
#include <stream/MemoryIStream.h>

#include <ximage.h>

using namespace pdfout;
using namespace pdfout::impl;

namespace kernel{ namespace graphics{ namespace image{

  ImageTiffEncoder::ImageTiffEncoder(ImageImpl const *image, ImageCompression compression, ImageDithering dithering)
    : ImageEncoder(image),
    mCompression(compression),
    mDithering(dithering){
  }

  ImageTiffEncoder::~ImageTiffEncoder(void) {}

  std::unique_ptr<ObjectIndirectReference> ImageTiffEncoder::serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const{
    std::unique_ptr<ImageDictionary> imageDictionary = std::make_unique<ImageDictionary>();
    //CxImage const *image = getImage();
    std::unique_ptr<CxImage> image = std::make_unique<CxImage>(*getImage());
    if (!image)
      return nullptr;

    std::unique_ptr<ObjectIndirectReference> ref = imageDictionary->getReference();

    imageDictionary->addKey(ImageDictionaryKeyWidth, std::make_unique<ObjectInteger>(image->GetWidth()));
    imageDictionary->addKey(ImageDictionaryKeyHeight, std::make_unique<ObjectInteger>(image->GetHeight()));
    imageDictionary->addKey(ImageDictionaryKeyColorSpace, std::make_unique<ObjectName>(toString(ColorSpaceFamilyDeviceGray)));
    imageDictionary->addKey(ImageDictionaryKeyBitsPerComponent, std::make_unique<ObjectInteger>(1));

    //std::unique_ptr<ObjectArray> filters = std::make_unique<ObjectArray>(IndirectTypeNonIndirectable);
    //filters->insert(std::make_unique<ObjectName>(toString(FilterNameFlate)));
    //filters->insert(std::make_unique<ObjectName>(toString(FilterNameCCITTFax)));
    //imageDictionary->addKey(StreamDictionaryKeyFilter, std::move(filters));
    imageDictionary->addKey(StreamDictionaryKeyFilter, std::make_unique<ObjectName>(toString(FilterNameCCITTFax)));
    //imageDictionary->addKey(ImageDictionaryKeyImageMask, std::make_unique<ObjectBoolean>(true));
    //int decodeArray[2] = {0, 1};
    //std::unique_ptr<ObjectArray> decode = std::make_unique<ObjectArray>(decodeArray, 2, IndirectTypeNonIndirectable);
    //imageDictionary->addKey(ImageDictionaryKeyDecode, std::move(decode));

    std::unique_ptr<CCITTFaxDecodeDictionary> decodeParms = std::make_unique<CCITTFaxDecodeDictionary>();
    //decodeParms->addKey(CCITFaxDecodeKeyBlackIs1, std::make_unique<ObjectBoolean>(false));
    //decodeParms->addKey(CCITFaxDecodeKeyEndOfBlock, std::make_unique<ObjectBoolean>(false));
    //decodeParms->addKey(CCITFaxDecodeKeyEncodedByteAlign, std::make_unique<ObjectBoolean>(false));
    decodeParms->addKey(CCITFaxDecodeKeyRows, std::make_unique<ObjectInteger>(image->GetHeight()));
    decodeParms->addKey(CCITFaxDecodeKeyColumns, std::make_unique<ObjectInteger>(image->GetWidth()));

    CxImageCompression cxImageCompression = CxImageCompressionNone;

    switch (mCompression){
      case ImageCompressionCCITTFaxGroup3:
        decodeParms->addKey(CCITFaxDecodeKeyK, std::make_unique<ObjectInteger>(0));
        cxImageCompression = CxImageCompressionCCITTGroup3FaxEncoding;
        break;

      case ImageCompressionCCITTFaxGroup4:
        decodeParms->addKey(CCITFaxDecodeKeyK, std::make_unique<ObjectInteger>(-1));
        cxImageCompression = CxImageCompressionCCITTGroup4FaxEncoding;
        break;
    }

    imageDictionary->addKey(StreamDictionaryKeyDecodeParms, std::move(decodeParms));

    uint8_t *imageBuffer = nullptr;
    int32_t imageSize = 0;

    try{
      image->Dither(mDithering);
      image->SetCodecOption(cxImageCompression, CXIMAGE_FORMAT_TIF);

      if (image->Encode(imageBuffer, imageSize, CXIMAGE_FORMAT_TIF)){

//std::unique_ptr<InputStream> istream = std::make_unique<MemoryIStream>(imageBuffer, imageSize);
//std::unique_ptr<Filter> deflateFilter = std::make_unique<filter::FilterDeflate>();
//deflateFilter->encode(istream.get(), imageDictionary->getStream());

        imageDictionary->write(imageBuffer, sizeof(uint8_t), imageSize);
        image->FreeMemory(imageBuffer);
      }
    }
    catch(...){
      if (imageBuffer)
        image->FreeMemory(imageBuffer);

      RUNTIME_EXCEPTION(std::string("failed to encode image to TIFF: ") + image->GetLastError());
    }

    imageDictionary->serialize(stream, removeCompression(params).get());

    return ref;
  }

}}}
