#ifndef EMF2PDF_API_EXPORTS

#include "Emf2PdfUseCase.h"

#undef TextOut

TEST_F(Emf2PdfUseCase, TextOut){
  std::wstring emfFile = emfRootDir + L"textout.emf";
  std::wstring pdfFile = L"textout.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, TextOut2){
  std::wstring emfFile = emfRootDir + L"textout2.emf";
  std::wstring pdfFile = L"textout2.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, TextOut3){
  std::wstring emfFile = emfRootDir + L"textout3.emf";
  std::wstring pdfFile = L"textout3.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, Ligatures){
  std::wstring emfFile = emfRootDir + L"ligatures.emf";
  std::wstring pdfFile = L"ligatures.emf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, Logfont){
  std::wstring emfFile = emfRootDir + L"logfont.emf";
  std::wstring pdfFile = L"logfont.emf";

  playEmfFile(emfFile, pdfFile);
}

#endif
