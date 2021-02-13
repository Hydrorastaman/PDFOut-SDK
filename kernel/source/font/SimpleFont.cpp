#include <font/SimpleFont.h>

#include <Exception.h>
#include <stream/InputStream.h>
#include <font/TrueType/FontTrueType.h>

namespace kernel{ namespace font{

  SimpleFont::SimpleFont(void){
  }

  SimpleFont::~SimpleFont(void) noexcept{
  }

  std::unique_ptr<SimpleFont> SimpleFont::createSimpleFont(pdfout::InputStream *stream){
    using kernel::font::truetype::FontTrueType;

    std::unique_ptr<SimpleFont> font(nullptr);

    // Try to create a TrueType font
    try{
      std::unique_ptr<SimpleFont> font = std::make_unique<FontTrueType>(stream);
      return font;
    }
    catch (pdfout::Exception &ex){
    }

    // Add a creation of another font types here

    return font;
  }

}}
