#include <gtest/gtest.h>

#include <memory>
#include <string>

#include <UString.h>
#include <Document.h>

using namespace pdfout;

class PageUseCase : public ::testing::Test{
};

TEST_F(PageUseCase, PageOrienation){
  std::unique_ptr<Document> doc = std::make_unique<Document>();
  doc->setCompressionFilter(FilterNameFlate);
  doc->setFontEmbedding(true);
  doc->setFontSubsetting(true);

  Font::addFontPath(L"C:/windows/Fonts/");
  Font *font = Font::createFont(L"arial.ttf");

  Page *page0 = doc->createPage(PaperSize_ISO_A4, PaperOrientationPortrait);
  ContentStream *cs0 = page0->getContents();
  cs0->setColorSpace(ColorSpace::createDeviceRGB());
  cs0->setTextFont(font, 12);
  cs0->beginText();
    cs0->setTextPosition(10.f, 200.f);
    cs0->showText("ISO-A4, Portrait");
  cs0->endText();  

  Page *page1 = doc->createPage(PaperSize_NA_Letter, PaperOrientationLandscape);
  ContentStream *cs1 = page1->getContents();
  cs1->setColorSpace(ColorSpace::createDeviceRGB());
  cs1->setTextFont(font, 12);
  cs1->beginText();
    cs1->setTextPosition(10.f, 200.f);
    cs1->showText("NA-Letter, Landscape");
  cs1->endText();  

  doc->addPage(page0);
  doc->addPage(page1);

  doc->saveToFile("PageUseCase_PageOrienation.pdf");
}
