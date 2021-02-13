#pragma once

#include <Windows.h>

namespace emf2pdf{

  template <typename T, typename Deleter>
  class GenericHandle{
  public:
    GenericHandle(void) : mHandle(NULL), mDeleter() {}
    explicit GenericHandle(T handle) : mHandle(handle), mDeleter() {}

    ~GenericHandle(void){
      if (mHandle)
        mDeleter(mHandle);
    }

    operator T(void) const {return mHandle;}

    operator bool(void) const {return mHandle != NULL;}

    T *operator&(void) {return &mHandle;}
    T *operator->(void) const {return mHandle;}

    void reset(T handle){ /**< reset object to another handle value */
      if (mHandle)
        mDeleter(mHandle);

      mHandle = handle;
    }

    /**
      * Releases the object
      * @return Handle to the handled object
      */
    T release(void){
      T tmp = mHandle;
      mHandle = NULL;
      return tmp;
    }

  private:
    GenericHandle(GenericHandle const &);
    GenericHandle &operator=(GenericHandle const &);

  private:
    T mHandle;
    Deleter mDeleter;
  };

  struct FileDeleter{
    void operator()(FILE *file) {fclose(file);}
  };

  typedef GenericHandle<FILE *, FileDeleter> File; /**< For fopen */

  struct HdcDeleter{
    void operator()(HDC dc) {DeleteDC(dc);}
  };

  typedef GenericHandle<HDC, HdcDeleter> Hdc; /**< For CreateDC and CreateCompatibleDC calls */

  struct HBitmapDeleter{
    void operator()(HBITMAP bitmap) {DeleteObject(bitmap);}
  };

  typedef GenericHandle<HBITMAP, HBitmapDeleter> HBitmap; /**< For CreateCompatibleBitmap call */

  struct HEnhMetafileDeleter{
    void operator()(HENHMETAFILE metafile) {DeleteEnhMetaFile(metafile);}
  };

  typedef GenericHandle<HENHMETAFILE, HEnhMetafileDeleter> HEnhMetafile; /**< For GetEnhMetafile call */

  struct HPenDeleter{
    void operator()(HPEN pen) {DeleteObject(pen);}
  };

  typedef GenericHandle<HPEN, HPenDeleter> HPen; /**< For CreatePen call */

  struct HBrushDeleter{
    void operator()(HBRUSH brush) {DeleteObject(brush);}
  };

  typedef GenericHandle<HBRUSH, HBrushDeleter> HBrush; /**< For CreateSolidBrush call */

  struct HandleDeleter{
    void operator()(HANDLE handle) {CloseHandle(handle);}
  };

  typedef GenericHandle<HANDLE, HandleDeleter> Handle;

  struct RgnDeleter{
    void operator()(HRGN rgn) {DeleteObject(rgn);}
  };

  typedef GenericHandle<HRGN, RgnDeleter> Rgn;

  struct HKeyDeleter{
    void operator()(HKEY hkey) {RegCloseKey(hkey);}
  };

  typedef GenericHandle<HKEY, HKeyDeleter> HKey;

}
