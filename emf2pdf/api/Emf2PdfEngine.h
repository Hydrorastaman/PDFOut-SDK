#pragma once

#include <string>

#include <Windows.h>

#include <Page.h>
#include <ColorSpace.h>
#include "Emf2PdfConfig.h"
#include "FontStorage.h"

namespace emf2pdf{

  namespace impl{
    class Emf2PdfEngineImpl;
  }

  class EMF2PDF_EXPORTS Emf2PdfEngine{
  public:
    /**
     Creates PDF with specified color space, supported are ColorSpaceFamilyDeviceRGB, ColorSpaceFamilyDeviceGray, ColorSpaceFamilyDeviceCMYK.

     @param fontStorage Fonts cache
     @param colorSpace Color space

     @code {.cpp}
       Document document;
       Page *page = document.createPage({0});
       document.addPage(page);

       Emf2PdfEngine engine;
       engine.convert(page, L"source.emf");

       document.saveToFile(L"source.pdf");
     @endcode
     */
    Emf2PdfEngine(FontStorage *fontStorage = nullptr, pdfout::ColorSpaceFamily colorSpace = pdfout::ColorSpaceFamilyDeviceRGB);

    ~Emf2PdfEngine(void);

    /**
     Converts existing EMF file to PDF page.

     @param page Existing PDF page
     @param emfFile Path to EMF file
     @param marginX Horizontal margin in inches
     @param marginY Vertical margin in inches

     @code {.cpp}
       Emf2PdfEngine engine;
       engine.convert(page, L"source.emf");
     @endcode
     */
    void convert(pdfout::Page *page, wchar_t const *emfFile, float marginX = 0.f, float marginY = 0.f);

    /**
     Converts existing metafile handle to PDF page.

     @param page Existing PDF page
     @param metafile Handle of EMF file
     @param marginX Horizontal margin in inches
     @param marginY Vertical margin in inches

     @code {.cpp}
       Emf2PdfEngine engine;
       HENHMETAFILE metafile = getMetafile();
       engine.convert(page, metafile);
     @endcode
     */
    void convert(pdfout::Page *page, HENHMETAFILE metafile, float marginX = 0.f, float marginY = 0.f);

    /**
     Sets JPEG quality in range [0 .. 100].

     @code {.cpp}
       Emf2PdfEngine engine;
       engine.setJpegQuality(80);
       engine.convert(page, L"source.emf");
     @endcode
     */
    void setJpegQuality(uint8_t jpegQuality);

    /**
     Recalculates intercharacter spacing to average value

     @code {.cpp}
       Emf2PdfEngine engine;
       engine.enableIntercharacterSpacingFix(true);
       engine.convert(page, L"source.emf");
     @endcode
     */
    void enableIntercharacterSpacingFix(bool flag);

    /**
     Enables Unicode Bidirectional Algorithm v12.0.0

     @code {.cpp}
       Emf2PdfEngine engine;
       engine.enableUnicodeBidiAlgorithm(true);
       engine.convert(page, L"source.emf");
     @endcode
     */
    void enableUnicodeBidiAlgorithm(bool flag);

  private:
    Emf2PdfEngine(Emf2PdfEngine const &);
    Emf2PdfEngine &operator=(Emf2PdfEngine const &);

  private:
    impl::Emf2PdfEngineImpl *mEmf2PdfEngineImpl;
  };
}
