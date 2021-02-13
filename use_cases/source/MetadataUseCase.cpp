#include <gtest/gtest.h>

#include <memory>
#include <string>

#include <UString.h>
#include <Document.h>

using namespace pdfout;

class MetadataUseCase : public ::testing::Test{
};

TEST_F(MetadataUseCase, DocumentMetadata){
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

  doc->addPage(page0);
  std::string const endl = "\r\n";

  std::stringstream ss;
  ss << "<?xpacket begin='' id='W5M0MpCehiHzreSzNTczkc9d'?>" << endl;
  ss << "<x:xmpmeta xmlns:x=\"adobe:ns:meta/\" x:xmptk=\"Adobe XMP Core 5.6-c015 81.157285, 2014/12/12-00:43:15\"> " << endl;
  ss << "   <rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"> " << endl;
  ss << "      <rdf:Description rdf:about=\"\"" << endl;
  ss << "            xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\"" << endl;
  ss << "            xmlns:xmpMM=\"http://ns.adobe.com/xap/1.0/mm/\"" << endl;
  ss << "            xmlns:dc=\"http://purl.org/dc/elements/1.1/\">" << endl;
  ss << "         <xmp:ModifyDate>2016-02-07T20:50:29+06:00</xmp:ModifyDate>" << endl;
  ss << "         <xmp:CreateDate>2016-02-07T20:50:29+06:00</xmp:CreateDate>" << endl;
  ss << "         <xmp:MetadataDate>2016-02-07T20:50:29+06:00</xmp:MetadataDate>" << endl;
  ss << "         <xmpMM:DocumentID>uuid:9e5da648-b072-480c-bc80-95cef49d75b9</xmpMM:DocumentID>" << endl;
  ss << "         <xmpMM:InstanceID>uuid:b44a9fdb-c194-40f6-80fe-fb686443464e</xmpMM:InstanceID>" << endl;
  ss << "         <dc:format>application/pdf</dc:format>" << endl;
  ss << "      </rdf:Description>" << endl;
  ss << "   </rdf:RDF>" << endl;
  ss << "</x:xmpmeta>" << endl;
  ss << "<?xpacket end='w'?>" << endl;

  pdfout::UString const metadata(ss.str().c_str(), pdfout::Encoding::Encoding_ASCII);
  XMPDocument const xmp(metadata);

  doc->setMetadata(xmp);
  //doc->setEncryption(EncryptionAlgorithmAESv3, "test", "test", UserAccessPrint | UserAccessCopy | UserAccessAccessible | UserAccessEdit, false);
  doc->saveToFile("MetadataUseCase_DocumentMetadata.pdf");
}
