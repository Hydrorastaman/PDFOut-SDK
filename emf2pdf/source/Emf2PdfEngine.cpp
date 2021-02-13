#include <Emf2PdfEngine.h>

#include <Exception.h>
#include <Emf2PdfEngineImpl.h>

namespace emf2pdf{

  Emf2PdfEngine::Emf2PdfEngine(FontStorage *fontStorage, pdfout::ColorSpaceFamily colorSpace)
    : mEmf2PdfEngineImpl(new impl::Emf2PdfEngineImpl(fontStorage, colorSpace)){
  }

  Emf2PdfEngine::~Emf2PdfEngine(void){
    if (mEmf2PdfEngineImpl){
      delete mEmf2PdfEngineImpl;
      mEmf2PdfEngineImpl = nullptr;
    }
  }

  void Emf2PdfEngine::convert(pdfout::Page *page, wchar_t const *emfFile, float marginX, float marginY){
    if (mEmf2PdfEngineImpl)
      return mEmf2PdfEngineImpl->convert(page, emfFile, marginX, marginY);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Emf2PdfEngine::convert(pdfout::Page *page, HENHMETAFILE metafile, float marginX, float marginY){
    if (mEmf2PdfEngineImpl)
      return mEmf2PdfEngineImpl->convert(page, metafile, marginX, marginY);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Emf2PdfEngine::setJpegQuality(uint8_t jpegQuality){
    if (mEmf2PdfEngineImpl)
      return mEmf2PdfEngineImpl->setJpegQuality(jpegQuality);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Emf2PdfEngine::enableIntercharacterSpacingFix(bool flag){
    if (mEmf2PdfEngineImpl)
      return mEmf2PdfEngineImpl->enableIntercharacterSpacingFix(flag);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void Emf2PdfEngine::enableUnicodeBidiAlgorithm(bool flag){
    if (mEmf2PdfEngineImpl)
      return mEmf2PdfEngineImpl->enableUnicodeBidiAlgorithm(flag);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

}
