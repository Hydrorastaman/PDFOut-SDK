#pragma once

#include <Windows.h>

#include <PaperSize.h>
#include "Emf2PdfConfig.h"

namespace emf2pdf{

  /**
   * Image bits per pixel values
   */
  enum ImageBpp{
    ImageBpp1 = 1,  /**< black-and-white image */
    ImageBpp4 = 4,  /**< 4-bit image */
    ImageBpp8 = 8,  /**< 8-bit image */
    ImageBpp24 = 24 /**< 24-bit image */
  };

  /**
   * Image dithering algorithm
   */
  enum ImageDithering{
    ImageDitheringFloydSteinberg = 0, /**< Floyd-Steinberg algorithm */
    ImageDitheringOrderedDithering,   /**< Ordered dithering algorithm */
    ImageDitheringBurkes,             /**< Burkes algorithm */
    ImageDitheringStucki,             /**< Stucki algorithm */
    ImageDitheringJarvisJudiceNinke,  /**< Jarvis-Judice-Ninke algorithm */
    ImageDitheringSierra,             /**< Sierra algorithm */
    ImageDitheringStevensonArce,      /**< Stevenson-Arce algorithm */
    ImageDitheringBayer               /**< Bayer algorithm */
  };

  /**
   * Image compression algorithm
   */
  enum ImageCompression{
    ImageCompressionAutomatic = 0,              /**< Automatic */
    ImageCompressionNone = 1,                   /**< None */
    ImageCompressionCCITTHuffmanRLE = 2,        /**< CCITT Huffman RLE */
    ImageCompressionCCITTGroup3FaxEncoding = 3, /**< CCITT Group 3 Fax Encoding */
    ImageCompressionCCITTGroup4FaxEncoding = 4, /**< CCITT Group 4 Fax Encoding */
    ImageCompressionLZW = 5,                    /**< LZW */
    ImageCompressionJpegDCT = 7,                /**< Jpeg DCT */
    ImageCompressionAdobeDeflate = 8,           /**< Adobe deflate */
    ImageCompressionMacintoshRLE = 32773,       /**< Macintosh RLE */
    ImageCompressionDeflate = 32946             /**< Deflate */
  };

  /**
   * Bounding rectangle used to display the existing EMF file or handle
   */
  enum DisplayBounding{
    DisplayBoundingDevice = 0, /**< Use device bounding */
    DisplayBoundingSource      /**< Use source bounding */
  };

  typedef void (*EmfHandleCallback)(HDC hdc, HANDLETABLE *table, ENHMETARECORD const *record, int obj, void *data);

  namespace impl{
    class EmfHandleImpl;
  }

  class EMF2PDF_EXPORTS EmfHandle{
  public:
    /**
     Creates EMF handle based on a screen device context.

     @warning GDI objects has 2GB limit, so width * height * dpiX * dpiY * 3 should be less than 2GB

     @param width Width of the picture to be stored in the enhanced metafile, in inches;
     @param height Height of the picture to be stored in the enhanced metafile, in inches;
     @param dpiX Number of pixels per logical inch along the handle width;
     @param dpiY Number of pixels per logical inch along the handle height;
     @param path A file name for the enhanced metafile to be created. If this parameter is null, the enhanced metafile is memory based and its contents are lost when it is deleted.

     @code {.cpp}
       EmfHandle handle(8.5, 11.0, 600, 600);
     @endcode
     */
    EmfHandle(float width, float height, int dpiX = 0, int dpiY = 0, wchar_t const *path = nullptr);

    /**
     Creates EMF handle based on a screen device context.

     @warning GDI objects has 2GB limit, so width * height * dpiX * dpiY * 3 should be less than 2GB

     @param paperSize A predefined size of the picture to be stored in the enhanced metafile, in inches;
     @param paperOrientation A predefined paper orientation;
     @param dpiX Number of pixels per logical inch along the handle width;
     @param dpiY Number of pixels per logical inch along the handle height;
     @param path A file name for the enhanced metafile to be created. If this parameter is null, the enhanced metafile is memory based and its contents are lost when it is deleted.

     @code {.cpp}
       EmfHandle handle(pdfout::PaperSize::PaperSize_ISO_A4, pdfout::PaperOrientation::PaperOrientationPortrait, 600, 600);
     @endcode
     */
    EmfHandle(pdfout::PaperSize paperSize, pdfout::PaperOrientation paperOrientation, int dpiX = 0, int dpiY = 0, wchar_t const *path = nullptr);

    /**
     Creates EMF handle based on a device context.

     @param hdc A handle to a reference device for the enhanced metafile. If parameter is null it uses the current display device for reference.
     @param path A file name for the enhanced metafile to be created. If this parameter is null, the enhanced metafile is memory based and its contents are lost when it is deleted.

     @code {.cpp}
       HDC device = GetDC(nullptr);
       EmfHandle handle(device);
       // ...
       ReleaseDC(device);
     @endcode
     */
    explicit EmfHandle(HDC hdc, wchar_t const *path = nullptr);

    /**
     Creates EMF handle based on a printer device context.

     @param printerName A name of a reference printer device for the enhanced metafile.
     @param devmode A pointer to a DEVMODE structure containing device-specific initialization data for the printer driver. If this parameter is null, device driver is to use the default initialization.
     @param path A file name for the enhanced metafile to be created. If this parameter is null, the enhanced metafile is memory based and its contents are lost when it is deleted.
     
     @code {.cpp}
       DEVMODEW devmode = {0};
       devmode.dmSize = sizeof(DEVMODE);
       devmode.dmFields = DM_PRINTQUALITY | DM_YRESOLUTION | DM_PAPERSIZE;
       devmode.dmPrintQuality = 300;
       devmode.dmYResolution = 300;
       devmode.dmPaperSize = DMPAPER_LETTER;
       
       EmfHandle handle(L"Device Name", &devmode);
     @endcode
     */
    explicit EmfHandle(wchar_t const *printerName, DEVMODEW const *devmode = nullptr, wchar_t const *path = nullptr);

    ~EmfHandle(void);

    /**
     Returns a handle to device context.

     @code {.cpp}
       EmfHandle handle(...);
       HDC hdc = (HDC) handle;
     @endcode
     */
    operator HDC(void) const;

    /**
     Closes an enhanced-metafile device context and returns a handle that identifies an enhanced-format metafile.

     @code {.cpp}
       EmfHandle handle(...);
       HENHMETAFILE metafile = (HENHMETAFILE) handle;
     @endcode
     */
    operator HENHMETAFILE(void);

    /**
     Closes an enhanced-metafile device context and returns a bitmap handle.

     @code {.cpp}
       EmfHandle handle(...);
       HBITMAP bitmap = (HBITMAP) handle;
     @endcode
     */
    operator HBITMAP(void);

    /**
     Closes an enhanced-metafile device context and creates EMF file for non-memory objects.

     @code {.cpp}
       EmfHandle handle(...);
       handle.playEmfFile(...);
       handle.close();
     @endcode
     */
    void close(void);

    /**
     Does a handle to device context is closed for drawing?

     @code {.cpp}
       EmfHandle handle(...);
       // ...
       if (!handle.isClosed())
         Draw(handle);
     @endcode
     */
    bool isClosed(void) const;

    /**
     Returns a null-terminated string with path to the EMF file. EMF file creates after closing an object.

     @code {.cpp}
       EmfHandle handle(..., L"path to EMF file");
       // ...
       auto path = handle.getPath();
     @endcode
     */
    wchar_t const *getPath(void) const;
  
    /**
     Sets callback function for specified EMF record type

     @param index Predefined EMF record type, see EMR_* defines in wingdi.h
     @param callback Specified callback
     
     @code {.cpp}
     void callback(HDC hdc, HANDLETABLE *table, ENHMETARECORD const *record, int obj, void *data){
       SaveDC(hdc);
         switch (record->iType){
           case EMR_HEADER:
             // ...
             PlayEnhMetaFileRecord(hdc, table, record, obj);
             break;
       
           case EMR_EOF:
             // ...
             PlayEnhMetaFileRecord(hdc, table, record, obj);
             break;
       
           default:
             PlayEnhMetaFileRecord(hdc, table, record, obj);
         }
       RestoreDC(hdc, -1);
     }
     
     HENHMETAFILE metafile = GetEnhMetaFileW(L"path to EMF file");
     
     EmfHandle handle(...);
     handle.setCallback(EMR_HEADER, callback);
     handle.setCallback(EMR_EOF, callback);
     handle.playEmfFile(metafile, nullptr);
     
     DeleteEnhMetaFile(metafile);
     @endcode
     */
    void setCallback(uint8_t index, EmfHandleCallback callback);

    /**
     Returns pointer to callback function

     @param index Predefined EMF record type, see EMR_* defines in wingdi.h

     @code {.cpp}
       EmfHandle handle(...);
       // ...
       auto callbackOnHeader = handle.getCallback(EMR_HEADER);
       if (callbackOnHeader)
         // ...
     @endcode
     */
    EmfHandleCallback getCallback(uint8_t index) const;

    /**
     Clears callback functions

     @code {.cpp}
       EmfHandle handle(...);
       // ...
       handle.clearCallback();
     @endcode
     */
    void clearCallback(void);

    /**
     Draws an existing EMF file on the handle.
     Call setCallback() to specify custom callbacks before creating an object.

     @param source HENHMETAFILE handle;
     @param data A pointer to optional callback-function data;

     @code {.cpp}
       class Test{
       public:
         // ...
         void Execute(){
           HENHMETAFILE source = getSource(...);
           EmfHandle handle(...);
           handle.playEmfFile(source, (void *) this);
         }
       // ...
       };
     @endcode
     */
    void playEmfFile(HENHMETAFILE source, void *data, DisplayBounding bounding = DisplayBoundingDevice);

    /**
     Draws an existing EMF file on the handle.
     Call setCallback() to specify custom callbacks before creating an object.

     @param source A path to an existing EMF file;
     @param data A pointer to optional callback-function data;

     @code {.cpp}
       class Test{
       public:
         // ...
         void Execute(){
           wchar_t const *source = getSource(...);
           EmfHandle handle(...);
           handle.playEmfFile(source, (void *) this);
         }
       // ...
       };
     @endcode
     */
    void playEmfFile(wchar_t const *source, void *data, DisplayBounding bounding = DisplayBoundingDevice);

    /**
     Returns logical device capabilities.

     @param index The item to be returned. One of the GetDeviceCaps(HDC hdc, int index) index value.

     @code {.cpp}
       EmfHandle handle(...);
       // ...
       auto const dpiX = handle.getDeviceCaps(LOGPIXELSX);
       auto const dpiY = handle.getDeviceCaps(LOGPIXELSY);
     @endcode
     */
    int getDeviceCaps(int index) const;

    /**
     Closes an enhanced-metafile device context and save as JPEG image.

     @param path A file name where image should be saved;
     @param quality Jpeg quality in range [0 .. 100];
     @param isGrayscale Does image should be grayscaled?

     @code {.cpp}
       EmfHandle handle(...);
       // ...
       handle.saveAsJpeg(L"image.jpeg", 80, true);
     @endcode
     */
    void saveAsJpeg(wchar_t const *path, uint8_t quality = 100, bool isGrayscale = false);

    /**
     Closes an enhanced-metafile device context and save as TIFF image.

     @param path A file name where image should be saved;
     @param bpp Bits per pixel, could be one of emf2pdf::ImageBpp value;
     @param compression Compression algorithm, could be one of emf2pdf::ImageCompression value;
     @param dithering Dithering algorithm for 1bpp images. Could be one of emf2pdf::ImageDithering value;
     @param isGrayscale Does image should be grayscaled?

     @code {.cpp}
       EmfHandle handle(...);
       // ...
       handle.saveAsTiff(L"image.tif");
     @endcode
     */
    void saveAsTiff(wchar_t const *path, ImageBpp bpp = ImageBpp24, ImageCompression compression = ImageCompressionAutomatic, ImageDithering dithering = ImageDitheringFloydSteinberg, bool isGrayscale = false);

    /**
     Closes an enhanced-metafile device context and save as PNG image.

     @param path A file name where image should be saved;
     @param bpp Bits per pixel, could be one of emf2pdf::ImageBpp value;
     @param dithering Dithering algorithm for 1bpp images. Could be one of emf2pdf::ImageDithering value;
     @param isGrayscale Does image should be grayscaled?

     @code {.cpp}
       EmfHandle handle(...);
       // ...
       handle.saveAsPng(L"image.png");
     @endcode
     */
    void saveAsPng(wchar_t const *path, ImageBpp bpp = ImageBpp24, ImageDithering dithering = ImageDitheringFloydSteinberg, bool isGrayscale = false);

    /**
     Closes an enhanced-metafile device context and save as BMP image.

     @param path A file name where image should be saved;
     @param bpp Bits per pixel, could be one of emf2pdf::ImageBpp value;
     @param dithering Dithering algorithm for 1bpp images. Could be one of emf2pdf::ImageDithering value;
     @param isGrayscale Does image should be grayscaled?

     @code {.cpp}
       EmfHandle handle(...);
       // ...
       handle.saveAsBmp(L"image.bmp");
     @endcode
     */
    void saveAsBmp(wchar_t const *path, ImageBpp bpp = ImageBpp24, ImageDithering dithering = ImageDitheringFloydSteinberg, bool isGrayscale = false);

  private:
    EmfHandle(EmfHandle const &);
    EmfHandle &operator=(EmfHandle const &);

  private:
    impl::EmfHandleImpl *mEmfHandleImpl;
  };

  /**
   Wrapper for WinAPI call GetDeviceCaps(HDC, int).

   @warning GetDeviceCaps(EmfHandle, index) will returns EmfHandle::getDeviceCaps() instead of GetDeviceCaps((HDC) EmfHandle, index) which returns system call for reference device.

   @code {.cpp}
     EmfHandle handle(...);
     // ...
     GetDeviceCaps(handle, LOGPIXELSX); // will return EmfHandle internal LOGPIXELSX value
     GetDeviceCaps((HDC) handle, LOGPIXELSX); // will return ::GetDeviceCaps(, LOGPIXELSX) which may differ than EmfHandle internal value
   @endcode

   */
  PDFOUT_EXPORTS int GetDeviceCaps(EmfHandle const &handle, int index);
}
