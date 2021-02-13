//#include <gtest/gtest.h>
//
//#include <memory>
//
//#include <Document.h>
//#include <Page.h>
//#include <Font.h>
//#include <ContentStream.h>
//#include <stream/SimpleOutputFileStream.h>
//#include <FormXObject.h>
//#include <annotations/TextAnnotation.h>
//#include <annotations/LinkAnnotation.h>
//
//class AnnotationUseCase : public ::testing::Test{
//};
//
//TEST_F(AnnotationUseCase, TextAnnotation){
//  std::unique_ptr<Document> doc(std::make_unique<Document>());
//  doc->setFontEmbedding(true);
//  Page *page = doc->createPage(PaperSize_NA_Letter);
//
//  UString::setDefaultEncoding(Encoding_UTF8);
//  Font::addFontPath(L"C:/windows/Fonts/");
//  Font *arialFont = Font::createFont(L"Arial.ttf");
//
//  struct Rectangle rect = {100, 100, 180, 120};
//  TextAnnotation *textAnnot = page->addTextAnnotation(rect, UString("text annotation", Encoding_CHAR));
//  textAnnot->setContents(UString("simple text annotation with Help icon", Encoding_CHAR));
//  textAnnot->setIconName(TextAnnotationIconHelp);
//
//  struct Rectangle rectOpen = {100, 200, 180, 220};
//  TextAnnotation *textAnnotOpen = page->addTextAnnotation(rectOpen, UString("opened text annotation", Encoding_CHAR));
//  textAnnotOpen->setContents(UString("simple OPENED text annotation with Comment icon", Encoding_CHAR));
//  textAnnotOpen->setIconName(TextAnnotationIconComment);
//  textAnnotOpen->setInitialState(true);
//
//  doc->addPage(page);
//  doc->saveToFile("AnnotationUseCase_TextAnnotation.pdf");
//}
//
//TEST_F(AnnotationUseCase, LinkAnnotation){
//  std::unique_ptr<Document> doc(std::make_unique<Document>());
//  doc->setFontEmbedding(true);
//  Page *page = doc->createPage(PaperSize_NA_Letter);
//  Page *page2 = doc->createPage(PaperSize_ISO_A4);
//
//  UString::setDefaultEncoding(Encoding_UTF8);
//  Font::addFontPath(L"C:/windows/Fonts/");
//  Font *arialFont = Font::createFont(L"Arial.ttf");
//
//  struct Rectangle rect = {100, 100, 180, 120};
//  LinkAnnotation *annot = page->addLinkAnnotation(rect, UString("link annotation", Encoding_CHAR));
//  annot->setDestination(page2, DestinationFitFull);
//  annot->setContents(UString("Link annotation to the page N2", Encoding_CHAR));
//  annot->setHighlighting(AnnotationHighlightingModePush);
//
//  doc->addPage(page);
//  doc->addPage(page2);
//  doc->saveToFile("AnnotationUseCase_LinkAnnotation.pdf");
//}
