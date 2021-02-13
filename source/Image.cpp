#include <Image.h>
#include <ImageImpl.h>

namespace pdfout{

  Image *Image::createImage(pdfout::UString const &fileName, bool delayLoad){
    return pdfout::impl::ImageImpl::createImage(fileName, delayLoad);
  }

  Image *Image::createImage(void *imageData, uint64_t size){
    return pdfout::impl::ImageImpl::createImage(imageData, size);
  }

}
