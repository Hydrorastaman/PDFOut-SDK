#ifndef EMF2PDF_API_EXPORTS

#include "Emf2PdfUseCase.h"

#include <Image.h>

TEST_F(Emf2PdfUseCase, CreatePen){
  std::wstring emfFile = emfRootDir + L"create_pen.emf";
  std::wstring pdfFile = L"create_pen.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, ExtCreatePen){
  std::vector<std::wstring> files = {
    emfRootDir + L"ext_create_pen.emf",
    emfRootDir + L"ext_create_pen_300dpi.emf",
    emfRootDir + L"ext_create_pen_600dpi.emf",
    emfRootDir + L"ext_create_pen_1200dpi.emf"
  };

  std::wstring pdfFile = L"ext_create_pen.pdf";

  for (auto const &file : files)
    playEmfFile(file, pdfFile);
}

#endif
