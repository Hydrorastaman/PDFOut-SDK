#ifndef EMF2PDF_API_EXPORTS

#include "Emf2PdfUseCase.h"

#include <Types.h>
#include <Tools.h>
#include <Image.h>
#include <Exception.h>
#include <ColorSpace.h>
#include <GenericHandle.h>

using namespace pdfout;
using namespace emf2pdf;

std::wstring Emf2PdfUseCase::emfRootDir = L"./emf/";
std::string  Emf2PdfUseCase::emfRootDirA = "./emf/";
Document Emf2PdfUseCase::mDocument = {};
FontStorage Emf2PdfUseCase::mFontStorage = {};

void Emf2PdfUseCase::SetUpTestCase(void){
  mDocument.setFontEmbedding(true);
  mDocument.setFontSubsetting(true);
  //mDocument.setCompressionFilter(FilterNameFlate);
}

void Emf2PdfUseCase::TearDownTestCase(void){
  mDocument.saveToFile(UString(L"Emf2PdfUseCase.pdf", Encoding_WCHAR_T));
}

void Emf2PdfUseCase::SetUp(void){
  mColorSpaceFamily = pdfout::ColorSpaceFamilyDeviceRGB;
  mJpegQuality = 80;
}

void Emf2PdfUseCase::TearDown(void){
}

void Emf2PdfUseCase::playEmfFile(std::wstring const &emfFile, std::wstring const &pdfFile) const{
  mFontStorage.loadEmbeddedFonts(emfFile.c_str());

  Emf2PdfEngine engine(&mFontStorage, mColorSpaceFamily);
  Page *page = mDocument.createPage({0});
  mDocument.addPage(page);
  engine.setJpegQuality(mJpegQuality);
  engine.enableIntercharacterSpacingFix(false);
  engine.enableUnicodeBidiAlgorithm(false);
  engine.convert(page, emfFile.c_str());

  Font::addFontPath(L"C:/windows/Fonts/");
  Font *font = Font::createFont(L"arial.ttf");
  ContentStream *cs = page->getContents();
  cs->save();
    cs->setTextFont(font, 12);
    cs->setColorRGB(ColorNameRed);
    cs->beginText();
      cs->setTextPosition(10, 20);
      cs->showText(UString(emfFile.c_str()));
    cs->endText();
  cs->restore();

  bool const drawEmfInBackground = false;

  if (drawEmfInBackground) {
    struct Rectangle mediaBox = {0.f};
    page->getMediaBox(mediaBox);
    float const width = getWidth(mediaBox);
    float const height = getHeight(mediaBox);
  
    Image *img = Image::createImage(UString(emfFile.c_str()));
    img->setTransparency(50);
    img->setJpegQuality(100);
    cs->save();
      cs->showImage(img, 0, 0, width, height, 0);
    cs->restore();
  }

  //EXPECT_NO_THROW(mEngine->convert(mPage, emfFile));
  //EXPECT_NO_THROW(mDocument->saveToFile(pdfFile));
}

uint32_t Emf2PdfUseCase::getGdiObjectsCount(void) const{
  Handle proc(OpenProcess(PROCESS_QUERY_INFORMATION, false, GetCurrentProcessId()));
  DWORD const count = GetGuiResources(proc, GR_GDIOBJECTS);
  return count;
}

#endif
