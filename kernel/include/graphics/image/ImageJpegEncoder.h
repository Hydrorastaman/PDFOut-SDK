#pragma once

#include <graphics/image/ImageEncoder.h>

namespace pdfout{ namespace impl{

  class ImageImpl;

}}

namespace kernel{ namespace graphics{ namespace image{

  class ImageJpegEncoder : public ImageEncoder{
  public:
    ImageJpegEncoder(pdfout::impl::ImageImpl const *image);
    ~ImageJpegEncoder(void);

    std::unique_ptr<ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

  private:
    ImageJpegEncoder(ImageJpegEncoder const &) = delete;
    ImageJpegEncoder &operator=(ImageJpegEncoder const &) = delete;
  };

}}}
