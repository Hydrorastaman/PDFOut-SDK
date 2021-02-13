#include <EmfHandle.h>
#include <Exception.h>
#include <EmfHandleImpl.h>

namespace emf2pdf{

  EmfHandle::EmfHandle(float width, float height, int dpiX, int dpiY, wchar_t const *path)
    : mEmfHandleImpl(new impl::EmfHandleImpl(width, height, dpiX, dpiY, path)){
  }

  EmfHandle::EmfHandle(pdfout::PaperSize paperSize, pdfout::PaperOrientation paperOrientation, int dpiX, int dpiY, wchar_t const *path)
    : mEmfHandleImpl(new impl::EmfHandleImpl(paperSize, paperOrientation, dpiX, dpiY, path)){
  }

  EmfHandle::EmfHandle(HDC hdc, wchar_t const *path)
    : mEmfHandleImpl(new impl::EmfHandleImpl(hdc, path)){
  }

  EmfHandle::EmfHandle(wchar_t const *printerName, DEVMODEW const *devmode, wchar_t const *path)
    : mEmfHandleImpl(new impl::EmfHandleImpl(printerName, devmode, path)){
  }

  EmfHandle::~EmfHandle(void){
    if (mEmfHandleImpl) {
      delete mEmfHandleImpl;
      mEmfHandleImpl = nullptr;
    }
  }

  EmfHandle::operator HDC(void) const{
    if (mEmfHandleImpl)
      return mEmfHandleImpl->operator HDC();

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  EmfHandle::operator HENHMETAFILE(void){
    if (mEmfHandleImpl)
      return mEmfHandleImpl->operator HENHMETAFILE();

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  EmfHandle::operator HBITMAP(void){
    if (mEmfHandleImpl)
      return mEmfHandleImpl->operator HBITMAP();

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void EmfHandle::close(void){
    if (mEmfHandleImpl)
      return mEmfHandleImpl->close();

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  bool EmfHandle::isClosed(void) const{
    if (mEmfHandleImpl)
      return mEmfHandleImpl->isClosed();

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  wchar_t const *EmfHandle::getPath(void) const{
    if (mEmfHandleImpl)
      return mEmfHandleImpl->getPath();

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void EmfHandle::setCallback(uint8_t index, EmfHandleCallback callback){
    if (mEmfHandleImpl)
      return mEmfHandleImpl->setCallback(index, callback);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  EmfHandleCallback EmfHandle::getCallback(uint8_t index) const{
    if (mEmfHandleImpl)
      return mEmfHandleImpl->getCallback(index);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void EmfHandle::clearCallback(void){
    if (mEmfHandleImpl)
      return mEmfHandleImpl->clearCallback();

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void EmfHandle::playEmfFile(HENHMETAFILE source, void *data, DisplayBounding bounding){
    if (mEmfHandleImpl)
      return mEmfHandleImpl->playEmfFile(source, data, bounding);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void EmfHandle::playEmfFile(wchar_t const *source, void *data, DisplayBounding bounding){
    if (mEmfHandleImpl)
      return mEmfHandleImpl->playEmfFile(source, data, bounding);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  int EmfHandle::getDeviceCaps(int index) const{
    if (mEmfHandleImpl)
      return mEmfHandleImpl->getDeviceCaps(index);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void EmfHandle::saveAsJpeg(wchar_t const *path, uint8_t quality, bool isGrayscale){
    if (mEmfHandleImpl)
      return mEmfHandleImpl->saveAsJpeg(path, quality, isGrayscale);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void EmfHandle::saveAsTiff(wchar_t const *path, ImageBpp bpp, ImageCompression compression, ImageDithering dithering, bool isGrayscale){
    if (mEmfHandleImpl)
      return mEmfHandleImpl->saveAsTiff(path, bpp, compression, dithering, isGrayscale);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void EmfHandle::saveAsPng(wchar_t const *path, ImageBpp bpp, ImageDithering dithering, bool isGrayscale){
    if (mEmfHandleImpl)
      return mEmfHandleImpl->saveAsPng(path, bpp, dithering, isGrayscale);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  void EmfHandle::saveAsBmp(wchar_t const *path, ImageBpp bpp, ImageDithering dithering, bool isGrayscale){
    if (mEmfHandleImpl)
      return mEmfHandleImpl->saveAsBmp(path, bpp, dithering, isGrayscale);

    RUNTIME_EXCEPTION("Invalid impl pointer");
  }

  int GetDeviceCaps(EmfHandle const &handle, int index){
    return handle.getDeviceCaps(index);
  }

}
