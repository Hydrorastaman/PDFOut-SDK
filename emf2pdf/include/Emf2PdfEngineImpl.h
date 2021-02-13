#pragma once

#include <stack>
#include <memory>
#include <string>

#include <Font.h>
#include <Page.h>
#include <ColorSpace.h>
#include <ContentStream.h>

#include <DeviceState.h>
#include <Emf2PdfEngine.h>
#include <GenericHandle.h>
#include <FontStorageImpl.h>

namespace emf2pdf{ namespace impl{

  class Emf2PdfEngineImpl{
  public:
    Emf2PdfEngineImpl(FontStorage *fontStorage, pdfout::ColorSpaceFamily colorSpace);
    ~Emf2PdfEngineImpl(void);

    void convert(pdfout::Page *page, wchar_t const *emfFile, float marginX, float marginY);
    void convert(pdfout::Page *page, HENHMETAFILE metafile, float marginX, float marginY);

    void setJpegQuality(uint8_t jpegQuality); /**< Set JPEG quality in range [0 .. 100] */

    void enableIntercharacterSpacingFix(bool flag);
    void enableUnicodeBidiAlgorithm(bool flag);

  private:
    static  int CALLBACK enhMetaFileProc(HDC hdc, HANDLETABLE *table, ENHMETARECORD const *emfr, int obj, LPARAM data);

    bool isActivePath(void) const;

    // General operations
    void onEmrSaveDc(ENHMETARECORD const *emfr);
    void onEmrRestoreDc(ENHMETARECORD const *emfr);
    void onEmrSetMapMode(ENHMETARECORD const *emfr);
    void onEmrSetPolyfillMode(ENHMETARECORD const *emfr);
    void onEmrMoveToEx(ENHMETARECORD const *emfr);
    void onEmrSetWorldTransform(ENHMETARECORD const *emfr);
    void onEmrModifyWorldTransform(ENHMETARECORD const *emfr);
    void onUpdateWorldTransformation(void);
    void onUpdateCurrentPosition(void);

    // Text painting operations
    void onEmrReserved108(ENHMETARECORD const *emfr);
    void onEmrExtTextOut(ENHMETARECORD const *emfr);
    void onEmrSetTextColor(ENHMETARECORD const *emfr);
    void onEmrSetTextAlign(ENHMETARECORD const *emfr);
    void onEmrSetBkColor(ENHMETARECORD const *emfr);
    void onEmrSetBkMode(ENHMETARECORD const *emfr);

    // Image painting operations
    HBITMAP copyBitmap(RECTL const &rclBounds);
    HBITMAP createDIBitmap(BYTE const *source, DWORD offBmiSrc, DWORD offBitsSrc, DWORD iUsageSrc);
    void drawImage(HBitmap const &bitmap, pdfout::TransformationMatrix const &tm);

    void printGDIObjects(std::string const &tag) const{
      Handle proc(OpenProcess(PROCESS_QUERY_INFORMATION, false, GetCurrentProcessId()));
      DWORD const count = GetGuiResources(proc, GR_GDIOBJECTS);
      fprintf(stderr, "%s : %i\n", tag.c_str(), count);
    }

    template <typename T>
    void onBlockTransfer(T *source, pdfout::TransformationMatrix const &xformSrc = {1.f, 0, 0, 1.f, 0, 0}){
      HBitmap bitmap(nullptr);
      pdfout::TransformationMatrix tm = mCurrentDeviceState->mWorldTransform;
      if (source->dwRop == PATCOPY){
        struct pdfout::Rectangle rcl = {source->xDest, source->yDest, source->xDest + source->cxDest, source->yDest + source->cyDest};
        LPtoDP(mHdc, (pdfout::Point *) &rcl, 2);
        onPatcopy(rcl);
        return;
      }

      if (source->dwRop == SRCCOPY){
        bitmap.reset(createDIBitmap((BYTE const *) source, source->offBmiSrc, source->offBitsSrc, source->iUsageSrc));
      }
      else
        bitmap.reset(copyBitmap({source->xDest, source->yDest, source->xDest + source->cxDest, source->yDest + source->cyDest}));

      Vector const pos = {(float) source->xDest, (float) source->yDest + source->cyDest};
      Vector const size = {(float) source->cxDest, (float) source->cyDest};
      tm *= {(float) size.mX, 0.f, 0.f, (float) -size.mY, pos.mX, pos.mY};
      drawImage(bitmap, tm);
    }

    void onEmrBitBlt(ENHMETARECORD const *emfr);
    void onEmrStretchDiBits(ENHMETARECORD const *emfr);
    void onEmrMaskBlt(ENHMETARECORD const *emfr);
    void onEmrTransparentBlt(ENHMETARECORD const *emfr);
    void onEmrPlgBlt(ENHMETARECORD const *emfr);
    void onEmrStretchBlt(ENHMETARECORD const *emfr);
    void onEmrAlphaBlend(ENHMETARECORD const *emfr);

    // Path painting operations
    void drawPath(std::vector<POINT> const &points, std::vector<BYTE> const &types, DWORD type, bool isStandAlone);
    void drawClippingPath(void);
    void selectClippingPath(void);
    void onEmrBeginPath(ENHMETARECORD const *emfr);
    void onEmrEndPath(ENHMETARECORD const *emfr);
    void onEmrCloseFigure(ENHMETARECORD const *emfr);
    void onEmrFillPath(ENHMETARECORD const *emfr);
    void onEmrStrokeAndFillPath(ENHMETARECORD const *emfr);
    void onEmrStrokePath(ENHMETARECORD const *emfr);
    void onEmrSelectClipPath(ENHMETARECORD const *emfr);
    void onEmrIntersectClipRect(ENHMETARECORD const *emfr);
    void onEmrExtSelectClipRgn(ENHMETARECORD const *emfr);

    // Region painting operations
    void onEmrPaintRgn(ENHMETARECORD const *emfr);

    // Font operations
    void onEmrReserved109(ENHMETARECORD const *emfr);
    void onEmrExtCreateFontIndirectW(HANDLETABLE *table, ENHMETARECORD const *emfr, int obj);

    // Extra operations
    void onEmrGdiComment(ENHMETARECORD const *emfr);
    void onEmrGdiCommentTextURL(ENHMETARECORD const *emfr);

    void clearDeviceStack(void);
    void save(void);
    void restore(void);
    pdfout::Color colorrefToRgb(COLORREF colorref) const;
    pdfout::Color colorrefToGray(COLORREF colorref) const;
    pdfout::Color colorrefToCmyk(COLORREF colorref) const;

    pdfout::TransformationMatrix xformToTM(XFORM const &xf) const;
    HBITMAP createCompatibleBitmap(HDC hdc, long cx, long cy) const;

    void setBrush(void) const;
    void setPen(void) const;
    void setWorldTransform(void) const;
    void setTextColor(void) const;
    void setTextBkColor(RECTL const &rect) const;
    void setTextClipping(RECTL const &rect) const;
    void setStrokingColor(COLORREF colorref) const;
    //void onPatcopy(RECTL const &rect);
    void onPatcopy(struct pdfout::Rectangle const &rect);
    pdfout::PathPaintingOp getPathPaintingOp(DWORD type) const;

    float toRad(float deg) const;
    void DPtoLP(HDC hdc, pdfout::Point *pts, int count);
    void LPtoDP(HDC hdc, pdfout::Point *pts, int count);

  private:
    Emf2PdfEngineImpl(Emf2PdfEngineImpl const &) = delete;
    Emf2PdfEngineImpl &operator=(Emf2PdfEngineImpl const &) = delete;

  private:
    pdfout::ColorSpaceFamily mColorSpace;
    int8_t mJpegQuality;
    Hdc mHdc;
    HBitmap mHBitmap;
    pdfout::Page *mPage;
    pdfout::ContentStream *mContentStream;
    std::unique_ptr<FontStorage> mFontStorage;
    bool mIsExternalFontStorage;

    std::stack<std::unique_ptr<DeviceState>> mDeviceStateStack;
    std::unique_ptr<DeviceState> mCurrentDeviceState;
    bool mIsActivePath;
    std::vector<POINT> mActivePathPoints;
    std::vector<uint8_t> mActivePathTypes;

    bool mEnableIntercharacterSpacingFix;
    bool mEnableUnicodeBidiAlgorithm;
  };

}}
