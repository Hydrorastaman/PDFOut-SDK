#include <gtest/gtest.h>

#include <memory>

#include <Font.h>
#include <Page.h>
#include <Document.h>
#include <ContentStream.h>

using namespace pdfout;

class PathConstructionUseCase : public ::testing::Test{
};

TEST_F(PathConstructionUseCase, SimplePath){
  std::unique_ptr<Document> doc = std::make_unique<Document>();
  doc->setFontEmbedding(true);
  Page *page = doc->createPage(PaperSize_NA_Letter);

  UString::setDefaultEncoding(Encoding_UTF8);
  Font::addFontPath(L"C:/windows/Fonts/");
  Font *arialFont = Font::createFont(L"Arial.ttf");

  ContentStream *cs = page->getContents();

  cs->beginPath(10, 10);
  cs->addLine(123, 322);
  cs->addBezierCurve12(32, 92, 132, 932, 523, 32);
  cs->closeSubPath();
  cs->closePath(PathPaintingOpCloseFillStrokeEO);

  doc->addPage(page);
  doc->saveToFile("PathConstructionUseCase_SimplePath.pdf");
}
