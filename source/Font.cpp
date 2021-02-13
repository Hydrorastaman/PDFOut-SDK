#include <Font.h>
#include <FontImpl.h>

namespace pdfout{

  void Font::addFontPath(pdfout::UString const &fontPath){
    return impl::FontImpl::addFontPath(fontPath);
  }

  void Font::setFontPath(pdfout::UString const &fontPath){
    return impl::FontImpl::setFontPath(fontPath);
  }

  Font *Font::createFont(pdfout::UString const &fileName, WritingMode writingMode){
    return impl::FontImpl::createFont(fileName, writingMode);
  }

  Font *Font::createFont(void const *fontData, uint64_t size, WritingMode writingMode){
    return impl::FontImpl::createFont(fontData, size, writingMode);
  }

  Font *Font::createFont(InputStream *istream, WritingMode writingMode){
    return impl::FontImpl::createFont(istream, writingMode);
  }

  FontCollection *Font::createFontCollection(pdfout::UString const &fileName, WritingMode writingMode){
    return impl::FontImpl::createFontCollection(fileName, writingMode);
  }

  FontCollection *Font::createFontCollection(void const *fontData, uint64_t size, WritingMode writingMode){
    return impl::FontImpl::createFontCollection(fontData, size, writingMode);
  }

  FontCollection *Font::createFontCollection(InputStream *istream, WritingMode writingMode){
    return impl::FontImpl::createFontCollection(istream, writingMode);
  }

}
