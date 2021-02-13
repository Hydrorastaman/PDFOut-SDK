#include <EmfHandleImpl.h>

#include <memory>
#include <vector>
#include <cstdint>
#include <algorithm>

#include <EmfTypes.h>
#include <Exception.h>
#include <GenericHandle.h>

#include <ximage.h>

#undef min
#undef max

using namespace emf2pdf;

namespace{
  uint16_t const flagForceDpiX = 0x1;
  uint16_t const flagForceDpiY = 0x2;

  std::vector<wchar_t> flagsToComments(emf2pdf::impl::EmfHandleImpl const &handle, uint16_t flags);

  std::unique_ptr<CxImage> createImage(HBITMAP bitmap);
  void setJpegQuality(CxImage *image, uint8_t quality);
  void setBpp(CxImage *image, ImageBpp bpp, ImageDithering dithering);
  void setGrayscale(CxImage *image);
  void setTiffCompression(CxImage *image, ImageCompression compression);
  void saveToFile(CxImage *image, ENUM_CXIMAGE_FORMATS format, wchar_t const *path);

  struct EnumData{
    emf2pdf::impl::EmfHandleImpl const *mHandle;
    void *mData;
  };

}

namespace emf2pdf{ namespace impl{

  EmfHandleImpl::EmfHandleImpl(float width, float height, int dpiX, int dpiY, wchar_t const *path)
    : mHdc(nullptr),
    mEnhMetafile(nullptr),
    mHBitmap(nullptr),
    mPath(path == nullptr ? L"" : path){

    Hdc hdc(GetDC(nullptr));
    initialize(width, height, dpiX, dpiY, hdc, flagForceDpiX | flagForceDpiY);
  }

  EmfHandleImpl::EmfHandleImpl(pdfout::PaperSize paperSize, pdfout::PaperOrientation paperOrientation, int dpiX, int dpiY, wchar_t const *path)
    : mHdc(nullptr),
    mEnhMetafile(nullptr),
    mHBitmap(nullptr),
    mPath(path == nullptr ? L"" : path){

    Hdc hdc(GetDC(nullptr));
    float const width = pdfout::getPaperWidth(paperSize, paperOrientation);
    float const height = pdfout::getPaperHeight(paperSize, paperOrientation);
    initialize(width, height, dpiX, dpiY, hdc, flagForceDpiX | flagForceDpiY);
  }

  EmfHandleImpl::EmfHandleImpl(HDC hdc, wchar_t const *path)
    : mHdc(nullptr),
    mEnhMetafile(nullptr),
    mHBitmap(nullptr),
    mPath(path == nullptr ? L"" : path){

    initialize(hdc);
  }

  EmfHandleImpl::EmfHandleImpl(wchar_t const *printerName, DEVMODEW const *devmode, wchar_t const *path)
    : mHdc(nullptr),
    mEnhMetafile(nullptr),
    mHBitmap(nullptr),
    mPath(path == nullptr ? L"" : path){

    Hdc printerHdc(createPrinterDC(printerName, devmode));
    initialize(printerHdc);
  }

  EmfHandleImpl::~EmfHandleImpl(void){
    if (mHBitmap)
      DeleteObject(mHBitmap);

    try{
      close();
      DeleteEnhMetaFile(mEnhMetafile);
    }
    catch (...){
    }
  }

  EmfHandleImpl::operator HDC(void) const{
    if (mIsClosed)
      RUNTIME_EXCEPTION("The handle is invalid, errcode: " + std::to_string(ERROR_INVALID_HANDLE));

    return mHdc;
  }

  EmfHandleImpl::operator HENHMETAFILE(void){
    close();
    return mEnhMetafile;
  }

  EmfHandleImpl::operator HBITMAP(void){
    return getBitmap();
  }

  void EmfHandleImpl::close(void){
    if (!mIsClosed){
      mIsClosed = true;
      mEnhMetafile = CloseEnhMetaFile(mHdc);
      if (!mEnhMetafile)
        RUNTIME_EXCEPTION("Failed to CloseEnhMetaFile(), errcode: " + std::to_string(GetLastError()));
    }
  }

  bool EmfHandleImpl::isClosed(void) const{
    return mIsClosed;
  }

  wchar_t const *EmfHandleImpl::getPath(void) const{
    return mPath.c_str();
  }

  void EmfHandleImpl::setCallback(uint8_t index, EmfHandleCallback callback){
    if (index < EMR_MIN || index > EMR_MAX)
      RUNTIME_EXCEPTION("Index out of range, should be in [EMR_MIN .. EMR_MAX]");

    mCallbacks[index] = callback;
  }

  EmfHandleCallback EmfHandleImpl::getCallback(uint8_t index) const{
    if (index < EMR_MIN || index > EMR_MAX)
      return nullptr;

    return mCallbacks[index];
  }

  void EmfHandleImpl::clearCallback(void){
    memset(mCallbacks, 0, sizeof(EmfHandleCallback) * EMR_MAX);
  }

  void EmfHandleImpl::playEmfFile(HENHMETAFILE source, void *data, DisplayBounding bounding){
    playEnhMetafile(source, data, mHdc, bounding);
  }

  void EmfHandleImpl::playEmfFile(wchar_t const *source, void *data, DisplayBounding bounding){
    playEnhMetafile(source, data, mHdc, bounding);
  }

  int EmfHandleImpl::getDeviceCaps(int index) const{
    switch (index){
      case LOGPIXELSX: return mDeviceCaps.mLogPixelsX;
      case LOGPIXELSY: return mDeviceCaps.mLogPixelsY;
      case HORZRES: return mDeviceCaps.mHorzRes;
      case VERTRES: return mDeviceCaps.mVertRes;
      case HORZSIZE: return mDeviceCaps.mHorzSize;
      case VERTSIZE: return mDeviceCaps.mVertSize;
      default: return GetDeviceCaps(mHdc, index);
    }
  }

  void EmfHandleImpl::saveAsJpeg(wchar_t const *path, uint8_t quality, bool isGrayscale){
    std::unique_ptr<CxImage> image = createImage(getBitmap());
    setJpegQuality(image.get(), quality);
    if (isGrayscale)
      setGrayscale(image.get());

    saveToFile(image.get(), CXIMAGE_FORMAT_JPG, path);
  }

  void EmfHandleImpl::saveAsTiff(wchar_t const *path, ImageBpp bpp, ImageCompression compression, ImageDithering dithering, bool isGrayscale){
    std::unique_ptr<CxImage> image = createImage(getBitmap());
    if (isGrayscale)
      setGrayscale(image.get());
    else
      setBpp(image.get(), bpp, dithering);

    setTiffCompression(image.get(), compression);    
    saveToFile(image.get(), CXIMAGE_FORMAT_TIF, path);
  }

  void EmfHandleImpl::saveAsPng(wchar_t const *path, ImageBpp bpp, ImageDithering dithering, bool isGrayscale){
    std::unique_ptr<CxImage> image = createImage(getBitmap());
    if (isGrayscale)
      setGrayscale(image.get());
    else
      setBpp(image.get(), bpp, dithering);

    saveToFile(image.get(), CXIMAGE_FORMAT_PNG, path);
  }

  void EmfHandleImpl::saveAsBmp(wchar_t const *path, ImageBpp bpp, ImageDithering dithering, bool isGrayscale){
    std::unique_ptr<CxImage> image = createImage(getBitmap());
    if (isGrayscale)
      setGrayscale(image.get());
    else
      setBpp(image.get(), bpp, dithering);

    saveToFile(image.get(), CXIMAGE_FORMAT_BMP, path);
  }

  void EmfHandleImpl::initialize(float width, float height, int16_t dpiX, int16_t dpiY, HDC hdc, uint16_t flags){
    mIsClosed = false;
    mHBitmap = nullptr;
    mWidth = width;
    mHeight = height;
    mDeviceCaps.mLogPixelsX = dpiX ? dpiX : GetDeviceCaps(hdc, LOGPIXELSX);
    mDeviceCaps.mLogPixelsY = dpiY ? dpiY : GetDeviceCaps(hdc, LOGPIXELSY);
    mDeviceCaps.mHorzRes = width * mDeviceCaps.mLogPixelsX;
    mDeviceCaps.mVertRes = height * mDeviceCaps.mLogPixelsY;
    mDeviceCaps.mHorzSize = width * 25.4f;
    mDeviceCaps.mVertSize = height * 25.4f;
    memset(mCallbacks, 0, EMR_MAX * sizeof(mCallbacks[0]));

    // Don't forget Windows Text Scale factor: DPI * HORZSIZE / (HORZRES * 25.4) * (HORZRES / DESKTOPHORZRES)
    float const scaleX = (float) mDeviceCaps.mLogPixelsX * GetDeviceCaps(hdc, HORZSIZE) / ((float) GetDeviceCaps(hdc, DESKTOPHORZRES) * 25.4f);
    float const scaleY = (float) mDeviceCaps.mLogPixelsY * GetDeviceCaps(hdc, VERTSIZE) / ((float) GetDeviceCaps(hdc, DESKTOPVERTRES) * 25.4f);

    mHdc = createWriteableEmf(hdc, to01mm(hdc, width * scaleX, height * scaleY), mPath.empty() ? nullptr : mPath.c_str(), flags);
  }

  void EmfHandleImpl::initialize(HDC hdc){
    if (!hdc)
      RUNTIME_EXCEPTION("The handle is invalid, errcode: " + std::to_string(ERROR_INVALID_HANDLE));

    float const dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
    float const dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
    float const width = (float) GetDeviceCaps(hdc, HORZRES) / dpiX;
    float const height = (float) GetDeviceCaps(hdc, VERTRES) / dpiY;
    initialize(width, height, dpiX, dpiY, hdc, 0);
  }

  HDC EmfHandleImpl::createWriteableEmf(HDC hdc, RECT size, wchar_t const *path, uint16_t flags){
    std::vector<wchar_t> const comments = flagsToComments(*this, flags);

    HDC emfHdc = CreateEnhMetaFileW(hdc, path, &size, flags ? comments.data() : nullptr);
    if (!emfHdc){
      RUNTIME_EXCEPTION("Failed to CreateEnhMetaFile(), errcode: " + std::to_string(GetLastError()));
      return nullptr;
    }

    return emfHdc;
  }

  HDC EmfHandleImpl::createPrinterDC(wchar_t const *printerName, DEVMODEW const *devmode){
    HDC printerDC = CreateDCW(L"WINSPOOL", printerName, nullptr, devmode);
    if (!printerDC)
      RUNTIME_EXCEPTION("Failed to CreateDC(), errcode: " + std::to_string(GetLastError()));

    return printerDC;
  }

  RECT EmfHandleImpl::to01mm(HDC hdc, pdfout::PaperSize paperSize, pdfout::PaperOrientation paperOrientation) const{
    pdfout::PaperSizeInfo const info = pdfout::getPaperSizeInfo(paperSize, paperOrientation);
    return to01mm(hdc, info.mWidth, info.mHeight);
  }

  RECT EmfHandleImpl::to01mm(HDC hdc, float width, float height) const{
    float const widthmm = GetDeviceCaps(hdc, HORZSIZE);
    float const heightmm = GetDeviceCaps(hdc, VERTSIZE);
    float const widthpixel = GetDeviceCaps(hdc, HORZRES);
    float const heightpixel = GetDeviceCaps(hdc, VERTRES);
    float const dpiX = widthpixel / widthmm * 25.4f;
    float const dpiY = heightpixel / heightmm * 25.4f;

    RECT rect = {0, 0, width * dpiX, height * dpiY};

    ::LPtoDP(hdc, (POINT *) &rect, 2); // map from logical to device

    rect.left = rect.left * widthmm * 100 / widthpixel;
    rect.right = rect.right * widthmm *100 / widthpixel;
    rect.top = rect.top * heightmm * 100 / heightpixel;
    rect.bottom = rect.bottom * heightmm * 100 / heightpixel;

    return rect;
  }

  ENHMETAHEADER EmfHandleImpl::getEnhMetaHeader(wchar_t const *source) const{
    HEnhMetafile metafile(GetEnhMetaFileW(source));
    if (!metafile)
      RUNTIME_EXCEPTION("Failed to GetEnhMetaFileW(), errcode: " + std::to_string(GetLastError()));

    ENHMETAHEADER header;
    memset(&header, 0, sizeof(ENHMETAHEADER));
    header.nSize = sizeof(ENHMETAHEADER);
    if (!GetEnhMetaFileHeader(metafile, sizeof(ENHMETAHEADER), &header))
      RUNTIME_EXCEPTION("Failed to GetEnhMetaFileHeader(), errcode: " + std::to_string(GetLastError()));

    return header;
  }

  RECT EmfHandleImpl::getEmfRclFrame(ENHMETAHEADER const &header) const{
    RECT rect = {header.rclFrame.left, header.rclFrame.top, header.rclFrame.right, header.rclFrame.bottom};
    return rect;
  }

  float EmfHandleImpl::getEmfDpiX(ENHMETAHEADER const &header) const{
    return 25.4f * header.szlDevice.cx / header.szlMillimeters.cx;
  }

  float EmfHandleImpl::getEmfDpiY(ENHMETAHEADER const &header) const{
    return 25.4f * header.szlDevice.cy / header.szlMillimeters.cy;
  }

  float EmfHandleImpl::getDeviceDpiX(HDC hdc) const{
    return (float) GetDeviceCaps(hdc, HORZRES) * 25.4f / GetDeviceCaps(hdc, HORZSIZE);
  }

  float EmfHandleImpl::getDeviceDpiY(HDC hdc) const{
    return (float) GetDeviceCaps(hdc, VERTRES) * 25.4f / GetDeviceCaps(hdc, VERTSIZE);
  }

  float EmfHandleImpl::getDeviceWidth(HDC hdc) const{
    float const dpiX = getDeviceDpiX(hdc);
    float const width = (float) GetDeviceCaps(hdc, HORZRES) / dpiX;
    return width;
  }

  float EmfHandleImpl::getDeviceHeight(HDC hdc) const{
    float const dpiY = getDeviceDpiY(hdc);
    float const height = (float) GetDeviceCaps(hdc, VERTRES) / dpiY;
    return height;
  }

  float EmfHandleImpl::getDeviceScaleX(HDC hdc) const{
    float const logicalDpiX = GetDeviceCaps(hdc, LOGPIXELSX);
    float const physicalDpiX = (float) GetDeviceCaps(hdc, HORZRES) * 25.4f / GetDeviceCaps(hdc, HORZSIZE);
    return logicalDpiX / physicalDpiX;
  }

  float EmfHandleImpl::getDeviceScaleY(HDC hdc) const{
    float const logicalDpiY = GetDeviceCaps(hdc, LOGPIXELSY);
    float const physicalDpiY = (float) GetDeviceCaps(hdc, VERTRES) * 25.4f / GetDeviceCaps(hdc, VERTSIZE);
    return logicalDpiY / physicalDpiY;
  }

  void EmfHandleImpl::playEnhMetafile(HENHMETAFILE source, void *data, HDC deviceDC, DisplayBounding bounding) const{
    if (!source)
      RUNTIME_EXCEPTION("Invalid HENHMETAFILE handle, errcode: " + std::to_string(ERROR_INVALID_HANDLE));

    Hdc screenDC(GetDC(nullptr));
    HDC hdc = deviceDC ? deviceDC : screenDC;

    RECT rect = {0};
    if (bounding == DisplayBoundingDevice){
      rect = {0, 0, GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES)};
    }
    else{
      ENHMETAHEADER hdr = {0};
      GetEnhMetaFileHeader(source, sizeof(hdr), &hdr);
      SetRect(&rect, hdr.rclFrame.left, hdr.rclFrame.top, hdr.rclFrame.right, hdr.rclFrame.bottom);

      POINT *pt = (POINT *) &rect;
      for (int i = 0; i < 2; ++i){
        int const tx = GetDeviceCaps(hdc, HORZSIZE) * 100;
        pt[i].x = (pt[i].x * GetDeviceCaps(hdc, HORZRES) + tx / 2) / tx;

        int const ty = GetDeviceCaps(hdc, VERTSIZE) * 100;
        pt[i].y = (pt[i].y * GetDeviceCaps(hdc, VERTRES) + ty / 2) / ty;
      }
    }

    EnumData enumData = {this, data};

    if (!EnumEnhMetaFile(mHdc, source, playEnhMetafileProc, &enumData, &rect))
      RUNTIME_EXCEPTION("Failed to EnumEnhMetafile(), errcode: " + std::to_string(GetLastError()));
  }

  void EmfHandleImpl::playEnhMetafile(wchar_t const *source, void *data, HDC deviceDC, DisplayBounding bounding) const{
    HEnhMetafile metafile(GetEnhMetaFileW(source));
    if (!metafile)
      RUNTIME_EXCEPTION("Failed to GetEnhMetaFileW(), errcode: " + std::to_string(GetLastError()));

    playEnhMetafile(metafile, data, deviceDC, bounding);
  }

  HBITMAP EmfHandleImpl::createBitmap(HDC hdc, uint32_t width, uint32_t height) const{
    HBitmap bitmap(CreateCompatibleBitmap(hdc, width, height));
    if (!bitmap){
      // fill BITMAPINFO
      BITMAPINFO bmi;
      memset(&bmi, 0, sizeof(BITMAPINFO));
      bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      bmi.bmiHeader.biWidth = width;
      bmi.bmiHeader.biHeight = height;
      bmi.bmiHeader.biPlanes = 1;

      // 24 bpp
      bmi.bmiHeader.biBitCount = 24;
      bmi.bmiHeader.biCompression = BI_RGB;
      bmi.bmiHeader.biSizeImage = ((((width * bmi.bmiHeader.biBitCount) + 31) & ~31) >> 3) * height;

      // try to create DIB
      LPVOID pBitmapBits = NULL;
      LPVOID pBytes = NULL;
      bitmap.reset(CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pBitmapBits, NULL, (DWORD) 0));
      if (!bitmap)
        RUNTIME_EXCEPTION("Failed to CreateDIBSection(), errcode: " + std::to_string(GetLastError()));
    }

    return bitmap.release();
  }

  HBITMAP EmfHandleImpl::getBitmap(void){
    if (isClosed() && mHBitmap)
      return mHBitmap;

    close();

    Hdc screenDC(GetDC(nullptr));

    Hdc memDC(CreateCompatibleDC(screenDC));
    if (!memDC)
      RUNTIME_EXCEPTION("Failed to CreateCompatibleDC(), errcode: " + std::to_string(GetLastError()));

    int const width = getDeviceCaps(HORZRES);
    int const height = getDeviceCaps(VERTRES);

    mHBitmap = createBitmap(screenDC, width, height);
    if (!mHBitmap)
      RUNTIME_EXCEPTION("Failed to CreateCompatibleBitmap(), errcode: " + std::to_string(GetLastError()));

    HBITMAP oldBitmap = (HBITMAP) SelectObject(memDC, mHBitmap);

    HBrush brush(CreateSolidBrush(RGB(255, 255, 255)));
    RECT bkRect = {0, 0, width, height};
    FillRect(memDC, &bkRect, brush);

    RECT rect = {0, 0, getDeviceCaps(HORZRES), getDeviceCaps(VERTRES)};
    if (!PlayEnhMetaFile(memDC, mEnhMetafile, &rect))
      RUNTIME_EXCEPTION("Failed to PlayEnhMetaFile(), errcode: " + std::to_string(GetLastError()));

    SelectObject(memDC, oldBitmap);
    return mHBitmap;
  }

  int CALLBACK EmfHandleImpl::playEnhMetafileProc(HDC hdc, HANDLETABLE *table, ENHMETARECORD const *emfr, int obj, LPARAM data){
    EnumData *enumData = (EnumData *) data;

    if (auto callback = enumData->mHandle->getCallback(emfr->iType)){
      callback(hdc, table, emfr, obj, enumData->mData);
      return 1;
    }

    switch(emfr->iType){
      case EMR_EXTTEXTOUTW:{
        std::vector<uint8_t> record(emfr->nSize);
        memcpy(record.data(), emfr, emfr->nSize);
        EMREXTTEXTOUTW *ptr = (EMREXTTEXTOUTW *) record.data();
        ptr->emrtext.fOptions &= ~ETO_IGNORELANGUAGE;
        if (!PlayEnhMetaFileRecord(hdc, table, (ENHMETARECORD *) record.data(), obj))
          RUNTIME_EXCEPTION("Failed to PlayEnhMetaFileRecord(), errcode: " + std::to_string(GetLastError()));

        return 1;
      }

      case EMR_EXTTEXTOUTA:{
        std::vector<uint8_t> record(emfr->nSize);
        memcpy(record.data(), emfr, emfr->nSize);
        EMREXTTEXTOUTA *ptr = (EMREXTTEXTOUTA *) record.data();
        ptr->emrtext.fOptions &= ~ETO_IGNORELANGUAGE;
        if (!PlayEnhMetaFileRecord(hdc, table, (ENHMETARECORD *) record.data(), obj))
          RUNTIME_EXCEPTION("Failed to PlayEnhMetaFileRecord(), errcode: " + std::to_string(GetLastError()));

        return 1;
      }

      case EMR_RESERVED_108:{
        std::vector<uint8_t> record(emfr->nSize);
        memcpy(record.data(), emfr, emfr->nSize);
        impl::EMRSMALLTEXTOUT *ptr = (impl::EMRSMALLTEXTOUT *) record.data();
        ptr->fOptions &= ~ETO_IGNORELANGUAGE;
        if (!PlayEnhMetaFileRecord(hdc, table, (ENHMETARECORD *) record.data(), obj))
          RUNTIME_EXCEPTION("Failed to PlayEnhMetaFileRecord(), errcode: " + std::to_string(GetLastError()));

        return 1;
      }
    }

    if (!PlayEnhMetaFileRecord(hdc, table, emfr, obj))
      RUNTIME_EXCEPTION("Failed to PlayEnhMetaFileRecord(), errcode: " + std::to_string(GetLastError()));

    return 1;
  }


  int GetDeviceCaps(EmfHandleImpl const &handle, int index){
    return handle.getDeviceCaps(index);
  }

}}

namespace{

  std::vector<wchar_t> flagsToComments(emf2pdf::impl::EmfHandleImpl const &handle, uint16_t flags){
    std::wstring cmd = L"";
    if (flags & flagForceDpiX)
      cmd += std::wstring(L"dpix=") + std::to_wstring(GetDeviceCaps(handle, LOGPIXELSX)) + L";";

    if (flags & flagForceDpiY)
      cmd += std::wstring(L"dpiy=") + std::to_wstring(GetDeviceCaps(handle, LOGPIXELSY)) + L";";

    std::vector<wchar_t> comments(cmd.length() + 2, 0);
    memcpy(comments.data(), cmd.data(), cmd.length() * sizeof(wchar_t));

    return comments;
  }

  std::unique_ptr<CxImage> createImage(HBITMAP bitmap){
    std::unique_ptr<CxImage> image = std::make_unique<CxImage>();
    if (!image->CreateFromHBITMAP(bitmap))
      RUNTIME_EXCEPTION(std::string("Failed to CxImage::CreateFromHBITMAP(), error: ") + image->GetLastError());

    return image;
  }

  void setJpegQuality(CxImage *image, uint8_t quality){
    image->SetJpegQuality(std::min((uint8_t) 100, quality));
  }

  void setBpp(CxImage *image, ImageBpp bpp, ImageDithering dithering){
    if (bpp != ImageBpp24){
      if (bpp == ImageBpp1 && !image->Dither(dithering))
        RUNTIME_EXCEPTION(std::string("Failed to CxImage::Dither(), error: ") + image->GetLastError());
      else if (!image->DecreaseBpp(bpp, true))
        RUNTIME_EXCEPTION(std::string("Failed to CxImage::DecreaseBpp(), error: ") + image->GetLastError());
    }
  }

  void setGrayscale(CxImage *image){
    if (!image->GrayScale())
      RUNTIME_EXCEPTION(std::string("Failed to CxImage::GrayScale(), error: ") + image->GetLastError());
  }

  void setTiffCompression(CxImage *image, ImageCompression compression){
    if (!image->SetCodecOption(compression, CXIMAGE_FORMAT_TIF))
      RUNTIME_EXCEPTION(std::string("Failed to CxImage::SetCodecOption(), error: ") + image->GetLastError());
  }

  void saveToFile(CxImage *image, ENUM_CXIMAGE_FORMATS format, wchar_t const *path){
    File file(_wfopen(path, L"wb"));
    if (!file)
      RUNTIME_EXCEPTION("Failed to _wfopen(), errno: " + std::to_string(errno));

    if (!image->Encode(file, format))
      RUNTIME_EXCEPTION(std::string("Failed to CxImage::Encode(), error: ") + image->GetLastError());
  }

}
