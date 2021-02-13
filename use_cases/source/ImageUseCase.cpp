#include <gtest/gtest.h>

#include <memory>

#include <Document.h>
#include <Page.h>
#include <Font.h>
#include <ContentStream.h>
#include <Image.h>
#include <ExtGState.h>
#include <Tools.h>
#include <ColorSpace.h>

using namespace pdfout;

class ImageUseCase : public ::testing::Test{
};

TEST_F(ImageUseCase, SimpleImageOut){
  std::unique_ptr<Document> doc = std::make_unique<Document>();
  Page *page = doc->createPage(PaperSize_NA_Letter);
  ContentStream *cs = page->getContents();
  Image *img = doc->createImage(UString("resource/image0.png", Encoding_CHAR));

  cs->showImage(img, 0, 100, img->getWidth(), img->getHeight(), 0.f);

  doc->addPage(page);
  doc->saveToFile("ImageUseCase_SimpleImageOut.pdf");
}

TEST_F(ImageUseCase, SimpleTransparentImageOut){
  std::unique_ptr<Document> doc = std::make_unique<Document>();
  Page *page = doc->createPage(PaperSize_NA_Letter);
  ContentStream *cs = page->getContents();

  Font::addFontPath(L"C:/windows/Fonts/");
  Font *font = Font::createFont(L"arial.ttf");

  ExtGState *gs = ExtGState::createExtGState();
  gs->setBlendMode(BlendModeNormal);
  cs->setExtGState(gs);

  cs->setColorSpace(ColorSpace::createDeviceRGB());
  cs->beginText();
    cs->setColorRGB(ColorNameRed);
    cs->setTextFont(font, 24);
    cs->setTextPosition(260, 550);
    cs->showText(UString("Transparent image!"));
  cs->endText();

  Image *img = doc->createImage(UString("resource/image0.png", Encoding_CHAR));
  img->setTransparency(50);

  struct Rectangle mediaBox = {0};
  page->getMediaBox(mediaBox);
  cs->showImage(img, 0, 100, getWidth(mediaBox), getHeight(mediaBox), 0.f);
  

  doc->addPage(page);
  doc->saveToFile("ImageUseCase_SimpleTransparentImageOut.pdf");
}

TEST_F(ImageUseCase, EncryptedImageOutRC440){
  std::unique_ptr<Document> doc = std::make_unique<Document>();
  doc->setEncryption(EncryptionAlgorithmRC4v1, "test", "test1", UserAccessPrint | UserAccessCopy | UserAccessAccessible | UserAccessEdit);
  Page *page = doc->createPage(PaperSize_NA_Letter);
  ContentStream *cs = page->getContents();

  Image *img = doc->createImage(UString("resource/image0.png", Encoding_CHAR));
  cs->showImage(img, 0, 100, img->getWidth(), img->getHeight(), 0.f);

  doc->addPage(page);
  doc->saveToFile("ImageUseCase_EncryptedImageOutRC440.pdf");
}

TEST_F(ImageUseCase, EncryptedImageOutRC4128){
  std::unique_ptr<Document> doc = std::make_unique<Document>();
  doc->setEncryption(EncryptionAlgorithmRC4v2, "test", "test1", UserAccessPrint | UserAccessCopy | UserAccessAccessible | UserAccessEdit);
  Page *page = doc->createPage(PaperSize_NA_Letter);
  ContentStream *cs = page->getContents();

  Image *img = doc->createImage(UString("resource/image0.png", Encoding_CHAR));
  cs->showImage(img, 0, 100, img->getWidth(), img->getHeight(), 0.f);

  doc->addPage(page);
  doc->saveToFile("ImageUseCase_EncryptedImageOutRC4128.pdf");
}

TEST_F(ImageUseCase, EncryptedImageOutAes128){
  std::unique_ptr<Document> doc = std::make_unique<Document>();
  doc->setEncryption(EncryptionAlgorithmAESv2, "test", "test1", UserAccessPrint | UserAccessCopy | UserAccessAccessible | UserAccessEdit);
  Page *page = doc->createPage(PaperSize_NA_Letter);
  ContentStream *cs = page->getContents();

  Image *img = doc->createImage(UString("resource/image0.png", Encoding_CHAR));
  cs->showImage(img, 0, 100, img->getWidth(), img->getHeight(), 0.f);

  doc->addPage(page);
  doc->saveToFile("ImageUseCase_EncryptedImageOutAes128.pdf");
}
