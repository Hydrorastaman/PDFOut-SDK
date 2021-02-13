#pragma once

#include <Image.h>
#include <graphics/image/ImageEncoder.h>

namespace pdfout{ namespace impl{

  class ImageImpl;

}}

namespace kernel{ namespace graphics{ namespace image{

  class ImageTiffEncoder : public ImageEncoder{
  public:
    ImageTiffEncoder(pdfout::impl::ImageImpl const *image, pdfout::ImageCompression compression, pdfout::ImageDithering dithering);
    ~ImageTiffEncoder(void);

    std::unique_ptr<ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

  private:
    ImageTiffEncoder(ImageTiffEncoder const &) = delete;
    ImageTiffEncoder &operator=(ImageTiffEncoder const &) = delete;

  private:
    pdfout::ImageCompression mCompression;
    pdfout::ImageDithering mDithering;
  };

}}}
