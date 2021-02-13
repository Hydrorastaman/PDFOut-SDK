#ifndef EMF2PDF_API_EXPORTS

#include "Emf2PdfUseCase.h"

TEST_F(Emf2PdfUseCase, Arc){
  std::wstring emfFile = emfRootDir + L"arc.emf";
  std::wstring pdfFile = L"arc.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, ArcTo){
  std::wstring emfFile = emfRootDir + L"arcto.emf";
  std::wstring pdfFile = L"arcto.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, Chord){
  std::wstring emfFile = emfRootDir + L"chord.emf";
  std::wstring pdfFile = L"chord.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, Pie){
  std::wstring emfFile = emfRootDir + L"pie.emf";
  std::wstring pdfFile = L"pie.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, Ellipse){
  std::wstring emfFile = emfRootDir + L"ellipse.emf";
  std::wstring pdfFile = L"ellipse.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, Ellipse2){
  std::wstring emfFile = emfRootDir + L"ellipse2.emf";
  std::wstring pdfFile = L"ellipse2.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, AngleArc){
  std::wstring emfFile = emfRootDir + L"anglearc.emf";
  std::wstring pdfFile = L"anglearc.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, Figures){
  std::wstring emfFile = emfRootDir + L"figures.emf";
  std::wstring pdfFile = L"figures.pdf";

  playEmfFile(emfFile, pdfFile);
}

#endif
