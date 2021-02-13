#include  <Emf2PdfEngineImpl.h>

#include <Page.h>
#include <ContentStream.h>

#include <EmfTypes.h>
#include <GenericHandle.h>

namespace{
  uint8_t const PT_RECT = 0x08;
}

using namespace pdfout;

namespace emf2pdf{ namespace impl{

  void Emf2PdfEngineImpl::drawPath(std::vector<POINT> const &points, std::vector<BYTE> const &types, DWORD type, bool isStandAlone){
    std::size_t const cpts = types.size();

    if (isStandAlone){
      mContentStream->save();
      drawClippingPath();
      setWorldTransform();
      setBrush();
      setPen();
    }

    for (auto i = 0; i < cpts; ++i){
      switch (types[i] & ~PT_CLOSEFIGURE){
        case PT_MOVETO:
          mContentStream->beginPath((float) points[i].x, (float) points[i].y);
          break;

        case PT_LINETO:
          mContentStream->addLine((float) points[i].x, (float) points[i].y);
          break;

        case PT_BEZIERTO:
          mContentStream->addBezierCurve12((float) points[i].x, (float) points[i].y, (float) points[i + 1].x, (float) points[i + 1].y, (float) points[i + 2].x, (float) points[i + 2].y);
          i += 2;
          break;

        case PT_RECT:
          mContentStream->addRectangle((float) points[i].x, (float) points[i].y, (float) points[i + 1].x, (float) points[i + 1].y);
          i += 1;
          break;
      }

      if (types[i] & PT_CLOSEFIGURE)
        mContentStream->closeSubPath();
    }

    if (isStandAlone){
      mContentStream->closePath(getPathPaintingOp(type));
      mContentStream->restore();
    }
  }

  void Emf2PdfEngineImpl::drawClippingPath(void){
    if (mCurrentDeviceState->mIsClipped){
      drawPath(mCurrentDeviceState->mClippingPathPoints, mCurrentDeviceState->mClippingPathTypes, 0, false);
      PathPaintingOp const pathClippingOp = mCurrentDeviceState->mPolyfillMode == ALTERNATE ? PathPaintingOpClippingEO : PathPaintingOpClippingNW;
      mContentStream->closePath(pathClippingOp);
    }
  }

  void Emf2PdfEngineImpl::selectClippingPath(void){
    Rgn rgn(CreateRectRgn(0, 0, 1, 1));
    if (!GetClipRgn(mHdc, rgn)){
      mCurrentDeviceState->mIsClipped = false;
      return;
    }

    DWORD const rgnSize = GetRegionData(rgn, 0, nullptr);
    std::vector<uint8_t> rgnDataBuff(rgnSize);
    GetRegionData(rgn, rgnSize, (RGNDATA *) rgnDataBuff.data());
    RGNDATA *rawData = (RGNDATA *) rgnDataBuff.data();

    uint32_t const count = rawData->rdh.nCount;
    if (!count){
      mCurrentDeviceState->mIsClipped = false;
      return;
    }

    mCurrentDeviceState->mIsClipped = true;
    mCurrentDeviceState->mClippingPathPoints.resize(count * 2);
    mCurrentDeviceState->mClippingPathTypes.resize(count * 2);

    RECT *rect = (RECT *) rawData->Buffer;

    for (std::size_t i = 0; i < count; ++i){
      mCurrentDeviceState->mClippingPathPoints[i * 2] = {rect[i].left, rect[i].top};
      mCurrentDeviceState->mClippingPathPoints[i * 2 + 1] = {rect[i].right - rect[i].left, rect[i].bottom - rect[i].top};
      mCurrentDeviceState->mClippingPathTypes[i * 2] = PT_RECT;
      mCurrentDeviceState->mClippingPathTypes[i * 2 + 1] = PT_RECT;// | PT_CLOSEFIGURE;
    }
  }

  void Emf2PdfEngineImpl::onEmrBeginPath(ENHMETARECORD const *emfr){
    mIsActivePath = true;
    mActivePathPoints.clear();
    mActivePathTypes.clear();
  }

  void Emf2PdfEngineImpl::onEmrEndPath(ENHMETARECORD const *emfr){
    int size = GetPath(mHdc, nullptr, nullptr, 0);
    mActivePathPoints.resize(size);
    mActivePathTypes.resize(size);

    GetPath(mHdc, mActivePathPoints.data(), mActivePathTypes.data(), size);
    mIsActivePath = false;
  }

  void Emf2PdfEngineImpl::onEmrCloseFigure(ENHMETARECORD const *emfr){
  }

  void Emf2PdfEngineImpl::onEmrFillPath(ENHMETARECORD const *emfr){
    PathPaintingOp pathPaintingOp = mCurrentDeviceState->mPolyfillMode == ALTERNATE ? PathPaintingOpFillEO : PathPaintingOpFillNW;
    mContentStream->save();
      drawClippingPath();
      setWorldTransform();
      setBrush();
      drawPath(mActivePathPoints, mActivePathTypes, 0, false);
      mContentStream->closePath(pathPaintingOp);
    mContentStream->restore();
  }

  void Emf2PdfEngineImpl::onEmrStrokeAndFillPath(ENHMETARECORD const *emfr){
    PathPaintingOp const pathPaintingOp = mCurrentDeviceState->mPolyfillMode == ALTERNATE ? PathPaintingOpCloseFillStrokeEO : PathPaintingOpCloseFillStrokeNW;

    mContentStream->save();
      drawClippingPath();
      setWorldTransform();
      setBrush();
      setPen();
      drawPath(mActivePathPoints, mActivePathTypes, 0, false);
      mContentStream->closePath(pathPaintingOp);
    mContentStream->restore();
  }

  void Emf2PdfEngineImpl::onEmrStrokePath(ENHMETARECORD const *emfr){
auto t = (EMRSTROKEPATH *) emfr;
    mContentStream->save();
      drawClippingPath();
      setWorldTransform();
      setPen();
      drawPath(mActivePathPoints, mActivePathTypes, 0, false);
      mContentStream->closePath(PathPaintingOpStroke);
    mContentStream->restore();
  }

  void Emf2PdfEngineImpl::onEmrSelectClipPath(ENHMETARECORD const *emfr){
    EMRSELECTCLIPPATH *source = (EMRSELECTCLIPPATH *) emfr;
    selectClippingPath();
  }

  void Emf2PdfEngineImpl::onEmrIntersectClipRect(ENHMETARECORD const *emfr){
    EMRINTERSECTCLIPRECT *source = (EMRINTERSECTCLIPRECT *) emfr;
    selectClippingPath();
  }

  void Emf2PdfEngineImpl::onEmrExtSelectClipRgn(ENHMETARECORD const *emfr){
    EMREXTSELECTCLIPRGN *source = (EMREXTSELECTCLIPRGN *) emfr;
    selectClippingPath();
  }

}}
