#pragma once

#include <string>
#include <cstdint>
#include <Windows.h>

#include <PaperSize.h>
#include <EmfHandle.h>

namespace emf2pdf{ namespace impl{

  class EmfHandleImpl{
  private:
    struct DeviceCaps{
      int mLogPixelsX;
      int mLogPixelsY;
      int mHorzRes;
      int mVertRes;
      int mHorzSize;
      int mVertSize;
    };

  public:
    EmfHandleImpl(float width, float height, int dpiX, int dpiY, wchar_t const *path);
    EmfHandleImpl(pdfout::PaperSize paperSize, pdfout::PaperOrientation paperOrientation, int dpiX, int dpiY, wchar_t const *path);
    EmfHandleImpl(HDC hdc, wchar_t const *path);
    EmfHandleImpl(wchar_t const *printerName, DEVMODEW const *devmode, wchar_t const *path);
    ~EmfHandleImpl(void);

    operator HDC(void) const;
    operator HENHMETAFILE(void);
    operator HBITMAP(void);

    void close(void);
    bool isClosed(void) const;

    wchar_t const *getPath(void) const;

    void setCallback(uint8_t index, EmfHandleCallback callback);
    EmfHandleCallback getCallback(uint8_t index) const;
    void clearCallback(void);

    void playEmfFile(HENHMETAFILE source, void *data, DisplayBounding bounding);
    void playEmfFile(wchar_t const *source, void *data, DisplayBounding bounding);
    int getDeviceCaps(int index) const;

    void saveAsJpeg(wchar_t const *path, uint8_t quality, bool isGrayscale);
    void saveAsTiff(wchar_t const *path, ImageBpp bpp, ImageCompression compression, ImageDithering dithering, bool isGrayscale);
    void saveAsPng(wchar_t const *path, ImageBpp bpp, ImageDithering dithering, bool isGrayscale);
    void saveAsBmp(wchar_t const *path, ImageBpp bpp, ImageDithering dithering, bool isGrayscale);

  private:
    void initialize(float width, float height, int16_t dpiX, int16_t dpiY, HDC hdc, uint16_t flags);
    void initialize(HDC hdc);

    HDC createWriteableEmf(HDC hdc, RECT size, wchar_t const *path, uint16_t flags);
    HDC createPrinterDC(wchar_t const *printerName, DEVMODEW const *devmode);
    RECT to01mm(HDC hdc, pdfout::PaperSize paperSize, pdfout::PaperOrientation paperOrientation) const;
    RECT to01mm(HDC hdc, float width, float height) const;

    ENHMETAHEADER getEnhMetaHeader(wchar_t const *source) const;
    RECT getEmfRclFrame(ENHMETAHEADER const &header) const;
    float getEmfDpiX(ENHMETAHEADER const &header) const;
    float getEmfDpiY(ENHMETAHEADER const &header) const;

    float getDeviceDpiX(HDC hdc) const; /**< Returns device physical DPI */
    float getDeviceDpiY(HDC hdc) const; /**< Returns device physical DPI */
    float getDeviceWidth(HDC hdc) const;   /**< Returns device physical width, in inches */
    float getDeviceHeight(HDC hdc) const;  /**< Returns device physical height, in inches */
    float getDeviceScaleX(HDC hdc) const;  /**< Returns device logical DPI / physical DPI */
    float getDeviceScaleY(HDC hdc) const;  /**< Returns device logical DPI / physical DPI */

    void playEnhMetafile(HENHMETAFILE source, void *data, HDC deviceDC, DisplayBounding bounding) const;
    void playEnhMetafile(wchar_t const *source, void *data, HDC deviceDC, DisplayBounding bounding) const;

    HBITMAP createBitmap(HDC hdc, uint32_t width, uint32_t height) const;
    HBITMAP getBitmap(void);

  private:
    EmfHandleImpl(EmfHandleImpl const &);
    EmfHandleImpl &operator=(EmfHandleImpl const &);

  private:
    HDC mHdc;
    HENHMETAFILE mEnhMetafile;
    HBITMAP mHBitmap;     // cashed HBITMAP for images
    std::wstring mPath;   // EMF target path
    bool mIsClosed;       // is EMF handle opened for drawing?
    float mWidth;         // source width, in inches
    float mHeight;        // source height, in inches
    DeviceCaps mDeviceCaps;
    EmfHandleCallback mCallbacks[EMR_MAX];

  private:
    static int CALLBACK playEnhMetafileProc(HDC hdc, HANDLETABLE *table, ENHMETARECORD const *emfr, int obj, LPARAM data);
  };
}}
