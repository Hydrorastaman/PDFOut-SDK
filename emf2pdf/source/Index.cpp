#ifndef EMF2PDF_API_EXPORTS

#include <gtest/gtest.h>

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

#endif

//
//int main1(int argc, char const *argv[]){
//  //EmfToPdf converter;
//
//  try{
//    //std::unique_ptr<Document> doc = std::make_unique<Document>();
//    //doc->setFontEmbedding(true);
//    //#ifndef _DEBUG
//    //  doc->setCompressionFilter(FilterNameFlate);
//    //#endif // _DEBUG
//
//    std::wstring emfCosunys = L"d:/tmp/emf2pdf.cosynus.emf";
//    std::wstring emfTaxcode = L"d:/tmp/emf2pdf.taxcode.emf";
//    std::wstring emfFroba = L"d:/tmp/emf2pdf.froba.emf";
//    std::wstring emfDolphin = L"d:/tmp/emf2pdf.dolphin.emf";
//    std::wstring emfDevicelock = L"d:/tmp/emf2pdf.devicelock.emf";
//    std::wstring emfDevicelock2 = L"d:/tmp/emf2pdf.devicelock2.emf";
//    std::wstring emfDashdev = L"d:/tmp/emf2pdf.dashdev.emf";
//    std::wstring emfTest = L"d:/tmp/emf2pdf.test.emf";
//
//    //std::vector<std::wstring> source;
//    //source.push_back(emfCosunys);
//    //source.push_back(emfTaxcode);
//    //source.push_back(emfFroba);
//    //source.push_back(emfDolphin);
//    //source.push_back(emfDevicelock);
//    //source.push_back(emfDevicelock2);
//    //source.push_back(emfDashdev);
//    //source.push_back(emfTest);
//
//std::vector<std::wstring> source{
//L"0000.emf",
//L"0000_.emf",
//L"00000.emf",
//L"00000000.emf",
////L"00000symbol.emf", // MakeFontDictionary not implemented
//L"00000tmp.emf",
//L"0001.emf",
//L"0002.emf",
//L"0003.emf",
//L"0004.emf",
//L"0005.emf",
//L"0006.emf",
//L"0007.emf",
//L"0007_.emf",
//L"0008.emf",
//L"0008_.emf",
//L"0009.emf",
//L"0010.emf",
////L"0011.emf",// MakeFontDictionary not implemented
//L"0012.emf",
//L"0012_.emf",
//L"0013.emf",
//L"0013_.emf",
//L"0014.emf",
//L"0015.emf",
//
////L"0016.emf", // TTC FONT
////L"0017.emf", // TTC FONT
////L"0018.emf", // TTC FONT
//
//L"0019.emf",
//L"0020.emf",
//L"0021.emf",
//L"0022.emf",
//L"0023.emf",
//L"0024.emf",
//L"0025.emf",
//L"0026.emf",
//L"0027.emf",
//
//// L"0028.emf", // TTC FONT
////L"0029.emf",  // TTC FONT
////L"0030.emf",  // TTC FONT
//
//L"0031.emf",
//L"0032.emf",
//L"0033.emf",
//L"0034.emf",
//L"0035.emf",
//L"0036.emf",
//L"0037.emf",
//// L"0038.emf", // bitmap load failed
////L"0039.emf", // jpeg decoder failed
////L"0040.emf", // // MakeFontDictionary not implemented
//L"0041.emf",
//L"0042.emf",
//L"0043.emf",
//L"0044.emf",
//L"0045.emf",
////L"0046.emf",// MakeFontDictionary not implemented
//L"0047.emf",
//L"0048.emf",
//L"0049.emf",
//L"0050.emf",
//L"0051.emf",
//L"0052.emf",
//L"0053.emf",
//L"0054.emf",
//L"0055.emf",
//L"0056.emf",
//L"0057.emf",
//L"0058.emf",
//L"0059.emf",
////L"0060.emf",// MakeFontDictionary not implemented
//
////L"0061.emf", // TTC FONT
//L"0062.emf",
//L"0063.emf",
//L"0064.emf",
//L"0065.emf",
//L"0066.emf",
//L"0067.emf",
////L"0068.EMF",// MakeFontDictionary not implemented
//L"0069.emf",
////L"0070.emf", // TTC FONT
//L"0071.emf",
//L"0072.emf",
//L"0073.emf",
//L"0073_.emf",
//L"0074.emf",
//L"0075.emf",
//L"0076.emf",
//L"0077.emf",
////L"0078.emf",// MakeFontDictionary not implemented
////L"0079.emf",// MakeFontDictionary not implemented
////L"0080.emf", // TTC FONT
//L"0081.emf",
////L"0082.emf", // TTC FONT
//L"0083.emf",
////L"0084.emf", // TTC FONT
//L"0085.emf",
//L"0086.emf",
//// L"0087.emf", // TTC FONT
//L"0088.emf",
//L"0089.emf",
//L"0090.emf",
//L"0091.emf",
//L"0092.emf",
//L"0093.emf",
//L"0094.emf",
//L"0095.emf",
////L"0096.EMF",// MakeFontDictionary not implemented
////L"0097.emf",// MakeFontDictionary not implemented
////L"0098.emf",// MakeFontDictionary not implemented
////L"0099.emf",// MakeFontDictionary not implemented
////L"0100.emf",// MakeFontDictionary not implemented
////L"0101.emf",// MakeFontDictionary not implemented
////L"0102.emf",// MakeFontDictionary not implemented
////L"0103.emf",// MakeFontDictionary not implemented
////L"0104.emf",// MakeFontDictionary not implemented
//L"0105.emf",
//L"0106.emf",
//L"0107.emf",
//L"0108.emf",
//L"0109.emf",
////L"0110.emf",// MakeFontDictionary not implemented
//L"0111.emf",
//L"0112.emf",
//L"0113.emf",
//L"0114.emf",
//L"0115.emf",
////L"0116.emf",// MakeFontDictionary not implemented
//L"0117.emf",
////L"0118.emf",// MakeFontDictionary not implemented
////L"0119.emf",// MakeFontDictionary not implemented
////L"0120.emf",// MakeFontDictionary not implemented
////L"0121.emf",// MakeFontDictionary not implemented
//L"0122.emf",
//L"0123.emf",
//L"0124.emf",
//L"0125.emf",
////L"0126.emf",// MakeFontDictionary not implemented
//L"0127.emf",
//L"0128.emf",
//L"0129.emf",
//L"0129_.emf",
//L"0130.emf",
//L"0131.emf",
////L"0132.emf",// MakeFontDictionary not implemented
////L"0133.emf", // FAILED TO LOAD EMBED FONT
////L"0134.emf",// MakeFontDictionary not implemented
//L"0135.emf",
////L"0136.emf",// MakeFontDictionary not implemented
////L"0137.emf",// MakeFontDictionary not implemented
//L"0138.emf",
////L"0139.emf",// MakeFontDictionary not implemented
//L"0140.emf",
////L"0141.emf", // Failed to create bitmap
//L"0142.emf",
////L"0143.emf",// MakeFontDictionary not implemented
//L"0144.emf",
//L"0145.emf",
////L"0146.emf",// MakeFontDictionary not implemented
//L"0147.emf",
////L"0148.emf",// MakeFontDictionary not implemented
//L"0149.emf",
//L"0150.emf",
//L"0151.emf",
////L"0152.emf", // failed to create bitmap
////L"0153.emf", // INVALID CMAP FORMAT
//L"0154.emf",
//L"0155.emf",
//L"0156.emf",
//L"0157.emf",
//L"0158.emf",
//L"0159.emf",
//L"0160.emf",
////L"0161.emf", // INVALID CMAP FORMAT
//L"0162.emf",
//L"0163.emf",
//L"0164.emf",
//L"0165.emf",
//L"0166.emf",
////L"0167.emf", // TTC FONT
//// L"0168.emf", // TTC FONT
//L"0169.emf",
//L"0170.emf",
//L"0171.emf",
//L"0172.emf",
//L"0173.emf",
//L"0175.emf",
//L"0176.emf",
//L"0177.emf",
//L"0178.emf",
//L"0179.emf",
//L"0180.emf",
//L"0181.emf",
//L"0182.emf",
//L"0183.emf",
//L"0184.emf",
//L"0185.emf",
//L"0186.emf",
//L"0187.emf",
//L"0188.emf",
//L"0189.emf",
//L"0190.emf",
//L"0191.emf",
//L"0192.emf",
//L"0193.emf",
//L"0194.emf",
//L"0195.emf",
//L"0196.emf",
//L"0197.emf",
//L"0198.emf",
//L"0199.emf",
//L"0200.emf",
//L"0201.emf",
//L"0202.emf",
//L"0203.emf",
//L"0204.emf",
//L"0205.emf",
//L"0206.emf",
//L"0207.emf",
//L"0208.emf",
//L"0209.emf",
//L"0210.emf",
//L"0211.emf",
//
//L"0212.emf",
//L"0213.emf",
//L"0214.emf",
//L"0215.emf",
//L"0217.emf",
//L"0218.emf",
//L"0219.emf",
//L"0220.emf",
//L"0221.emf",
//L"0222.emf",
//
////L"0223.emf", // makeFontDictionary() NOT IMPLEMENTED
//L"0224.emf",
//L"0225.emf",
//L"0226.emf",
//L"0227.emf",
//
//L"0228.emf",
//L"0229.emf",
//L"0230.emf",
//L"0231.emf",
//L"0232.emf"
//};
//
//    emf2pdf::Emf2PdfEngine engine;
//
//    PerformanceLogger logger;
//
//    for (std::size_t i = 0; i < 1; ++i){
//      std::wstring emfPath = L"d:/tmp/testcase/" + source[i];
//      fwprintf(stderr, L"processing: %s\n", emfPath.c_str());
//
//      try{
//        std::unique_ptr<Document> doc = std::make_unique<Document>();
//        doc->setFontEmbedding(true);
//        Page *page = doc->createPage({0, 0, 0, 0});
//        doc->addPage(page);
//
//        engine.convert(page, emfPath);
//
//        std::wstring pdfPath = L"d:/tmp/testcase/pdf/" + source[i] + L".pdf";
//        doc->saveToFile(UString(pdfPath.c_str(), Encoding_WCHAR_T));
//      }
//      catch (std::runtime_error &err){
//        fprintf(stderr, "%s\n", err.what());
//      }
//      catch (...){
//        fprintf(stderr, "unhandled exception\n");
//      }
//    }
//
//logger.startCounter("save to PDF");
//    //std::unique_ptr<OutputStream> fileStream(std::make_unique<SimpleOutputFileStream>("d:/tmp/emf2pdf.pdf"));
//    //doc->saveToStream(fileStream.get());
//logger.stopCounter("save to PDF");
//
//logger.flushToFile(L"d:/tmp/emf2pdf.log");
//  }
//  catch (std::runtime_error &err){
//    fprintf(stderr, "%s\n", err.what());
//  }
//  catch (...){
//    fprintf(stderr, "unhandled exception\n");
//  }
//
//  return 0;
//}
