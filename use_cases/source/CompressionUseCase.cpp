#include <gtest/gtest.h>

#include <memory>

#include <Page.h>
#include <Font.h>
#include <Image.h>
#include <Document.h>
#include <ColorSpace.h>
#include <ContentStream.h>

using namespace pdfout;

class CompressionUseCase : public ::testing::Test{
};

TEST_F(CompressionUseCase, FlateCompression){

  std::unique_ptr<Document> doc = std::make_unique<Document>();

  doc->setCompressionFilter(FilterNameFlate);
  doc->setFontEmbedding(true);
  Page *page = doc->createPage(PaperSize_NA_Letter);

  Font::addFontPath(L"C:/windows/Fonts/");
  Font *font = Font::createFont(L"arial.ttf");

  ContentStream *stream = page->getContents();
  stream->setColorSpace(ColorSpace::createDeviceRGB());
  stream->beginText();
    stream->setTextFont(font, 12);
    stream->setTextPosition(260, 550);
    stream->showText(UString("Hello, world!"));
  stream->endText();

  doc->addPage(page);
  doc->saveToFile("CompressionUseCase_FlateCompression.pdf");
}
