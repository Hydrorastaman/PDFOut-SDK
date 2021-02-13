#pragma once

#include <graphics/image/ImageEncoder.h>

namespace pdfout{ namespace impl{

  class ImageImpl;

}}

namespace kernel{ namespace graphics{ namespace image{

  class ImageIndexedEncoder : public ImageEncoder{
  public:
    ImageIndexedEncoder(pdfout::impl::ImageImpl const *image);
    ~ImageIndexedEncoder(void);

    std::unique_ptr<ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

  private:
    ImageIndexedEncoder(ImageIndexedEncoder const &) = delete;
    ImageIndexedEncoder &operator=(ImageIndexedEncoder const &) = delete;
  };

}}}
