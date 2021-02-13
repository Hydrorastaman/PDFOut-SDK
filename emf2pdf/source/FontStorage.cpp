#include <FontStorage.h>
#include <Exception.h>
#include <FontStorageImpl.h>

namespace emf2pdf{

  FontStorage::FontStorage(void)
    : mFontStorageImpl(new impl::FontStorageImpl()){
  }

  FontStorage::~FontStorage(void){
    if (mFontStorageImpl){
      delete mFontStorageImpl;
      mFontStorageImpl = nullptr;
    }
  }

  void FontStorage::loadEmbeddedFonts(wchar_t const *emfFile){
    if (mFontStorageImpl)
      return mFontStorageImpl->loadEmbeddedFonts(emfFile);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void FontStorage::loadEmbeddedFonts(HENHMETAFILE metafile){
    if (mFontStorageImpl)
      return mFontStorageImpl->loadEmbeddedFonts(metafile);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  pdfout::Font *FontStorage::getCurrentFont(HDC hdc, uint32_t ufi, TextInfo &textInfo){
    if (mFontStorageImpl)
      return mFontStorageImpl->getCurrentFont(hdc, ufi, textInfo);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void FontStorage::storeEmbeddedFont(pdfout::Font *font, wchar_t const *facename, uint32_t ufi, bool isBold, bool isItalic){
    if (mFontStorageImpl)
      return mFontStorageImpl->storeEmbeddedFont(font, facename, ufi, isBold, isItalic);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  std::size_t FontStorage::getEmbeddedFontsCount(void) const{
    if (mFontStorageImpl)
      return mFontStorageImpl->getEmbeddedFontsCount();

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  pdfout::Font const *FontStorage::getEmbeddedFont(std::size_t index) const{
    if (mFontStorageImpl)
      return mFontStorageImpl->getEmbeddedFont(index);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  std::size_t FontStorage::getSystemFontsCount(void) const{
    if (mFontStorageImpl)
      return mFontStorageImpl->getSystemFontsCount();

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  pdfout::Font const *FontStorage::getSystemFont(std::size_t index) const{
    if (mFontStorageImpl)
      return mFontStorageImpl->getSystemFont(index);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

}
