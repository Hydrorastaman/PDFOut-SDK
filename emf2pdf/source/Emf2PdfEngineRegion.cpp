#include  <Emf2PdfEngineImpl.h>

#include <EmfTypes.h>
#include <ContentStream.h>

using namespace pdfout;

namespace emf2pdf{ namespace impl{

  void Emf2PdfEngineImpl::onEmrPaintRgn(ENHMETARECORD const *emfr){
    EMRPAINTRGN *source = (EMRPAINTRGN *) emfr;
    RGNDATA *rgnData = (RGNDATA *) source->RgnData;
    RECT *rgnRect = (RECT *) rgnData->Buffer;

    struct Rectangle pathRect = {(float) rgnRect->left, (float) rgnRect->bottom, (float) rgnRect->right, (float) rgnRect->top};

    mContentStream->save();
      setWorldTransform();
      setBrush();
      mContentStream->beginPath(pathRect.mLowerLeftX, pathRect.mLowerLeftY);
      mContentStream->addRectangle(pathRect);
      mContentStream->closePath(PathPaintingOpFillNW);
    mContentStream->restore();
  }

}}
