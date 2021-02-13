#include  <Emf2PdfEngineImpl.h>

#include <numeric>
#include <algorithm>
#include <unordered_map>

#include <EmfTypes.h>
#include <ContentStream.h>

#undef min
#undef max

using namespace pdfout;

namespace emf2pdf{ namespace impl{

  void Emf2PdfEngineImpl::onEmrReserved108(ENHMETARECORD const *emfr){
    if (mIsActivePath)
      return;

    EMRSMALLTEXTOUT *source = (EMRSMALLTEXTOUT *) emfr;
    std::size_t elemSize = 0;
    Encoding encoding = Encoding_LATIN1;
    if (source->fOptions & ETO_SMALL_CHARS)
      elemSize = sizeof(uint8_t);
    else{
      elemSize = sizeof(wchar_t);
      encoding = Encoding_WCHAR_T;
    }

    uint8_t *text = nullptr;
    std::size_t count = 0;

    bool isClipped = source->fOptions & ETO_CLIPPED;
    RECTL clippingRect = {0};
    bool const isOpaque = source->fOptions & ETO_OPAQUE;

    if (source->fOptions & ETO_NO_RECT){
      text = (uint8_t *) source->Text;
      count = source->nChars;
    }
    else{
      EMRSMALLTEXTOUTCLIPA *clipSource = (EMRSMALLTEXTOUTCLIPA *) emfr;
      text = (uint8_t *) clipSource->Text;
      count = clipSource->nChars;
      clippingRect = clipSource->rcClip;
    }

    if (!count){
      if (isOpaque)
        setTextBkColor(clippingRect);

      return;
    }

    TextInfo textInfo = {0};
    textInfo.mGraphicsMode = source->iGraphicsMode;
    Font *font = mFontStorage->getCurrentFont(mHdc, mCurrentDeviceState->mUfi, textInfo);
    if (!font)
      return;

    UString outText("", encoding);
    if (source->fOptions & ETO_GLYPH_INDEX){
      if (elemSize == sizeof(uint8_t)){
        std::vector<uint16_t> const uniText(text, text + count);
        font->toCharCodes(uniText.data(), sizeof(uint16_t), count, outText, 0);
      }
      else
        font->toCharCodes(text, sizeof(uint16_t), count, outText, 0);
    }
    else
      outText = UString(text, count * elemSize, encoding);

    std::vector<int32_t> offDx(count, 0);

    float const toDeviceUnits = (float) textInfo.mSize / font->getUnitsPerEm(); // designUnits * fontSize / unitsPerEm;
    float const toDesignUnits = (float) font->getUnitsPerEm() / textInfo.mSize; // deviceUnits * uintsPerEm / fontSize;

    pdfout::UString const wideText = outText.encode(pdfout::Encoding_WCHAR_T);
    wchar_t const *str = (wchar_t const *) wideText.getData();

    SIZE sz = {0};
    for (std::size_t i = 0; i < count - 1; ++i){
      float const ch = (float) font->getCharWidth(str[i]) * toDeviceUnits;
      GetTextExtentPoint32W(mHdc, &str[i], 1, &sz);
      offDx[i + 1] = -std::lround(((float) sz.cx - ch) * toDesignUnits);
    }

    // intercharacters corrections
    if (mEnableIntercharacterSpacingFix){
      int32_t const space = std::lround((float) std::accumulate(std::begin(offDx), std::end(offDx), 0.f) / offDx.size());
      for (auto &elem : offDx)
        elem = space;
    }

    if (mEnableUnicodeBidiAlgorithm && encoding == Encoding_WCHAR_T){
      std::vector<uint32_t> transpositions;
      outText = outText.applyUnicodeBidiAlgorithm(transpositions);
      for (std::size_t i = 0; i < transpositions.size(); ++i){
        if (transpositions[i] >= i)
          std::swap(offDx[i], offDx[transpositions[i]]);
      }
    }

    Point const currPos = mCurrentDeviceState->mCurrentPos;
    POINT pos = {source->ptlReference.x, source->ptlReference.y};
    if (mCurrentDeviceState->mUpdateCP)
      pos = {(LONG) currPos.mX, (LONG) currPos.mY};

    float textScale = 1.f;
    if (source->iGraphicsMode == GM_COMPATIBLE && (source->eyScale * source->exScale) != 0.f)
      textScale *= std::fabs(source->exScale / source->eyScale);

    TransformationMatrix ttm = {textScale, 0.f, 0.f, 1.f, 0.f, 0.f};
    if (textInfo.mWritingMode == pdfout::WritingModeHorizontal || textInfo.mWritingMode != font->getWritingMode())
      ttm *= TransformationMatrix::createRotationMatrix(toRad((float) -textInfo.mEscapement / 10.f));

    if (textInfo.mIsItalic && !font->isItalic()) {
        ttm *= TransformationMatrix::createSkewingMatrix(0.f, -0.33f);
        ttm *= TransformationMatrix::createTranslationMatrix(textInfo.mSize * 0.33f, 0.f);
    }

    float const det = mCurrentDeviceState->mWorldTransform.getDeterminant();
    if (det >= 0.f)
      ttm *= TransformationMatrix(1, 0, 0, -1, 0, 0);

    uint16_t decoration = 0;

    bool const isUnderline = textInfo.mIsUnderline && !font->isUnderline();
    if (isUnderline)
        decoration |= TextDecorationPropertyUnderline;

    mContentStream->save();
      drawClippingPath();

      if (count){
        setWorldTransform();

        if (isOpaque)
          setTextBkColor(clippingRect);

        if (isClipped)
          setTextClipping(clippingRect);

        mContentStream->beginText();
          setTextColor();
          mContentStream->setTextFont(font, textInfo.mSize);
          mContentStream->setTextMatrix(ttm);
          mContentStream->showText(outText, pos.x, pos.y, mCurrentDeviceState->mTextAlignment, decoration, count > 1 ? &offDx[1] : nullptr, offDx.size() - 1);
        mContentStream->endText();

      }

    mContentStream->restore();

    if (mCurrentDeviceState->mUpdateCP)
      onUpdateCurrentPosition();
  }

  void Emf2PdfEngineImpl::onEmrExtTextOut(ENHMETARECORD const *emfr){
    if (mIsActivePath)
      return;

    EMREXTTEXTOUTA *source = (EMREXTTEXTOUTA *) emfr;
    std::size_t elemSize = sizeof(uint8_t);
    Encoding encoding = Encoding_LATIN1;

    if (emfr->iType == EMR_EXTTEXTOUTW){
      elemSize = sizeof(wchar_t);
      encoding = Encoding_WCHAR_T;
    }

    uint8_t *text = (uint8_t *) ((uint8_t *) emfr + source->emrtext.offString);
    std::size_t const count = source->emrtext.nChars;
    bool const isClipped = source->emrtext.fOptions & ETO_CLIPPED;
    bool const isOpaque = source->emrtext.fOptions & ETO_OPAQUE;
    RECTL const clippingRect = source->emrtext.rcl;

    if (!count){
      if (isOpaque)
        setTextBkColor(clippingRect);

      return;
    }
 
    TextInfo textInfo = {0};
    textInfo.mGraphicsMode = source->iGraphicsMode;
    Font *font = mFontStorage->getCurrentFont(mHdc, mCurrentDeviceState->mUfi, textInfo);
    if (!font)
      return;

    UString outText("", encoding);
    if (source->emrtext.fOptions & ETO_GLYPH_INDEX)
      font->toCharCodes(text, elemSize, count, outText, 0);
    else
      outText = UString(text, count * elemSize, encoding);

    std::vector<int32_t> offDx(count, 0);

    if (count > 1) {
      int32_t const *offDxPtr = (int32_t const *)((uint8_t *) source + source->emrtext.offDx);
      if (source->emrtext.fOptions & ETO_PDY){
        for (int i = 0; i < count - 1; ++i)
          offDx[i + 1] = offDxPtr[i * 2];
      }
      else{
        memcpy(&offDx[1], offDxPtr, (count - 1) * sizeof(int32_t));
      }
    }

    float const toDeviceUnits = (float) textInfo.mSize / font->getUnitsPerEm(); // designUnits * fontSize / unitsPerEm;
    float const toDesignUnits = (float) font->getUnitsPerEm() / textInfo.mSize; // deviceUnits * uintsPerEm / fontSize;

    wchar_t const *str = (wchar_t const *) outText.getData();
    for (std::size_t i = 0; i < count - 1; ++i){
      float const ch = (float) font->getCharWidth(str[i]) * toDeviceUnits;
      offDx[i + 1] = -std::lround(((float) offDx[i + 1] - ch) * toDesignUnits);
    }

    // intercharacters corrections
    if (mEnableIntercharacterSpacingFix){
      int32_t const space = std::lround((float) std::accumulate(std::begin(offDx), std::end(offDx), 0.f) / offDx.size());
      for (auto &elem : offDx)
        elem = space;
    }

    if (mEnableUnicodeBidiAlgorithm){
      std::vector<uint32_t> transpositions;
      outText = outText.applyUnicodeBidiAlgorithm(transpositions);
      for (std::size_t i = 0; i < transpositions.size(); ++i){
        if (transpositions[i] >= i)
          std::swap(offDx[i], offDx[transpositions[i]]);
      }
    }

    Point const currPos = mCurrentDeviceState->mCurrentPos;
    POINT pos = {source->emrtext.ptlReference.x, source->emrtext.ptlReference.y};

    if (mCurrentDeviceState->mUpdateCP)
      pos = {(LONG) currPos.mX, (LONG) currPos.mY};

    float textScale = 1.f;
    if (source->iGraphicsMode == GM_COMPATIBLE && (source->eyScale * source->exScale) != 0.f)
      textScale *= std::fabs(source->exScale / source->eyScale);

    TransformationMatrix ttm = {textScale, 0.f, 0.f, 1.f, 0.f, 0.f};

    if (textInfo.mWritingMode == pdfout::WritingModeHorizontal || textInfo.mWritingMode != font->getWritingMode())
      ttm *= TransformationMatrix::createRotationMatrix(toRad((float) -textInfo.mEscapement / 10.f));

    if (textInfo.mIsItalic && !font->isItalic()) {
      ttm *= TransformationMatrix::createSkewingMatrix(0.f, -0.33f);
      ttm *= TransformationMatrix::createTranslationMatrix(textInfo.mSize * 0.33f, 0.f);
    }

    float const det = mCurrentDeviceState->mWorldTransform.getDeterminant();
    if (det >= 0.f)
      ttm *= TransformationMatrix(1, 0, 0, -1, 0, 0);

    uint16_t decoration = 0;

    bool const isUnderline = textInfo.mIsUnderline && !font->isUnderline();
    if (isUnderline)
      decoration |= TextDecorationPropertyUnderline;

    mContentStream->save();
      drawClippingPath();

      setWorldTransform();
      if (isOpaque)
        setTextBkColor(clippingRect);

      if (isClipped)
        setTextClipping(clippingRect);

      mContentStream->beginText();
        setTextColor();
        mContentStream->setTextFont(font, textInfo.mSize);
        mContentStream->setTextMatrix(ttm);
        mContentStream->showText(outText, pos.x, pos.y, mCurrentDeviceState->mTextAlignment, decoration, count > 1 ? &offDx[1] : nullptr, offDx.size() - 1);
      mContentStream->endText();

    mContentStream->restore();

    if (mCurrentDeviceState->mUpdateCP)
      onUpdateCurrentPosition();
  }

  void Emf2PdfEngineImpl::onEmrSetTextColor(ENHMETARECORD const *emfr){
    EMRSETTEXTCOLOR *source = (EMRSETTEXTCOLOR *) emfr;
    mCurrentDeviceState->mTextColor = source->crColor;
  }

  void Emf2PdfEngineImpl::onEmrSetTextAlign(ENHMETARECORD const *emfr){
    EMRSETTEXTALIGN *source = (EMRSETTEXTALIGN *) emfr;
    uint16_t alignment = 0;

    switch ((TA_LEFT | TA_RIGHT | TA_CENTER) & source->iMode){
      case TA_LEFT:
        alignment |= AlignmentLeft;
        break;

      case TA_RIGHT:
        alignment |= AlignmentRight;
        break;

      case TA_CENTER:
        alignment |= AlignmentHCenter;
        break;
    }

    switch ((TA_TOP | TA_BASELINE | TA_BOTTOM) & source->iMode){
      case TA_TOP:
        alignment |= AlignmentTop;
        break;

      case TA_BASELINE:
        alignment |= AlignmentBaseline;
        break;

      case TA_BOTTOM:
        alignment |= AlignmentBottom;
        break;
    }

    mCurrentDeviceState->mTextAlignment = alignment;
    mCurrentDeviceState->mUpdateCP = (source->iMode & (TA_UPDATECP | TA_NOUPDATECP)) == TA_UPDATECP;
  }

  void Emf2PdfEngineImpl::onEmrSetBkColor(ENHMETARECORD const *emfr){
    EMRSETBKCOLOR *source = (EMRSETBKCOLOR *) emfr;
    mCurrentDeviceState->mBkColor = source->crColor;
  }

  void Emf2PdfEngineImpl::onEmrSetBkMode(ENHMETARECORD const *emfr){
    EMRSETBKMODE *source = (EMRSETBKMODE *) emfr;
    mCurrentDeviceState->mBkMode = source->iMode;
  }

  void Emf2PdfEngineImpl::onEmrGdiCommentTextURL(ENHMETARECORD const* emfr){
    EMREXTTEXTOUTURLW *source = (EMREXTTEXTOUTURLW *) emfr;
  }

}}
