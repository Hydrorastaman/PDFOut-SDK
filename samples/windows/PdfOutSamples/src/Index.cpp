#include <Windows.h>

#include <Document.h>
#include <EmfHandle.h>
#include <Exception.h>
#include <Emf2PdfEngine.h>

void drawText(emf2pdf::EmfHandle const &handle, int x, int y, std::wstring const &text){
  using namespace pdfout;

  LOGFONT lf = {0};
  lf.lfHeight = -MulDiv(12, GetDeviceCaps(handle, LOGPIXELSY), 72);
  std::wstring const faceName = L"Arial";
  wcscpy_s(lf.lfFaceName, LF_FACESIZE, faceName.c_str());

  pdfout::ColorName const colors[] = {
    ColorNameAliceBlue,
    ColorNameAntiqueWhite,
    ColorNameAqua,
    ColorNameAquamarine,
    ColorNameAzure,
    ColorNameBeige,
    ColorNameBisque,
    ColorNameBlack,
    ColorNameBlanchedAlmond,
    ColorNameBlue,
    ColorNameBlueViolet,
    ColorNameBrown,
    ColorNameBurlyWood,
    ColorNameCadetBlue,
    ColorNameChartreuse,
    ColorNameChocolate,
    ColorNameCoral,
    ColorNameCornflowerBlue,
    ColorNameCornsilk,
    ColorNameCrimson,
    ColorNameCyan,
    ColorNameDarkBlue,
    ColorNameDarkCyan,
    ColorNameDarkGoldenRod,
    ColorNameDarkGray,
    ColorNameDarkGrey,
    ColorNameDarkGreen};

  SetGraphicsMode(handle, GM_ADVANCED);
  for (int i = 0, colorIndex = 0; i < 3600; i += 300, ++colorIndex){
    SaveDC(handle);
    SetBkMode(handle, TRANSPARENT);
    lf.lfEscapement = lf.lfOrientation = i;

    HFONT font = CreateFontIndirect(&lf);
    SelectObject(handle, font);

    Color color = Color::createRGB(colors[colorIndex]);
    SetTextColor(handle, RGB(color.mValue.mRGB.mRed * 255, color.mValue.mRGB.mGreen * 255, color.mValue.mRGB.mBlue * 255));
    TextOut(handle, x, y, text.c_str(), text.length());

    RestoreDC(handle, -1);
  }
}

int main(int argc, char const *argv[]){

  try{
    // Creates document object
    std::unique_ptr<pdfout::Document> document = std::make_unique<pdfout::Document>();

    // Creates page object and add it to the document
    struct pdfout::Rectangle mediaBox = {0};
    pdfout::Page *page = document->createPage(mediaBox);
    document->addPage(page);

    // Creates in-memory handle with A4 paper format
    pdfout::PaperSize paperSize = pdfout::PaperSize_ISO_A4;

    emf2pdf::EmfHandle handle(paperSize, pdfout::PaperOrientationPortrait, 600, 600);
    float const dpiX = (float) GetDeviceCaps(handle, LOGPIXELSX);
    float const dpiY = (float) GetDeviceCaps(handle, LOGPIXELSY);

    // Scale factor between logical DPI and physical DPI. Actual for screen-based handles.
    float const scale = dpiX / (GetDeviceCaps((HDC) handle, HORZRES) * 25.4f / GetDeviceCaps((HDC) handle, HORZSIZE));

    int const resX = GetDeviceCaps(handle, HORZRES);
    int const resY = GetDeviceCaps(handle, VERTRES);

    // draw some elements on the EMF handle
    Rectangle(handle, 100, 100, resX - 100, resY - 100);
    drawText(handle, 300.f * scale, 300.f * scale, L"         Hello, world!");

    // Creates EMF to PDF engine and draws EMF handle on PDF page
    emf2pdf::Emf2PdfEngine engine;
    engine.convert(page, handle);

    // Creates images from closed handle
    handle.saveAsBmp(L"PdfOutSamples.bmp");
    handle.saveAsJpeg(L"PdfOutSamples.jpeg");
    handle.saveAsPng(L"PdfOutSamples.png");
    handle.saveAsTiff(L"PdfOutSamples.tiff");

    // Gets page content stream for an additional drawing if required
    pdfout::ContentStream *cs = page->getContents();

    // Enables PDF compression, currently supports only flate encoding
    document->setCompressionFilter(pdfout::FilterNameFlate);

    // Enables font embedding and subsetting for better size
    document->setFontEmbedding(true);
    document->setFontSubsetting(true);

    // Sets document information
    document->setTitle("Document title");
    document->setAuthor("Document author");
    document->setCreator("Document creator");
    document->setSubject("Document subject");
    document->setKeywords("keyword keyword keyword");
    document->setProducer("Document producer");

    pdfout::DateTime const now(pdfout::TimeZoneUTC0200P);
    document->setCreationDate(now);
    document->setModificationDate(now);

    // Saves document to file on disk    
    document->saveToFile(L"PdfOutSamples.pdf");
  }
  catch (pdfout::Exception &ex){
    fprintf(stderr, "pdfout::Exception: %s\n", ex.what());
  }
  catch (std::exception &ex){
    fprintf(stderr, "std::exception: %s\n", ex.what());
  }

  return 0;
}

