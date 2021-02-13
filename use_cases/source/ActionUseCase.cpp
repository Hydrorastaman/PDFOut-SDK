//#include <gtest/gtest.h>
//
//#include <memory>
//
//#include <Document.h>
//#include <Page.h>
//#include <Font.h>
//#include <ContentStream.h>
//#include <controls/PushButton.h>
//#include <stream/SimpleOutputFileStream.h>
//#include <FormXObject.h>
//
//class ActionUseCase : public ::testing::Test{
//};
//
//TEST_F(ActionUseCase, UriAction){
//  std::unique_ptr<Document> doc(std::make_unique<Document>());
//  doc->setFontEmbedding(true);
//  Page *page = doc->createPage(PaperSize_NA_Letter);
//
//  UString::setDefaultEncoding(Encoding_UTF8);
//  Font::addFontPath(L"C:/windows/Fonts/");
//  Font *arialFont = Font::createFont(L"Arial.ttf");
//
//  struct Rectangle rect = {100, 100, 180, 120};
//  PushButton *button = doc->createPushButton(rect);
//  page->addControl(button);
//  button->setBkColor(Color::createRGB(0.f, 0.f, 1.f));
//  button->setBorderColor(Color::createRGB(0.5f, 0.5f, 0.f));
//  button->setNCaption(UString("Normal Caption", Encoding_CHAR));
//  button->setDCaption(UString("Down Caption", Encoding_CHAR));
//  button->setRCaption(UString("Rollover Caption", Encoding_CHAR));
//
//  FormXObject *obj0 = button->createAppearance();
//  ContentStream *cst0 = obj0->getContents();
//  cst0->save();
//    cst0->setColorRGB(1.f, 1.f, 1.f);
//    cst0->addRectangle(button->getRect());
//    cst0->closePath(PathPaintingOpCloseFillStrokeNW);
//    cst0->beginText();
//      cst0->setTextFont(arialFont, 12);
//      cst0->setColorRGB(0.f, 0.f, 0.f);
//      cst0->showText(L"PUSH IT!", rect, AlignmentHCenter | AlignmentVCenter );
//    cst0->endText();
//  cst0->restore();
//  button->setNormalAppearance(obj0, "Yes");
//  button->setDefaultApperance("Yes");
//
//  FormXObject *obj1 = button->createAppearance();
//  ContentStream *cst1 = obj1->getContents();
//  cst1->save();
//    cst1->setColorRGB(1.f, 1.f, 1.f);
//    cst1->addRectangle(button->getRect());
//    cst1->closePath(PathPaintingOpCloseFillStrokeNW);
//    cst1->beginText();
//      cst1->setTextFont(arialFont, 12);
//      cst1->setColorRGB(0.f, 0.f, 0.f);
//      cst1->showText(L"DOWN!", rect, AlignmentHCenter | AlignmentVCenter );
//    cst1->endText();
//  cst1->restore();
//
//  button->setDownAppearance(obj1, "Off");
//
//  FormXObject *obj2 = button->createAppearance();
//  ContentStream *cst2 = obj2->getContents();
//  cst2->save();
//    cst2->setColorRGB(1.f, 1.f, 1.f);
//    cst2->addRectangle(button->getRect());
//    cst2->closePath(PathPaintingOpCloseFillStrokeNW);
//    cst2->beginText();
//      cst2->setTextFont(arialFont, 12);
//      cst2->setColorRGB(0.f, 0.f, 0.f);
//      cst2->showText(L"ROLLOVER!", rect, AlignmentHCenter | AlignmentVCenter );
//    cst2->endText();
//  cst2->restore();
//
//  button->setRolloverAppearance(obj2, "Off");
//
//  doc->addPage(page);
//  doc->saveToFile("ActionUseCase_UriAction.pdf");
//}
