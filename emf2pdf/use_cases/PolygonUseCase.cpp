#ifndef EMF2PDF_API_EXPORTS

#include "Emf2PdfUseCase.h"

TEST_F(Emf2PdfUseCase, Polygon16){
  std::wstring emfFile = emfRootDir + L"polygon16.emf";
  std::wstring pdfFile = L"polygon16.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, Polygon){
  std::wstring emfFile = emfRootDir + L"polygon.emf";
  std::wstring pdfFile = L"polygon.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, Polybezier16){
  std::wstring emfFile = emfRootDir + L"polybezier16.emf";
  std::wstring pdfFile = L"polybezier16.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, Polybezier){
  std::wstring emfFile = emfRootDir + L"polybezier.emf";
  std::wstring pdfFile = L"polybezier.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, PolybezierTo16){
  std::wstring emfFile = emfRootDir + L"polybezierto16.emf";
  std::wstring pdfFile = L"polybezierto16.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, PolybezierTo){
  std::wstring emfFile = emfRootDir + L"polybezierto.emf";
  std::wstring pdfFile = L"polybezierto.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, PolylineTo16){
  std::wstring emfFile = emfRootDir + L"polylineto16.emf";
  std::wstring pdfFile = L"polylineto16.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, PolylineTo){
  std::wstring emfFile = emfRootDir + L"polylineto.emf";
  std::wstring pdfFile = L"polylineto.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, Polyline16){
  std::wstring emfFile = emfRootDir + L"polyline16.emf";
  std::wstring pdfFile = L"polyline16.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, Polyline){
  std::wstring emfFile = emfRootDir + L"polyline.emf";
  std::wstring pdfFile = L"polyline.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, PolyPolygon16){
  std::wstring emfFile = emfRootDir + L"polypolygon16.emf";
  std::wstring pdfFile = L"polypolygon16.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, PolyPolygon){
  std::wstring emfFile = emfRootDir + L"polypolygon.emf";
  std::wstring pdfFile = L"polypolygon.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, PolyPolyline16){
  std::wstring emfFile = emfRootDir + L"polypolyline16.emf";
  std::wstring pdfFile = L"polypolyline16.pdf";

  playEmfFile(emfFile, pdfFile);
}

TEST_F(Emf2PdfUseCase, PolyPolyline){
  std::wstring emfFile = emfRootDir + L"polypolyline.emf";
  std::wstring pdfFile = L"polypolyline.pdf";

  playEmfFile(emfFile, pdfFile);
}

#endif
