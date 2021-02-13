#include <graphics/image/ImageEncoder.h>

#include <ImageImpl.h>
#include <ximage.h>

using namespace pdfout::impl;

namespace kernel{ namespace graphics{ namespace image{

  ImageEncoder::ImageEncoder(ImageImpl const *image)
    : mImage(image){
  }

  ImageEncoder::~ImageEncoder(void) {}

  CxImage const *ImageEncoder::getImage(void) const{
    return mImage->getImage();
  }

  ImageImpl const *ImageEncoder::getImageHandler(void) const{
    return mImage;
  }

}}}

