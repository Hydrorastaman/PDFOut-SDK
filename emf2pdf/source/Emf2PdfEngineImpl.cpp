#include <Emf2PdfEngineImpl.h>

#include <regex>
#include <algorithm>
#include <EmfTypes.h>
#include <Exception.h>
#include <GenericHandle.h>

#undef min
#undef max

using namespace pdfout;

namespace emf2pdf{ namespace impl{

  Emf2PdfEngineImpl::Emf2PdfEngineImpl(FontStorage *fontStorage, ColorSpaceFamily colorSpace)
    : mColorSpace(colorSpace),
    mJpegQuality(100),
    mHdc(nullptr),
    mHBitmap(nullptr),
    mPage(nullptr),
    mContentStream(nullptr),
    mFontStorage(fontStorage != nullptr ? std::unique_ptr<FontStorage>(fontStorage) : std::make_unique<FontStorage>()),
    mIsExternalFontStorage(fontStorage != nullptr ? true : false),
    mDeviceStateStack(),
    mCurrentDeviceState(nullptr),
    mIsActivePath(false),
    mActivePathPoints(),
    mActivePathTypes(),
    mEnableIntercharacterSpacingFix(false),
    mEnableUnicodeBidiAlgorithm(false){
  }

  Emf2PdfEngineImpl::~Emf2PdfEngineImpl(void){
    if (mIsExternalFontStorage)
      mFontStorage.release();
  }

  void Emf2PdfEngineImpl::convert(Page *page, wchar_t const *emfFile, float marginX, float marginY){
    HEnhMetafile emf(GetEnhMetaFileW(emfFile));
    if (!emf)
      RUNTIME_EXCEPTION("Failed to GetEnhMetaFile(), errcode: " + std::to_string(GetLastError()));

    return convert(page, emf, marginX, marginY);
  }

  void Emf2PdfEngineImpl::convert(pdfout::Page *page, HENHMETAFILE metafile, float marginX, float marginY){
    mPage = page;
    mContentStream = page->getContents();

    ENHMETAHEADER hdr = {0};
    GetEnhMetaFileHeader(metafile, sizeof(hdr), &hdr);

    uint16_t emfForceDpiX = 0;
    uint16_t emfForceDpiY = 0;

    if (hdr.nDescription){
      std::size_t const len = GetEnhMetaFileDescriptionW(metafile, 0, nullptr);
      std::vector<wchar_t> description(len, 0);
      GetEnhMetaFileDescriptionW(metafile, len, description.data());
      std::wstring query(std::begin(description), std::end(description));

      std::wregex const pattern(L"([\\w+]+)=([[:digit:]]+);");
      auto words_begin = std::wsregex_iterator(std::begin(query), std::end(query), pattern);
      auto words_end = std::wsregex_iterator();

      for (std::wsregex_iterator iter = words_begin; iter != words_end; ++iter){
        std::wstring const key = (*iter)[1].str();
        std::wstring const value = (*iter)[2].str();

        if (key == L"dpix")
          emfForceDpiX = _wtoi(value.c_str());
        else if (key == L"dpiy")
          emfForceDpiY = _wtoi(value.c_str());
      }
    }

    uint16_t const emfDpiX = (uint16_t) std::lround(25.4f * hdr.szlDevice.cx / hdr.szlMillimeters.cx);
    uint16_t const emfDpiY = (uint16_t) std::lround(25.4f * hdr.szlDevice.cy / hdr.szlMillimeters.cy);

    float const scaleX = emfForceDpiX ? (float) emfDpiX / emfForceDpiX : 1.f;
    float const scaleY = emfForceDpiY ? (float) emfDpiY / emfForceDpiY : 1.f;

    float const frameWidth = (hdr.rclFrame.right - hdr.rclFrame.left) / 100.f;
    float const frameHeight = (hdr.rclFrame.bottom - hdr.rclFrame.top) / 100.f;
    float const pdfWidth = (float) std::lround(frameWidth / 25.4f * 72.f) * scaleX;
    float const pdfHeight = (float) std::lround(frameHeight / 25.4f * 72.f) * scaleY;

    struct Rectangle mediaBox = {0, 0, pdfWidth, pdfHeight};
    page->setMediaBox(mediaBox);

    RECT rect = {0, 0, std::round(emfDpiX * frameWidth / 25.4f), std::round(emfDpiY * frameHeight / 25.4f)};
    float const emfWidth = (float) (rect.right - rect.left);
    float const emfHeight = (float) (rect.bottom - rect.top);

    mContentStream->save();
    mContentStream->setColorSpace(ColorSpace::createDeviceRGB());

    float const dx = std::max(0.f, marginX) * 72.f * scaleX;
    float const dy = std::max(0.f, marginY) * 72.f * scaleY;
    TransformationMatrix ctm((pdfWidth - 2.f * dx) / emfWidth, 0, 0, -(pdfHeight - 2.f * dy) / emfHeight, dx, pdfHeight - dy);
    mContentStream->setCTM(ctm);

    Hdc screenDC(GetDC(nullptr));
    mHdc.reset(CreateCompatibleDC(screenDC));
    mHBitmap.reset(createCompatibleBitmap(screenDC, emfWidth, emfHeight));
    HBITMAP oldBitmap = (HBITMAP) SelectObject(mHdc, mHBitmap);

    HBrush brush(CreateSolidBrush(RGB(255, 255, 255)));
    RECT bkRect = {0, 0, emfWidth, emfHeight};
    FillRect(mHdc, &bkRect, brush);

    clearDeviceStack();

    EnumEnhMetaFile(mHdc, metafile, enhMetaFileProc, this, &rect);

    SelectObject(mHdc, oldBitmap);

    mContentStream->restore();
  }

  void Emf2PdfEngineImpl::setJpegQuality(uint8_t jpegQuality){
    mJpegQuality = std::min(jpegQuality, (uint8_t) 100);
  }

  void Emf2PdfEngineImpl::enableIntercharacterSpacingFix(bool flag){
    mEnableIntercharacterSpacingFix = flag;
  }

  void Emf2PdfEngineImpl::enableUnicodeBidiAlgorithm(bool flag){
    mEnableUnicodeBidiAlgorithm = flag;
  }

  bool Emf2PdfEngineImpl::isActivePath(void) const{
    return mIsActivePath;
  }

  void Emf2PdfEngineImpl::onEmrSaveDc(ENHMETARECORD const *emfr){
    save();
  }

  void Emf2PdfEngineImpl::onEmrRestoreDc(ENHMETARECORD const *emfr){
    restore();
    //onUpdateWorldTransformation();
  }

  void Emf2PdfEngineImpl::onEmrSetMapMode(ENHMETARECORD const *emfr){
    EMRSETMAPMODE *source = (EMRSETMAPMODE *) emfr;
    mCurrentDeviceState->mMapMode = source->iMode;
    onUpdateWorldTransformation();
  }

  void Emf2PdfEngineImpl::onEmrSetPolyfillMode(ENHMETARECORD const *emfr){
    EMRSETPOLYFILLMODE *source = (EMRSETPOLYFILLMODE *) emfr;
    mCurrentDeviceState->mPolyfillMode = source->iMode;
  }

  void Emf2PdfEngineImpl::onEmrMoveToEx(ENHMETARECORD const *emfr){
    if (mIsActivePath)
      return;

    EMRMOVETOEX *source = (EMRMOVETOEX *) emfr;
    mCurrentDeviceState->mCurrentPos = {(float) source->ptl.x, (float) source->ptl.y};
  }

  void Emf2PdfEngineImpl::onEmrSetWorldTransform(ENHMETARECORD const *emfr){
    EMRSETWORLDTRANSFORM *source = (EMRSETWORLDTRANSFORM *) emfr;
    memcpy(&mCurrentDeviceState->mWorldTransform, &source->xform, sizeof(XFORM));
  }

  void Emf2PdfEngineImpl::onEmrModifyWorldTransform(ENHMETARECORD const *emfr){
    EMRMODIFYWORLDTRANSFORM *source = (EMRMODIFYWORLDTRANSFORM *) emfr;
    switch (source->iMode){
      case MWT_IDENTITY:{
        mCurrentDeviceState->mWorldTransform = {1.f, 0, 0, 1.f, 0, 0};
        XFORM const xform = {1.f, 0, 0, 1.f, 0, 0};
        ModifyWorldTransform(mHdc, &xform, MWT_IDENTITY);
        break;
      }

      case MWT_LEFTMULTIPLY:{
        TransformationMatrix tm;
        memcpy(&tm, &source->xform, sizeof(XFORM));
        mCurrentDeviceState->mWorldTransform = mCurrentDeviceState->mWorldTransform * tm;
        break;
      }

      case MWT_RIGHTMULTIPLY:{
        TransformationMatrix tm;
        memcpy(&tm, &source->xform, sizeof(XFORM));
        mCurrentDeviceState->mWorldTransform = tm * mCurrentDeviceState->mWorldTransform;
        break;
      }
    }
  }

  void Emf2PdfEngineImpl::onUpdateWorldTransformation(void){
    XFORM xForm = {0};
    GetWorldTransform(mHdc, &xForm);
    memcpy(&mCurrentDeviceState->mWorldTransform, &xForm, sizeof(XFORM));
  }

  void Emf2PdfEngineImpl::onUpdateCurrentPosition(void){
    POINT currPos = {0};
    GetCurrentPositionEx(mHdc, &currPos);
    mCurrentDeviceState->mCurrentPos = {(float) currPos.x, (float) currPos.y};
  }

  void Emf2PdfEngineImpl::clearDeviceStack(void){
    std::stack<std::unique_ptr<DeviceState>> empty;
    std::swap(mDeviceStateStack, empty);

    mCurrentDeviceState = std::make_unique<DeviceState>();
  }

  void Emf2PdfEngineImpl::save(void){
    mDeviceStateStack.push(std::unique_ptr<DeviceState>(mCurrentDeviceState->clone()));
  }

  void Emf2PdfEngineImpl::restore(void){
    if (!mDeviceStateStack.empty()){
      mCurrentDeviceState.reset(mDeviceStateStack.top().release());
      mDeviceStateStack.pop();
    }
  }

  Color Emf2PdfEngineImpl::colorrefToRgb(COLORREF colorref) const{
    return Color::createRGB(GetRValue(colorref) / 255.f, GetGValue(colorref) / 255.f, GetBValue(colorref) / 255.f);
  }

  Color Emf2PdfEngineImpl::colorrefToGray(COLORREF colorref) const{
    // The luminosity method
    float const r = (float) GetRValue(colorref) / 255.f;
    float const g = (float) GetGValue(colorref) / 255.f;
    float const b = (float) GetBValue(colorref) / 255.f;
    return Color::createGray(0.21f * r + 0.72f * g + 0.07f * b);
  }

  Color Emf2PdfEngineImpl::colorrefToCmyk(COLORREF colorref) const{
    uint8_t r = GetRValue(colorref);
    uint8_t g = GetGValue(colorref);
    uint8_t b = GetBValue(colorref);

    if (r == 0 && g == 0 && b == 0){
      return Color::createCMYK(0.f, 0.f, 0.f, 1.f);
    }
    else{
      float const dr = (float) r / 255.f;
      float const dg = (float) g / 255.f;
      float const db = (float) b / 255.f;

      float const k = 1.f - std::max(std::max(dr, dg), db);
      float const c = (1.f - dr - k) / (1.f - k);
      float const m = (1.f - dg - k) / (1.f - k);
      float const y = (1.f - db - k) / (1.f - k);

      float const cyan = (1.f - dr - k) / (1.f - k);
      float const magenta = (1.f - dg - k) / (1.f - k);
      float const yellow = (1.f - db - k) / (1.f - k);
      float const black = 1.f - std::max(std::max(dr, dg), db);

      return Color::createCMYK(cyan, magenta, yellow, black);
    }
  }

  void Emf2PdfEngineImpl::setBrush(void) const{
    HBRUSH brush = (HBRUSH) GetCurrentObject(mHdc, OBJ_BRUSH);
    LOGBRUSH logBrush = {0};
    GetObject(brush, sizeof(LOGBRUSH), &logBrush);

    if (logBrush.lbStyle == BS_NULL){
      mCurrentDeviceState->mIsNullBrush = true;
      return;
    }

    mCurrentDeviceState->mIsNullBrush = false;

    switch (mColorSpace){
      case ColorSpaceFamilyDeviceRGB:
        mContentStream->setColorRGB(colorrefToRgb(logBrush.lbColor));
        break;

      case ColorSpaceFamilyDeviceGray:
        mContentStream->setColorGray(colorrefToGray(logBrush.lbColor));
        break;

      case ColorSpaceFamilyDeviceCMYK:
        mContentStream->setColorCMYK(colorrefToCmyk(logBrush.lbColor));
        break;

      default:
        RUNTIME_EXCEPTION("Unsupported ColorSpaceFamily value");
    }
  }

  void Emf2PdfEngineImpl::setPen(void) const{
    HPEN pen = (HPEN) GetCurrentObject(mHdc, OBJ_PEN);
    float lineWidth = 0.f;
    DWORD penStyle = 0;
    COLORREF color = RGB(0.f, 0.f, 0.f);

    LOGPEN logPen = {0};
    if (!GetObject(pen, sizeof(LOGPEN), &logPen)){
      EXTLOGPEN extLogPen = {0};
      GetObject(pen, sizeof(EXTLOGPEN), &extLogPen);

      penStyle = extLogPen.elpPenStyle;
      lineWidth = extLogPen.elpWidth;
      color = extLogPen.elpColor;
    }
    else{
      lineWidth = logPen.lopnWidth.x;
      color = logPen.lopnColor;
      penStyle = logPen.lopnStyle;
    }

    bool const isGeometric = penStyle & PS_GEOMETRIC;

    std::vector<float> dash;
    bool isNullPen = false;

    switch (penStyle & PS_STYLE_MASK){
      case PS_SOLID:
        break;

      case PS_DASH:
        if (isGeometric && lineWidth > 1.f)
          dash = {lineWidth * 2.f, lineWidth * 2.f};
        else
          dash = {18.f, 6.f};
        break;

      case PS_DOT:
        if (isGeometric && lineWidth > 1.f)
          dash = {1.f, lineWidth * 2.f};
        else
          dash = {3.f, 3.f};
        break;

      case PS_DASHDOT:
        if (isGeometric && lineWidth > 1.f)
          dash = {lineWidth * 2.f, lineWidth * 2.f, 1.f, lineWidth * 2.f};
        else
          dash = {9.f, 6.f, 3.f, 6.f};
        break;

      case PS_DASHDOTDOT:
        if (isGeometric && lineWidth > 1.f)
          dash = {lineWidth * 2.f, lineWidth * 2.f, 1.f, lineWidth * 2.f, 1.f, lineWidth * 2.f};
        else
          dash = {9.f, 3.f, 3.f, 3.f, 3.f, 3.f};
        break;

      case PS_NULL:
        isNullPen = true;
        break;
    }

    mCurrentDeviceState->mIsNullPen = isNullPen;
    if (isNullPen)
      return;

    if (lineWidth > 1 && !isGeometric)
      dash.clear();

    if (!dash.empty()){
      float const scale = 1.f / std::sqrt((std::pow(std::fabs(mCurrentDeviceState->mWorldTransform.mA), 2.f) + std::pow(std::fabs(mCurrentDeviceState->mWorldTransform.mD), 2.f)) / 2.f);
      for (auto &elem : dash)
        elem *= scale;
      mContentStream->setDashArray(dash.data(), dash.size(), 0);
    }

    // default value for line cap and join
    mContentStream->setLineCap(LineCapRound);
    mContentStream->setLineJoin(LineJoinRound);

    if (isGeometric){
      // set line cap
      if (penStyle & PS_ENDCAP_SQUARE)
        mContentStream->setLineCap(LineCapProjectingSquare);
      else if (penStyle & PS_ENDCAP_ROUND)
        mContentStream->setLineCap(LineCapRound);
      else if (penStyle & PS_ENDCAP_FLAT)
        mContentStream->setLineCap(LineCapButt);

      // set line join
      if (penStyle & PS_JOIN_BEVEL)
        mContentStream->setLineJoin(LineJoinBevel);
      else if (penStyle & PS_JOIN_MITER)
        mContentStream->setLineJoin(LineJoinMiter);
      else if (penStyle & PS_JOIN_ROUND)
        mContentStream->setLineJoin(LineJoinRound);
    }

    mContentStream->setLineWidth(lineWidth);
    setStrokingColor(color);
  }

  void Emf2PdfEngineImpl::setWorldTransform(void) const{
    mContentStream->setCTM(mCurrentDeviceState->mWorldTransform);
  }

  void Emf2PdfEngineImpl::setTextColor(void) const{
#ifdef _DEBUG
    mContentStream->setColorRGB(colorrefToRgb(RGB(0, 255, 0)));
#else
    switch (mColorSpace){
      case ColorSpaceFamilyDeviceRGB:
        mContentStream->setColorRGB(colorrefToRgb(mCurrentDeviceState->mTextColor));
        break;

      case ColorSpaceFamilyDeviceGray:
        mContentStream->setColorGray(colorrefToGray(mCurrentDeviceState->mTextColor));
        break;

      case ColorSpaceFamilyDeviceCMYK:
        mContentStream->setColorCMYK(colorrefToCmyk(mCurrentDeviceState->mTextColor));
        break;

      default:
        RUNTIME_EXCEPTION("Unsupported ColorSpaceFamily value");
    }
#endif
  }

  void Emf2PdfEngineImpl::setTextBkColor(RECTL const &rect) const{
    mContentStream->setColorRGB(colorrefToRgb(mCurrentDeviceState->mBkColor));
    mContentStream->addRectangle(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    mContentStream->closePath(pdfout::PathPaintingOpFillNW);
  }

  void Emf2PdfEngineImpl::setTextClipping(RECTL const &rect) const{
    mContentStream->addRectangle(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    mContentStream->closePath(PathPaintingOpClippingNW);
  }

  void Emf2PdfEngineImpl::setStrokingColor(COLORREF colorref) const{
    switch (mColorSpace){
      case ColorSpaceFamilyDeviceRGB:
        mContentStream->setStrokingColorRGB(colorrefToRgb(colorref));
        break;

      case ColorSpaceFamilyDeviceGray:
        mContentStream->setStrokingColorGray(colorrefToGray(colorref));
        break;

      case ColorSpaceFamilyDeviceCMYK:
        mContentStream->setStrokingColorCMYK(colorrefToCmyk(colorref));
        break;

      default:
        RUNTIME_EXCEPTION("Unsupported ColorSpaceFamily value");
    }
  }

  void Emf2PdfEngineImpl::onPatcopy(struct pdfout::Rectangle const &rclBounds){
    mContentStream->save();
      setBrush();
      drawClippingPath();
      mContentStream->addRectangle(rclBounds);
      mContentStream->closePath(PathPaintingOpFillNW);
    mContentStream->restore();
  }

  PathPaintingOp Emf2PdfEngineImpl::getPathPaintingOp(DWORD type) const{
    switch (type){
      case EMR_CHORD:
      case EMR_ELLIPSE:
      case EMR_PIE:
      case EMR_POLYGON:
      case EMR_POLYGON16:
      case EMR_POLYPOLYGON:
      case EMR_POLYPOLYGON16:
      case EMR_RECTANGLE:
      case EMR_ROUNDRECT:{
        bool const isEO = mCurrentDeviceState->mPolyfillMode == ALTERNATE;

        if (mCurrentDeviceState->mIsNullBrush && mCurrentDeviceState->mIsNullPen)
          return PathPaintingOpNope;
        else if (mCurrentDeviceState->mIsNullBrush)
          return PathPaintingOpStroke;
        else if (mCurrentDeviceState->mIsNullPen)
          return isEO ? PathPaintingOpFillEO : PathPaintingOpFillNW;

        return isEO ? PathPaintingOpFillAndStrokeEO : PathPaintingOpFillAndStrokeNW;
      }

      case EMR_ANGLEARC:
      case EMR_ARC:
      case EMR_ARCTO:
      case EMR_LINETO:
      case EMR_POLYBEZIER:
      case EMR_POLYBEZIER16:
      case EMR_POLYBEZIERTO:
      case EMR_POLYBEZIERTO16:
      case EMR_POLYDRAW:
      case EMR_POLYDRAW16:
      case EMR_POLYLINE:
      case EMR_POLYLINE16:
      case EMR_POLYLINETO:
      case EMR_POLYLINETO16:
      case EMR_POLYPOLYLINE:
      case EMR_POLYPOLYLINE16:{
        if (mCurrentDeviceState->mIsNullPen)
          return PathPaintingOpNope;

        return PathPaintingOpStroke;
      }
    }

    return PathPaintingOpStroke;
  }

  float Emf2PdfEngineImpl::toRad(float deg) const{
    return deg * ContentStream::Pi / 180.f;
  }

  void Emf2PdfEngineImpl::DPtoLP(HDC hdc, Point *pts, int count){
    //d = ( l - viewportorg.x ) / viewportext.x * windowext.x + windoworg.x
    POINT viewportOrg = {0};
    GetViewportOrgEx(hdc, &viewportOrg);

    SIZE viewportExt = {0};
    GetViewportExtEx(hdc, &viewportExt);

    POINT windowOrg = {0};
    GetWindowOrgEx(hdc, &windowOrg);

    SIZE windowExt = {0};
    GetWindowExtEx(hdc, &windowExt);

    pdfout::TransformationMatrix const tm = mCurrentDeviceState->mWorldTransform.inverse();

    for (int i = 0; i < count; ++i){
      pdfout::Vector v(pts[i].mX, pts[i].mY);
      v *= tm;

      pts[i].mX = ((float) v.mX - viewportOrg.x) / (float) viewportExt.cx * windowExt.cx + windowOrg.x;
      pts[i].mY = ((float) v.mY - viewportOrg.y) / (float) viewportExt.cy * windowExt.cy + windowOrg.y;
    }
  }

  void Emf2PdfEngineImpl::LPtoDP(HDC hdc, Point *pts, int count){
    //l = (d - windoworg.x) / windowext.x * viewportext.x + viewportorg.x
    POINT viewportOrg = {0};
    GetViewportOrgEx(hdc, &viewportOrg);

    SIZE viewportExt = {0};
    GetViewportExtEx(hdc, &viewportExt);

    POINT windowOrg = {0};
    GetWindowOrgEx(hdc, &windowOrg);

    SIZE windowExt = {0};
    GetWindowExtEx(hdc, &windowExt);

    pdfout::TransformationMatrix const tm = mCurrentDeviceState->mWorldTransform;

    for (int i = 0; i < count; ++i){
      pdfout::Vector v(pts[i].mX, pts[i].mY);
      v *= tm;

      pts[i].mX = ((float) v.mX - windowOrg.x) / (float) windowExt.cx * viewportExt.cx + viewportOrg.x;
      pts[i].mY = ((float) v.mY - windowOrg.y) / (float) windowExt.cy * viewportExt.cy + viewportOrg.y;
    }
  }

  TransformationMatrix Emf2PdfEngineImpl::xformToTM(XFORM const &xf) const{
    return TransformationMatrix((float *) &xf);
  }

  int CALLBACK Emf2PdfEngineImpl::enhMetaFileProc(HDC hdc, HANDLETABLE *table, ENHMETARECORD const *emfr, int obj, LPARAM data){
    Emf2PdfEngineImpl *engine = (Emf2PdfEngineImpl *) data;
    switch (emfr->iType){
      case EMR_ANGLEARC:
      case EMR_ARC:
      case EMR_ARCTO:
      case EMR_CHORD:
      case EMR_ELLIPSE:
      case EMR_LINETO:
      case EMR_PIE:
      case EMR_POLYBEZIER:
      case EMR_POLYBEZIER16:
      case EMR_POLYBEZIERTO:
      case EMR_POLYBEZIERTO16:
      case EMR_POLYDRAW:
      case EMR_POLYDRAW16:
      case EMR_POLYGON:
      case EMR_POLYGON16:
      case EMR_POLYLINE:
      case EMR_POLYLINE16:
      case EMR_POLYLINETO:
      case EMR_POLYLINETO16:
      case EMR_POLYPOLYGON:
      case EMR_POLYPOLYGON16:
      case EMR_POLYPOLYLINE:
      case EMR_POLYPOLYLINE16:
      case EMR_RECTANGLE:
      case EMR_ROUNDRECT:{
        if (engine->isActivePath())
          break;

        SaveDC(hdc);
          BeginPath(hdc);
            PlayEnhMetaFileRecord(hdc, table, emfr, obj);
          EndPath(hdc);

          int size = GetPath(hdc, nullptr, nullptr, 0);
          std::vector<POINT> points(size);
          std::vector<uint8_t> types(size);
          GetPath(hdc, points.data(), types.data(), size);
        RestoreDC(hdc, -1);

        engine->drawPath(points, types, emfr->iType, true);
        break;
      }
    }

    // fix for font creation process when facename is missed in the system
    switch (emfr->iType){
      case EMR_EXTCREATEFONTINDIRECTW:
        engine->onEmrExtCreateFontIndirectW(table, emfr, obj);
        return 1;
    }

    PlayEnhMetaFileRecord(hdc, table, emfr, obj);

    switch (emfr->iType){
      case EMR_SAVEDC:
        engine->onEmrSaveDc(emfr);
        break;

      case EMR_RESTOREDC:
        engine->onEmrRestoreDc(emfr);
        break;

      case EMR_RESERVED_108:
        engine->onEmrReserved108(emfr);
        break;

      case EMR_RESERVED_109:
        engine->onEmrReserved109(emfr);
        break;

      case EMR_EXTTEXTOUTA:
      case EMR_EXTTEXTOUTW:
        engine->onEmrExtTextOut(emfr);

        break;

      case EMR_BITBLT:
        engine->onEmrBitBlt(emfr);
        break;

      case EMR_STRETCHDIBITS:
        engine->onEmrStretchDiBits(emfr);
        break;

      case EMR_MASKBLT:
        engine->onEmrMaskBlt(emfr);
        break;

      case EMR_PLGBLT:
        engine->onEmrPlgBlt(emfr);
        break;

      case EMR_STRETCHBLT:
        engine->onEmrStretchBlt(emfr);
        break;

      case EMR_TRANSPARENTBLT:
        engine->onEmrTransparentBlt(emfr);
        break;

      case EMR_BEGINPATH:
        engine->onEmrBeginPath(emfr);
        break;

      case EMR_ENDPATH:
        engine->onEmrEndPath(emfr);
        break;

      case EMR_CLOSEFIGURE:
        engine->onEmrCloseFigure(emfr);
        break;

      case EMR_FILLPATH:
        engine->onEmrFillPath(emfr);
        break;

      case EMR_STROKEANDFILLPATH:
        engine->onEmrStrokeAndFillPath(emfr);
        break;

      case EMR_STROKEPATH:
        engine->onEmrStrokePath(emfr);
        break;

      case EMR_SELECTCLIPPATH:
        engine->onEmrSelectClipPath(emfr);
        break;

      case EMR_INTERSECTCLIPRECT:
        engine->onEmrIntersectClipRect(emfr);
        break;

      case EMR_EXTSELECTCLIPRGN:
        engine->onEmrExtSelectClipRgn(emfr);
        break;

      case EMR_SETTEXTCOLOR:
        engine->onEmrSetTextColor(emfr);
        break;

      case EMR_SETTEXTALIGN:
        engine->onEmrSetTextAlign(emfr);
        break;

      case EMR_PAINTRGN:
        engine->onEmrPaintRgn(emfr);
        break;

      case EMR_MOVETOEX:
        engine->onEmrMoveToEx(emfr);
        break;

      case EMR_SETMAPMODE:
        engine->onEmrSetMapMode(emfr);
        break;

      case EMR_SETPOLYFILLMODE:
        engine->onEmrSetPolyfillMode(emfr);
        break;

      case EMR_SETWORLDTRANSFORM:
      case EMR_SCALEVIEWPORTEXTEX:
      case EMR_SCALEWINDOWEXTEX:
      case EMR_SETWINDOWEXTEX:
      case EMR_SETWINDOWORGEX:
      case EMR_SETVIEWPORTEXTEX:
      case EMR_SETVIEWPORTORGEX:
        engine->onUpdateWorldTransformation();
        break;

      case EMR_MODIFYWORLDTRANSFORM:
        engine->onEmrModifyWorldTransform(emfr);
        break;

      case EMR_SETBKCOLOR:
        engine->onEmrSetBkColor(emfr);
        break;

      case EMR_SETBKMODE:
        engine->onEmrSetBkMode(emfr);
        break;

      case EMR_ALPHABLEND:
        engine->onEmrAlphaBlend(emfr);
        break;

      case EMR_GDICOMMENT:
        engine->onEmrGdiComment(emfr);
        break;


      case EMR_OFFSETCLIPRGN:
      case EMR_EXCLUDECLIPRECT:
      case EMR_SETBRUSHORGEX:
      case EMR_SETPIXELV:
      case EMR_EOF:
      case EMR_SETMAPPERFLAGS:
      case EMR_SETROP2:
      case EMR_SETSTRETCHBLTMODE:
      case EMR_SETCOLORADJUSTMENT:
      case EMR_SETMETARGN:
      case EMR_SELECTOBJECT:
      case EMR_CREATEPEN:
      case EMR_CREATEBRUSHINDIRECT:
      case EMR_DELETEOBJECT:
      case EMR_SELECTPALETTE:
      case EMR_CREATEPALETTE:
      case EMR_SETPALETTEENTRIES:
      case EMR_RESIZEPALETTE:
      case EMR_REALIZEPALETTE:
      case EMR_EXTFLOODFILL:
      case EMR_SETARCDIRECTION:
      case EMR_SETMITERLIMIT:

      case EMR_FLATTENPATH:
      case EMR_WIDENPATH:
      case EMR_ABORTPATH:
      case EMR_FILLRGN:
      case EMR_FRAMERGN:
      case EMR_INVERTRGN:

      case EMR_SETDIBITSTODEVICE:
      case EMR_CREATEMONOBRUSH:
      case EMR_CREATEDIBPATTERNBRUSHPT:
      case EMR_EXTCREATEPEN:
      case EMR_SETICMMODE:
      case EMR_CREATECOLORSPACE:
      case EMR_SETCOLORSPACE:
      case EMR_DELETECOLORSPACE:
      case EMR_GLSRECORD:
      case EMR_GLSBOUNDEDRECORD:
      case EMR_PIXELFORMAT:
      case EMR_RESERVED_105:
      case EMR_RESERVED_106:
      case EMR_RESERVED_107:
      case EMR_RESERVED_110:
      case EMR_COLORCORRECTPALETTE:
      case EMR_SETICMPROFILEA:
      case EMR_SETICMPROFILEW:
      case EMR_SETLAYOUT:
      case EMR_RESERVED_117:
      case EMR_GRADIENTFILL:
      case EMR_RESERVED_119:
      case EMR_RESERVED_120:
      case EMR_COLORMATCHTOTARGETW:
      case EMR_CREATECOLORSPACEW:
      case EMR_HEADER:
      case EMR_POLYTEXTOUTA:
      case EMR_POLYTEXTOUTW:
        break;
    }

    return 1;
  }

  void Emf2PdfEngineImpl::onEmrGdiComment(ENHMETARECORD const* emfr) {
    EMRCOMMENT* emrComment = (EMRCOMMENT*)emfr;

    switch (emrComment->mType) {
    case EmrGdiCommentTypeTextURL:
        onEmrGdiCommentTextURL(emfr);
        break;
    }
  }

}}
