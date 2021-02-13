#include <gtest/gtest.h>

#include <memory>

#include <Page.h>
#include <Font.h>
#include <Tools.h>
#include <Document.h>
#include <ColorSpace.h>
#include <ContentStream.h>

using namespace pdfout;

class TextOutUseCase : public ::testing::Test{
};

TEST_F(TextOutUseCase, SimpleTextOut){
  std::unique_ptr<Document> doc = std::make_unique<Document>();
  Page *page = doc->createPage(getPaperBoundaries(PaperSize_NA_Letter));
  doc->addPage(page);
  doc->setCompressionFilter(FilterNameFlate);
  doc->setFontEmbedding(true);
  doc->setFontSubsetting(true);

  Font::addFontPath(L"C:/windows/Fonts/");
  Font *font = Font::createFont(L"arial.ttf");

  ContentStream *stream = page->getContents();
  stream->setColorSpace(ColorSpace::createDeviceRGB());
  stream->setTextFont(font, 12);
  stream->beginText();
    stream->setTextPosition(260, 550);
    stream->showText(UString(L"Hello, world! Привет, Мир!"));
  stream->endText();

  doc->saveToFile("TextOutUseCase_SimpleTextOut.pdf");
}

TEST_F(TextOutUseCase, AlignedTextOut){
  std::unique_ptr<Document> doc = std::make_unique<Document>();
  Page *page = doc->createPage(getPaperBoundaries(PaperSize_NA_Letter));
  doc->addPage(page);
  doc->setFontEmbedding(true);

  Font::addFontPath(L"C:/windows/Fonts/");
  Font *font = Font::createFont(L"arial.ttf");

  ContentStream *stream = page->getContents();
  stream->setColorSpace(ColorSpace::createDeviceRGB());
  struct Rectangle rect{10, 10, 200, 100};

  stream->setStrokingColorRGB(ColorNameRed);
  stream->addRectangle(rect);
  stream->closePath(PathPaintingOpCloseAndStroke);

  double const rWidht = getWidth(rect);
  double const rHeight = getHeight(rect);

  // horizontal line
  stream->beginPath(rect.mLowerLeftX, rect.mLowerLeftY + rHeight / 2.f);
  stream->addLine(rect.mUpperRightX, rect.mLowerLeftY + rHeight / 2.f);
  stream->closePath(PathPaintingOpCloseAndStroke);

  // vertical line
  stream->beginPath(rect.mLowerLeftX + rWidht / 2.f, rect.mLowerLeftY);
  stream->addLine(rect.mLowerLeftX + rWidht / 2.f, rect.mUpperRightY);
  stream->closePath(PathPaintingOpCloseAndStroke);

  stream->setTextFont(font, 12);

  stream->setColorRGB(ColorNameBlack);
  stream->beginText();
    stream->showText(UString("Left"), rect, AlignmentLeft | AlignmentVCenter);
    stream->showText(UString("Top"), rect, AlignmentTop | AlignmentHCenter);
    stream->showText(UString("Right"), rect, AlignmentRight | AlignmentVCenter);
    stream->showText(UString("Bottom"), rect, AlignmentBottom | AlignmentHCenter);
    stream->showText(UString("Center"), rect, AlignmentVCenter | AlignmentHCenter);
  stream->endText();

  stream->beginPath(0, 500);
    stream->addLine(1000, 500);
  stream->closePath(PathPaintingOpStroke);
  stream->beginPath(200, 400);
    stream->addLine(200, 600);
  stream->closePath(PathPaintingOpStroke);

  stream->beginText();
    stream->showText(UString(L"Top-Left"), 200, 500, AlignmentTop | AlignmentLeft);
    stream->showText(UString(L"Base-Center"), 200, 500, AlignmentBaseline | AlignmentHCenter);
    stream->showText(UString(L"Bottom-Right"), 200, 500, AlignmentBottom | AlignmentRight);
  stream->endText();

  doc->saveToFile("TextOutUseCase_AlignedTextOut.pdf");
}

TEST_F(TextOutUseCase, RotatedTextOut){
  std::unique_ptr<Document> doc = std::make_unique<Document>();
  Page *page = doc->createPage(getPaperBoundaries(PaperSize_NA_Letter));
  doc->addPage(page);
  //doc->setCompressionFilter(FilterNameFlate);
  doc->setFontEmbedding(true);

  Font::addFontPath(L"C:/windows/Fonts/");
  Font *font = Font::createFont(L"arial.ttf");

  ContentStream *stream = page->getContents();
  int i = 0;
  stream->beginText();
    stream->setTextFont(font, 12);
    stream->setTextPosition(260, 550);
    for (float angle = 0.f; angle < 2.f * ContentStream::Pi; angle += ContentStream::Pi / 20, ++i){
      stream->save();
        TransformationMatrix tm = TransformationMatrix::createTranslationMatrix(300, 500) * TransformationMatrix::createRotationMatrix(angle);
        stream->setCTM(tm);
        stream->setColorRGB((ColorName) i);
        stream->showText(UString("Rotated text"), 60, 0, AlignmentBaseline | AlignmentLeft);
      stream->restore();
    }

  stream->endText();

  doc->saveToFile("TextOutUseCase_RotatedTextOut.pdf");
}
