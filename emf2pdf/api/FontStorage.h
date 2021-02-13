#pragma once

#include <Font.h>
#include <Windows.h>
#include "Emf2PdfConfig.h"

namespace emf2pdf{

  enum EmbedFontType{
    EmbedFontTypeEngine,
    EmbedFontTypeSubset,
    EmbedFontTypeDelta
  };

  struct EMF2PDF_EXPORTS TextInfo{
    bool mIsBold;
    bool mIsItalic;
    bool mIsUnderline;
    uint16_t mSize;
    uint16_t mGraphicsMode;
    int32_t mEscapement;
    int32_t mOrientation;
    pdfout::WritingMode mWritingMode;
  };

  namespace impl{
    class FontStorageImpl;
  }

  class EMF2PDF_EXPORTS FontStorage{
  public:
    static uint32_t const unknownUfi = -1;

    FontStorage(void);
    ~FontStorage(void);

    /**
     Loads embedded fonts from existing EMF file.

     @param emfFile Path to EMF file.

     @code {.cpp}
       FontStorage storage;
       storage.loadEmbeddedFonts(L"source.emf");
     @endcode
     */
    void loadEmbeddedFonts(wchar_t const *emfFile);

    /**
     Loads embedded fonts from metafile handle.

     @param metafile Handle of EMF file.

     @code {.cpp}
       FontStorage storage;
       HENHMETAFILE metafile = getMetafile();
       storage.loadEmbeddedFonts(metafile);
     @endcode
     */
    void loadEmbeddedFonts(HENHMETAFILE metafile);

    /**
     Retrieves a font that has been selected into the specified device content.

     @param hdc Device content;
     @param ufi Universal font ID;
     @param textInfo Extra text information;
     @return Pointer to Font object.

     @code {.cpp}
       FontStorage storage;
       HDC device = getDevice();
       TextInfo info;
       Font *font = storage.getCurrentFont(device, FontStorage::unknownUfi, info);
     @endcode
     */
    pdfout::Font *getCurrentFont(HDC hdc, uint32_t ufi, TextInfo &textInfo);

    /**
     Store embedded font.

     @param font Pointer to Font object;
     @param facename Typeface name of the font;
     @param ufi Universal font ID;
     @param isBold Does font has a bold style?
     @param isItalic Does font has an italic style?

     @code {.cpp}
       FontStorage storage;
       Font *font = loadFont();
       storage.storeEmbeddedFont(font, L"facename", 32, true, false);
     @endcode
     */
    void storeEmbeddedFont(pdfout::Font *font, wchar_t const *facename, uint32_t ufi, bool isBold, bool isItalic);

    /**
     Returns a count of embedded fonts.
     */
    std::size_t getEmbeddedFontsCount(void) const;

    /**
     Retrieves an embedded font by index.

     @code {.cpp}
       FontStorage storage;
       // ...
       for (std::size_t i = 0, count = storage.getEmbeddedFontsCount(); i < count; ++i){
         Font const *font = storage.getEmbeddedFont(i);
         // ...
       }
     @endcode
     */
    pdfout::Font const *getEmbeddedFont(std::size_t index) const;

    /**
     Returns a count of system fonts.
     */
    std::size_t getSystemFontsCount(void) const;

    /**
     Retrieves a system font by index.

     @code {.cpp}
       FontStorage storage;
       // ...
       for (std::size_t i = 0, count = storage.getSystemFontsCount(); i < count; ++i){
         Font const *font = storage.getSystemFont(i);
         // ...
       }
     @endcode
     */
    pdfout::Font const *getSystemFont(std::size_t index) const;

  private:
    FontStorage(FontStorage const &);
    FontStorage &operator=(FontStorage const &);

  private:
    impl::FontStorageImpl *mFontStorageImpl;
  };

}
