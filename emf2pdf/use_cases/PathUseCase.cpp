#ifndef EMF2PDF_API_EXPORTS

#include "Emf2PdfUseCase.h"

#include <Image.h>

TEST_F(Emf2PdfUseCase, FillPath){
  std::wstring emfFile = emfRootDir + L"fill_path.emf";
  std::wstring pdfFile = L"fill_path.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, AbortPath){
  std::wstring emfFile = emfRootDir + L"abort_path.emf";
  std::wstring pdfFile = L"abort_path.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, StrokePath){
  std::wstring emfFile = emfRootDir + L"stroke_path.emf";
  std::wstring pdfFile = L"stroke_path.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, StrokeAndFillPath){
  std::wstring emfFile = emfRootDir + L"stroke_and_fill_path.emf";
  std::wstring pdfFile = L"stroke_and_fill_path.pdf";

  playEmfFile(emfFile, pdfFile);
}

#endif
