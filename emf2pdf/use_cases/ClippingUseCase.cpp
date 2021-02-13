#ifndef EMF2PDF_API_EXPORTS

#include "Emf2PdfUseCase.h"

#include <Image.h>

TEST_F(Emf2PdfUseCase, SelectClipPathCopy){
  std::wstring emfFile = emfRootDir + L"select_clip_path_copy.emf";
  std::wstring pdfFile = L"select_clip_path_copy.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, SelectClipPathAnd){
  std::wstring emfFile = emfRootDir + L"select_clip_path_and.emf";
  std::wstring pdfFile = L"select_clip_path_and.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, SelectClipPathOr){
  std::wstring emfFile = emfRootDir + L"select_clip_path_or.emf";
  std::wstring pdfFile = L"select_clip_path_or.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, SelectClipPathXor){
  std::wstring emfFile = emfRootDir + L"select_clip_path_xor.emf";
  std::wstring pdfFile = L"select_clip_path_xor.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, SelectClipPathDiff){
  std::wstring emfFile = emfRootDir + L"select_clip_path_diff.emf";
  std::wstring pdfFile = L"select_clip_path_diff.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, SelectClipPath){
  std::wstring emfFile = emfRootDir + L"select_clip_path.emf";
  std::wstring pdfFile = L"select_clip_path.pdf";

  playEmfFile(emfFile, pdfFile);
}

#endif
