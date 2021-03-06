#include <gtest/gtest.h>

#include <memory>

#include <Page.h>
#include <Font.h>
#include <UString.h>
#include <Document.h>
#include <ColorSpace.h>
#include <ContentStream.h>

using namespace pdfout;

class EncryptUseCase : public ::testing::Test{
};

TEST_F(EncryptUseCase, Aes128Encrypt){
  std::unique_ptr<Document> doc = std::make_unique<Document>();
  Page *page = doc->createPage(PaperSize_NA_Letter);
  doc->addPage(page);
  doc->setFontEmbedding(true);
  doc->setEncryption(EncryptionAlgorithmAESv2, UString(L"тест", Encoding_WCHAR_T), UString(L"тест", Encoding_WCHAR_T), UserAccessPrint | UserAccessCopy | UserAccessAccessible | UserAccessEdit);
  //doc->setCompressionFilter(FilterNameFlate);


  Font::addFontPath(L"C:/windows/Fonts/");
  Font *font = Font::createFont(L"arial.ttf");

  ContentStream *stream = page->getContents();
  stream->setColorSpace(ColorSpace::createDeviceRGB());
  stream->beginText();
    stream->setTextFont(font, 12);
    stream->setTextPosition(260, 550);
    stream->showText(UString("Hello, world!"));
  stream->endText();

  doc->saveToFile("EncryptUseCase_Aes128Encrypt.pdf");
}

TEST_F(EncryptUseCase, Aes256Encrypt){
  std::unique_ptr<Document> doc = std::make_unique<Document>();
  Page *page = doc->createPage(PaperSize_NA_Letter);
  doc->addPage(page);
  doc->setFontEmbedding(true);
  doc->setEncryption(EncryptionAlgorithmAESv3, "test", "test", UserAccessPrint | UserAccessCopy | UserAccessAccessible | UserAccessEdit);
  //doc->setCompressionFilter(FilterNameFlate);


  Font::addFontPath(L"C:/windows/Fonts/");
  Font *font = Font::createFont(L"arial.ttf");

  ContentStream *stream = page->getContents();
  stream->setColorSpace(ColorSpace::createDeviceRGB());
  stream->beginText();
    stream->setTextFont(font, 12);
    stream->setTextPosition(260, 550);
    stream->showText(UString("Hello, world!"));
  stream->endText();

  doc->saveToFile("EncryptUseCase_Aes256Encrypt.pdf");
}

TEST_F(EncryptUseCase, RC440Encrypt){
  std::unique_ptr<Document> doc = std::make_unique<Document>();
  Page *page = doc->createPage(PaperSize_NA_Letter);
  doc->addPage(page);
  doc->setFontEmbedding(true);
  doc->setEncryption(EncryptionAlgorithmRC4v1, "test", "test1", UserAccessPrint | UserAccessCopy | UserAccessEdit);
  //doc->setCompressionFilter(FilterNameFlate);

  Font::addFontPath(L"C:/windows/Fonts/");
  Font *font = Font::createFont(L"arial.ttf");

  ContentStream *stream = page->getContents();
  stream->setColorSpace(ColorSpace::createDeviceRGB());
  stream->beginText();
    stream->setTextFont(font, 12);
    stream->setTextPosition(260, 550);
    stream->showText(UString("Hello, world!"));
  stream->endText();

  doc->saveToFile("EncryptUseCase_Rc440Encrypt.pdf");
}

TEST_F(EncryptUseCase, RC4128Encrypt){
  std::unique_ptr<Document> doc = std::make_unique<Document>();
  Page *page = doc->createPage(PaperSize_NA_Letter);
  doc->addPage(page);
  doc->setFontEmbedding(true);
  doc->setEncryption(EncryptionAlgorithmRC4v2, "test", "test1", UserAccessPrint | UserAccessCopy | UserAccessEdit);
  //doc->setCompressionFilter(FilterNameFlate);

  Font::addFontPath(L"C:/windows/Fonts/");
  Font *font = Font::createFont(L"arial.ttf");

  ContentStream *stream = page->getContents();
  stream->setColorSpace(ColorSpace::createDeviceRGB());
  stream->beginText();
    stream->setTextFont(font, 12);
    stream->setTextPosition(260, 550);
    stream->showText(UString("Hello, world!"));
  stream->endText();

  doc->saveToFile("EncryptUseCase_Rc4128Encrypt.pdf");
}
