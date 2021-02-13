#ifndef EMF2PDF_API_EXPORTS

#include "Emf2PdfUseCase.h"

TEST_F(Emf2PdfUseCase, BitBlt){
  std::wstring emfFile = emfRootDir + L"bitblt.emf";
  std::wstring pdfFile = L"bitblt.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, StretchBlt){
  std::wstring emfFile = emfRootDir + L"stretchblt.emf";
  std::wstring pdfFile = L"stretchblt.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, MaskBlt){
  std::wstring emfFile = emfRootDir + L"maskblt.emf";
  std::wstring pdfFile = L"maskblt.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, TransparentBlt){
  std::wstring emfFile = emfRootDir + L"transparentblt.emf";
  std::wstring pdfFile = L"transparentblt.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, StretchDiBits){
  std::wstring emfFile = emfRootDir + L"stretchdibits.emf";
  std::wstring pdfFile = L"stretchdibits.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, PlgBlt){
  std::wstring emfFile = emfRootDir + L"plgblt.emf";
  std::wstring pdfFile = L"plgblt.pdf";

  playEmfFile(emfFile, pdfFile);
}

#endif
