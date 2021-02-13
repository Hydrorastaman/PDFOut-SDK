#include  <Emf2PdfEngineImpl.h>

#include <Page.h>
#include <Image.h>
#include <Exception.h>
#include <ContentStream.h>
#include <TransformationMatrix.h>

#include <EmfTypes.h>

#include <ximage.h>

#undef min
#undef max
#include <algorithm>

using namespace pdfout;

namespace emf2pdf{ namespace impl{

  HBITMAP Emf2PdfEngineImpl::copyBitmap(RECTL const &rclBounds){
    Hdc screenDC(GetDC(nullptr));
    Hdc memDC(CreateCompatibleDC(screenDC));

    RECTL bounds = {rclBounds.left, rclBounds.top, rclBounds.right, rclBounds.bottom};
    //::DPtoLP(mHdc, (POINT *) &bounds, 2);

    bounds = {std::min(bounds.left, bounds.right), std::min(bounds.top, bounds.bottom),
              std::max(bounds.left, bounds.right), std::max(bounds.top, bounds.bottom)};

    LONG const cx = bounds.right - bounds.left;
    LONG const cy = bounds.bottom - bounds.top;

    HBitmap bitmap(createCompatibleBitmap(screenDC, cx, cy));

    HBITMAP oldBitmap = (HBITMAP) SelectObject(memDC, bitmap);
    SetStretchBltMode(memDC, HALFTONE);

    if (!BitBlt(memDC, 0, 0, cx, cy, mHdc, bounds.left, bounds.top, SRCCOPY))
      RUNTIME_EXCEPTION(std::string("Failed to BitBlt(), errcode: ") + std::to_string(GetLastError()));

    SelectObject(memDC, oldBitmap);
    return bitmap.release();
  }

  HBITMAP Emf2PdfEngineImpl::createDIBitmap(BYTE const *source, DWORD offBmiSrc, DWORD offBitsSrc, DWORD iUsageSrc){
    BITMAPINFO const *bitmapInfo = (BITMAPINFO  const *)((BYTE const *) source + offBmiSrc);
    BITMAPINFOHEADER const *bitmapHeader = &bitmapInfo->bmiHeader;
    BYTE  const *data = (BYTE const *) source + offBitsSrc;
    HBitmap sourceBitmap(CreateDIBitmap(mHdc, bitmapHeader, CBM_INIT, data, bitmapInfo, iUsageSrc));

    Hdc screenDC(GetDC(nullptr));
    Hdc sourceDC(CreateCompatibleDC(screenDC));
    HBITMAP oldBitmap = (HBITMAP) SelectObject(sourceDC, sourceBitmap);

    Hdc memDC(CreateCompatibleDC(screenDC));
    SetStretchBltMode(memDC, HALFTONE);
    LONG const cx = std::abs(bitmapHeader->biWidth);
    LONG const cy = std::abs(bitmapHeader->biHeight);
    HBitmap bitmap(createCompatibleBitmap(screenDC, cx, cy));
    HBITMAP oldBitmapMem = (HBITMAP) SelectObject(memDC, bitmap);
    StretchBlt(memDC, 0, 0, cx, cy, sourceDC, 0, 0, cx, cy, SRCCOPY);

    SelectObject(memDC, oldBitmapMem);
    SelectObject(sourceDC, oldBitmap);
    return bitmap.release();
  }

  void Emf2PdfEngineImpl::drawImage(HBitmap const &bitmap, TransformationMatrix const &tm){
    std::unique_ptr<CxImage> image = std::make_unique<CxImage>();
    if (!image->CreateFromHBITMAP(bitmap))
      RUNTIME_EXCEPTION(std::string("Failed to load CxImage::CreateFromHBITMAP(): ") + image->GetLastError());

    switch (mColorSpace){
      case ColorSpaceFamilyDeviceRGB:
        break;

      case ColorSpaceFamilyDeviceGray:
        if (!image->GrayScale())
          RUNTIME_EXCEPTION(std::string("Failed to CxImage::GrayScale(): ") + image->GetLastError());
        break;

      case ColorSpaceFamilyDeviceCMYK:
        break;

      default:
        RUNTIME_EXCEPTION("Unsupported ColorSpaceFamily value");
    }

    ImageCompression compression = image->IsIndexed() ? ImageCompressionIndexed : ImageCompressionJpegDCT;
    Image *img = Image::createImage(image->GetDIB(), image->GetSize());
    img->setCompression(compression);
    img->setJpegQuality(mJpegQuality);

    mContentStream->save();
      drawClippingPath();
      mContentStream->showImage(img, tm);
    mContentStream->restore();
  }

  void Emf2PdfEngineImpl::onEmrBitBlt(ENHMETARECORD const *emfr){
    EMRBITBLT *source = (EMRBITBLT *) emfr;
    onBlockTransfer(source, TransformationMatrix((float *) &source->xformSrc));
  }

  void Emf2PdfEngineImpl::onEmrStretchDiBits(ENHMETARECORD const *emfr){
    EMRSTRETCHDIBITS *source = (EMRSTRETCHDIBITS *) emfr;
    onBlockTransfer(source);
  }

  void Emf2PdfEngineImpl::onEmrMaskBlt(ENHMETARECORD const *emfr){
    EMRMASKBLT *source = (EMRMASKBLT *) emfr;
    onBlockTransfer(source, TransformationMatrix((float *) &source->xformSrc));
  }

  void Emf2PdfEngineImpl::onEmrTransparentBlt(ENHMETARECORD const *emfr){
    EMRTRANSPARENTBLT *source = (EMRTRANSPARENTBLT *) emfr;
    onBlockTransfer(source, TransformationMatrix((float *) &source->xformSrc));
  }

  void Emf2PdfEngineImpl::onEmrPlgBlt(ENHMETARECORD const *emfr){
    EMRPLGBLT *source = (EMRPLGBLT *) emfr;

    float const X1 = source->aptlDest[0].x;
    float const Y1 = source->aptlDest[0].y;

    float const X2 = source->aptlDest[1].x;
    float const Y2 = source->aptlDest[1].y;

    float const X3 = source->aptlDest[2].x;
    float const Y3 = source->aptlDest[2].y;

    RECTL rect = {std::min(std::min(X1, X2), X3), std::min(std::min(Y1, Y2), Y3), std::max(std::max(X1, X2), X3), std::max(std::max(Y1, Y2), Y3)};

    HBitmap bitmap(copyBitmap(source->rclBounds));
    TransformationMatrix tm = mCurrentDeviceState->mWorldTransform;
    float const x0 = rect.left;
    float const y0 = rect.bottom;
    float const cxDest = rect.right - rect.left;
    float const cyDest = rect.bottom - rect.top;
    tm *= {cxDest, 0.f, 0.f, -cyDest, x0, y0};
    drawImage(bitmap, tm);
  }

  void Emf2PdfEngineImpl::onEmrStretchBlt(ENHMETARECORD const *emfr){
    EMRSTRETCHBLT *source = (EMRSTRETCHBLT *) emfr;
    onBlockTransfer(source, TransformationMatrix((float *) &source->xformSrc));
  }

  void Emf2PdfEngineImpl::onEmrAlphaBlend(ENHMETARECORD const *emfr){
    EMRALPHABLEND *source = (EMRALPHABLEND *) emfr;
    onBlockTransfer(source, TransformationMatrix((float *) &source->xformSrc));
  }

  HBITMAP Emf2PdfEngineImpl::createCompatibleBitmap(HDC hdc, long cx, long cy) const{
    HBitmap bitmap(CreateCompatibleBitmap(hdc, cx, cy));
    if (!bitmap){
      // fill BITMAPINFO
      BITMAPINFO bmi;
      memset(&bmi, 0, sizeof(BITMAPINFO));
      bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      bmi.bmiHeader.biWidth = cx;
      bmi.bmiHeader.biHeight = cy;
      bmi.bmiHeader.biPlanes = 1;

      // 24 bpp
      bmi.bmiHeader.biBitCount = 24;
      bmi.bmiHeader.biCompression = BI_RGB;
      bmi.bmiHeader.biSizeImage = ((((cx * bmi.bmiHeader.biBitCount) + 31) & ~31) >> 3) * cy;

      // try to create DIB
      LPVOID pBitmapBits = NULL;
      LPVOID pBytes = NULL;
      bitmap.reset(CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pBitmapBits, NULL, (DWORD) 0));
      if (!bitmap)
        RUNTIME_EXCEPTION("Failed to CreateDIBSection(), errcode: " + std::to_string(GetLastError()));
    }

    return bitmap.release();
  }

}}
