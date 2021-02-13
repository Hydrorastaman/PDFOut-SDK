#include <ContentStreamImpl.h>

#include <string>
#include <numeric>

#include <Font.h>
#include <Image.h>
#include <Tools.h>
#include <ToString.h>
#include <ExtGState.h>
#include <ResourceManager.h>
#include <graphics/Operators.h>
#include <object/ObjectStream.h>
#include <stream/MemoryOStream.h>
#include <graphics/GraphicsState.h>
#include <object/ObjectUTF16BEString.h>

using namespace kernel;

namespace pdfout{ namespace impl{

  ContentStreamImpl::ContentStreamImpl(ResourceManager *resourceManager)
    : ContentStream(),
    mCurrentGraphicsState(std::make_unique<GraphicsState>()),
    mGraphicsStateStack(),
    mOperators(),
    mResourceManager(resourceManager){
  }

  ContentStreamImpl::~ContentStreamImpl(void){
  }

  void ContentStreamImpl::serialize(kernel::ObjectStream *stream) const{
    for (auto const &cmd : mOperators){
      cmd->serialize(stream);
      stream->writeEOL();
    }
  }

  void ContentStreamImpl::push(Operator *cmd){
    mOperators.push_back(std::unique_ptr<Operator>(cmd));
  }

  void ContentStreamImpl::save(void){
    push(makeOperator_q());
    mGraphicsStateStack.push(std::unique_ptr<GraphicsState>(mCurrentGraphicsState->clone()));
  }

  void ContentStreamImpl::restore(void){
    if (!mGraphicsStateStack.empty()){
      push(makeOperator_Q());
      mCurrentGraphicsState.reset(mGraphicsStateStack.top().release());
      mGraphicsStateStack.pop();
    }
  }

  void ContentStreamImpl::setCTM(TransformationMatrix const &ctm){
    push(makeOperator_cm(ctm));
    mCurrentGraphicsState->mCTM = ctm;
  }

  TransformationMatrix ContentStreamImpl::getCTM(void) const{
    return mCurrentGraphicsState->mCTM;
  }

  void ContentStreamImpl::setLineWidth(float lineWidth){
    push(makeOperator_w(lineWidth));
    mCurrentGraphicsState->mLineWidth = lineWidth;
  }

  void ContentStreamImpl::setLineCap(LineCap lineCap){
    push(makeOperator_J(lineCap));
    mCurrentGraphicsState->mLineCap = lineCap;
  }

  void ContentStreamImpl::setLineJoin(LineJoin lineJoin){
    push(makeOperator_j(lineJoin));
    mCurrentGraphicsState->mLineJoin = lineJoin;
  }

  void ContentStreamImpl::setMiterLimit(float miterLimit){
    push(makeOperator_M(miterLimit));
    mCurrentGraphicsState->mMiterLimit = miterLimit;
  }

  void ContentStreamImpl::setDashArray(float const *dashPattern, std::size_t count, std::size_t phase){
    push(makeOperator_d(dashPattern, count, phase));
    //mCurrentGraphicsState->mDashPattern = dashPattern;
  }

  void ContentStreamImpl::setIntent(RenderingIntent renderingIntent){
    push(makeOperator_ri(toString(renderingIntent).c_str()));
    mCurrentGraphicsState->mRenderingIntent = renderingIntent;
  }

  void ContentStreamImpl::setFlatness(float flatness){
    push(makeOperator_i(flatness));
    mCurrentGraphicsState->mFlatness = flatness;
  }

  void ContentStreamImpl::setExtGState(ExtGState *extGState){
    ExtGStateImpl *extGStateImpl = dynamic_cast<ExtGStateImpl *>(extGState);
    push(makeOperator_gs(extGStateImpl->getUID().c_str()));
    // TODO: copy extGState values to mCurrentGraphicsState->mExtGState
    mResourceManager->addExtGState(extGStateImpl);
  }

  void ContentStreamImpl::beginText(void){
    push(makeOperator_BT());
    mResourceManager->addProcSet(ProcSetText);
  }

  void ContentStreamImpl::endText(void){
    push(makeOperator_ET());
  }

  void ContentStreamImpl::setTextCharSpacing(float charSpacing){
    push(makeOperator_Tc(charSpacing));
    mCurrentGraphicsState->mTextState.mCharacterSpacing = charSpacing;
  }

  void ContentStreamImpl::setTextWordSpacing(float wordSpacing){
    push(makeOperator_Tw(wordSpacing));
    mCurrentGraphicsState->mTextState.mWordSpacing = wordSpacing;
  }

  void ContentStreamImpl::setTextScale(float scale){
    push(makeOperator_Tz(scale));
    mCurrentGraphicsState->mTextState.mHorizontalScaling = scale;
  }

  void ContentStreamImpl::setTextLeading(int32_t leading){
    push(makeOperator_TL(leading));
    mCurrentGraphicsState->mTextState.mLeading = leading;
  }

  void ContentStreamImpl::setTextFont(Font *font, int32_t fontSize){
    FontImpl *fontImpl = dynamic_cast<FontImpl *>(font);
    std::string const fontName = fontImpl->getUID();
    push(makeOperator_Tf(fontName.c_str(), fontSize));

    mCurrentGraphicsState->mTextState.mTextFont = fontName;
    mCurrentGraphicsState->mTextState.mTextFontSize = fontSize;
    mCurrentGraphicsState->mTextState.mFont = fontImpl;
    mResourceManager->addFont(fontImpl);
  }

  void ContentStreamImpl::setTextRender(TextRenderingMode render){
    push(makeOperator_Tr(render));
    mCurrentGraphicsState->mTextState.mTextRenderingMode = render;
  }

  void ContentStreamImpl::setTextRise(int32_t rise){
    push(makeOperator_Ts(rise));
    mCurrentGraphicsState->mTextState.mTextRise = rise;
  }

  void ContentStreamImpl::setTextPosition(float x, float y){
    push(makeOperator_Td(x, y));
  }

  void ContentStreamImpl::setTextAbsolutePosition(float x, float y){
    setTextMatrix(TransformationMatrix::createTranslationMatrix(x, y));
  }

  void ContentStreamImpl::setTextPositionAndLeading(float x, float y){
    push(makeOperator_TD(x, y));
    mCurrentGraphicsState->mTextState.mLeading = -y;
  }
                                                          
  void ContentStreamImpl::setTextMatrix(TransformationMatrix const &textMatrix){
    push(makeOperator_Tm(textMatrix));
    mCurrentGraphicsState->mTextState.mCTM = textMatrix;
  }

  void ContentStreamImpl::setTextNewLine(void){
    push(makeOperator_T_ast());
  }

  void ContentStreamImpl::showText(pdfout::UString const &text){
    std::unique_ptr<kernel::ObjectUTF16BEString> obj = std::make_unique<kernel::ObjectUTF16BEString>(text);
    mResourceManager->updateUsedCIDs(mCurrentGraphicsState->mTextState.mFont, obj->getData(), obj->getSize());
    push(makeOperator_Tj(std::move(obj)));
  }

  void ContentStreamImpl::showTextAtNewLine(pdfout::UString const &text){
    std::unique_ptr<kernel::ObjectUTF16BEString> obj = std::make_unique<kernel::ObjectUTF16BEString>(text);
    mResourceManager->updateUsedCIDs(mCurrentGraphicsState->mTextState.mFont, obj->getData(), obj->getSize());
    push(makeOperator_quote(std::move(obj)));
  }

  void ContentStreamImpl::showTextAtNewLine(float wordSpacing, float charSpacing, pdfout::UString const &text){
    std::unique_ptr<kernel::ObjectUTF16BEString> obj = std::make_unique<kernel::ObjectUTF16BEString>(text);
    mResourceManager->updateUsedCIDs(mCurrentGraphicsState->mTextState.mFont, obj->getData(), obj->getSize());
    push(makeOperator_dquote(wordSpacing, charSpacing, std::move(obj)));
  }

  void ContentStreamImpl::showText(pdfout::UString const &text, struct Rectangle const &rect, uint16_t alignment, uint16_t decoration){
    std::unique_ptr<kernel::ObjectUTF16BEString> obj = std::make_unique<kernel::ObjectUTF16BEString>(text);
    mResourceManager->updateUsedCIDs(mCurrentGraphicsState->mTextState.mFont, obj->getData(), obj->getSize());

    FontImpl *font = mCurrentGraphicsState->mTextState.mFont;
    if (!font)
      return;

    std::vector<int32_t> widths;
    float const fontSize = mCurrentGraphicsState->mTextState.mTextFontSize;
    float const lineGap = font->getLineGap() * fontSize / 1000.f;
    float const textHeight = mCurrentGraphicsState->mTextState.mTextFontSize - lineGap;
    float const textScale = mCurrentGraphicsState->mTextState.mHorizontalScaling / 100.f;
    float const textWidth = font->getTextWidth(text, widths) * fontSize / font->getUnitsPerEm() * textScale;
    float const rWidth = rect.mUpperRightX - rect.mLowerLeftX;
    float const rHeight = rect.mUpperRightY - rect.mLowerLeftY;

    float tx = rect.mLowerLeftX;
    if (alignment & AlignmentLeft)
      tx = rect.mLowerLeftX;
    else if (alignment & AlignmentRight)
      tx = rect.mLowerLeftX + rWidth - textWidth;
    else if (alignment & AlignmentHCenter)
      tx = rect.mLowerLeftX + (rWidth - textWidth) / 2.f;

    float ty = rect.mLowerLeftY;
    if (alignment & AlignmentBottom)
      ty = rect.mLowerLeftY;
    else if (alignment & AlignmentTop)
      ty = rect.mLowerLeftY + rHeight - textHeight;
    else if (alignment & AlignmentVCenter)
      ty = rect.mLowerLeftY + (rHeight - textHeight) / 2.f;

    TransformationMatrix tm = mCurrentGraphicsState->mTextState.mCTM;
    tm.mE += tx;
    tm.mF += ty;
    push(makeOperator_Tm(tm));
    push(makeOperator_Tj(std::move(obj)));

    bool bForceTextUnderline = decoration & TextDecorationPropertyUnderline;
    if (bForceTextUnderline) {
      float const unitsPerEm = font->getUnitsPerEm();
      float const mult = fontSize / unitsPerEm;
      float const winDescent = font->getWinDescent() * mult;
      float const lineWidth = (float)font->getUnderlineYSize() * mult;

      forceTextUnderline(textWidth, winDescent, lineWidth, tm);
    }
  }

  void ContentStreamImpl::showText(pdfout::UString const &text, float x, float y, uint16_t alignment, uint16_t decoration, int32_t *distance, std::size_t count){
    std::unique_ptr<kernel::ObjectUTF16BEString> obj = std::make_unique<kernel::ObjectUTF16BEString>(text);
    mResourceManager->updateUsedCIDs(mCurrentGraphicsState->mTextState.mFont, obj->getData(), obj->getSize());

    FontImpl *font = mCurrentGraphicsState->mTextState.mFont;
    if (!font)
      return;

    std::vector<int32_t> widths;
    float const fontSize = mCurrentGraphicsState->mTextState.mTextFontSize;
    float const unitsPerEm = font->getUnitsPerEm();
    float const mult = fontSize / unitsPerEm;
    float const textWidth = font->getTextWidth(text, widths) * mult;
    float const winAscent = font->getWinAscent() * mult;
    float const winDescent = font->getWinDescent() * mult;

    float const det = (mCurrentGraphicsState->mTextState.mCTM * mCurrentGraphicsState->mCTM).getDeterminant();
    float const sign = det >= 0.f ? 1.f : -1.f;

    bool const isHorizontal = font->getWritingMode() == pdfout::WritingModeHorizontal;

    float dx = 0.f;
    float dy = 0.f;

    if (isHorizontal){
      if (alignment & AlignmentLeft)
        dx = 0;
      else if (alignment & AlignmentRight)
        dx = sign * textWidth;
      else if (alignment & AlignmentHCenter)
        dx = sign * textWidth / 2.f;

      if (alignment & AlignmentBottom)
        dy = -sign * winDescent;
      else if (alignment & AlignmentTop)
        dy = sign * winAscent;
      else if (alignment & AlignmentBaseline)
        dy = 0;
    }
    else{
      dx = -sign * (winAscent - winDescent) / 2.f;

      if (alignment & AlignmentBottom)
        dy = -sign *textWidth;
      else if (alignment & AlignmentTop)
        dy = sign * winAscent;
      else if (alignment & AlignmentBaseline)
        dy = 0;
    }

    TransformationMatrix tm;
    tm *= TransformationMatrix::createTranslationMatrix(x, y);
    tm *= mCurrentGraphicsState->mTextState.mCTM;
    tm *= TransformationMatrix::createTranslationMatrix(dx, dy);
    push(makeOperator_Tm(tm));

    float const toFontSpace = 1000.f / font->getUnitsPerEm();

    if (distance && count){
      std::vector<int32_t> positions(count, 0);
      for (std::size_t i = 0; i < count; ++i)
        positions[i] = std::lround((float) distance[i] * toFontSpace);

      push(makeOperator_TJ(std::move(obj), positions));
    }
    else
      push(makeOperator_Tj(std::move(obj)));

    bool bForceTextUnderline = decoration & TextDecorationPropertyUnderline;
    if (bForceTextUnderline){
      float const lineWidth = (float) font->getUnderlineYSize() * mult;

      float totalDistance = 0;
      for (std::size_t i = 0; i < count; ++i)
          totalDistance += distance[i];

      totalDistance *= mult;

      forceTextUnderline(textWidth - totalDistance, winDescent, lineWidth, tm);
    }
  }

  void ContentStreamImpl::forceTextUnderline(float textWidth, float winDescent, float lineWidth, TransformationMatrix const &tm){
    float const offset = winDescent * 0.7;
    setLineWidth(lineWidth);

    switch (mCurrentGraphicsState->mColorSpace->getFamily()) {
      case ColorSpaceFamilyDeviceRGB:
        setStrokingColorRGB(mCurrentGraphicsState->mColorRGB);
        break;

      case ColorSpaceFamilyDeviceGray:
        setStrokingColorGray(mCurrentGraphicsState->mColorGray);
        break;

      case ColorSpaceFamilyDeviceCMYK:
        setStrokingColorCMYK(mCurrentGraphicsState->mColorCMYK);
        break;
    }

    setCTM(tm);
    beginPath(0, -offset);
    addLine(textWidth, -offset);
    closePath();
  }

  void ContentStreamImpl::showImage(Image *image, float x, float y, float scaleX, float scaleY, float rotate){
    ImageImpl *imageImpl = dynamic_cast<ImageImpl *>(image);
    TransformationMatrix tm;
    tm *= TransformationMatrix::createTranslationMatrix(x, y);
    tm *= TransformationMatrix::createRotationMatrix(rotate);
    tm *= TransformationMatrix::createScalingMatrix(scaleX, scaleY);

    push(makeOperator_cm(tm));
    push(makeOperator_Do(imageImpl->getUID().c_str()));

    mResourceManager->addImage(imageImpl);
    mResourceManager->addProcSet(ProcSetImageC);
  }

  void ContentStreamImpl::showImage(Image *image, TransformationMatrix const &tm){
    ImageImpl *imageImpl = dynamic_cast<ImageImpl *>(image);
    push(makeOperator_cm(tm));
    push(makeOperator_Do(imageImpl->getUID().c_str()));

    mResourceManager->addImage(imageImpl);
    mResourceManager->addProcSet(ProcSetImageC);
  }

  void ContentStreamImpl::setColorSpace(ColorSpace *colorSpace){
    ColorSpaceImpl *colorSpaceImpl = dynamic_cast<ColorSpaceImpl *>(colorSpace);
    switch (colorSpace->getFamily()){
      case ColorSpaceFamilyDeviceGray:
      case ColorSpaceFamilyDeviceRGB:
      case ColorSpaceFamilyDeviceCMYK:
        push(makeOperator_cs(toString(colorSpace->getFamily()).c_str()));
        break;

      default:
        push(makeOperator_cs(colorSpaceImpl->getUID().c_str()));
        mResourceManager->addColorSpace(colorSpaceImpl);
    }

    mCurrentGraphicsState->mColorSpace = colorSpace;
  }

  void ContentStreamImpl::setStrokingColorSpace(ColorSpace *colorSpace){
    ColorSpaceImpl *colorSpaceImpl = dynamic_cast<ColorSpaceImpl *>(colorSpace);
    switch (colorSpaceImpl->getFamily()){
      case ColorSpaceFamilyDeviceGray:
      case ColorSpaceFamilyDeviceRGB:
      case ColorSpaceFamilyDeviceCMYK:
        push(makeOperator_CS(toString(colorSpaceImpl->getFamily()).c_str()));
        break;

      default:
        push(makeOperator_CS(colorSpaceImpl->getUID().c_str()));
    }

    mCurrentGraphicsState->mStrokingColorSpace = colorSpaceImpl;
  }

  void ContentStreamImpl::setColorGray(float gray){
    mCurrentGraphicsState->mColorGray = Color::createGray(gray);
    push(makeOperator_g(gray));
  }

  void ContentStreamImpl::setColorGray(Color const &color){
    push(makeOperator_g(color.mValue.mGray.mGray));
  }

  void ContentStreamImpl::setColorIndex(uint32_t index){
    Color const color = Color::createIndex(index);
    setColor(color);
  }

  void ContentStreamImpl::setColorRGB(float red, float green, float blue){
    mCurrentGraphicsState->mColorRGB = Color::createRGB(red, green, blue);
    push(makeOperator_rg(red, green, blue));
  }

  void ContentStreamImpl::setColorRGB(Color const &color){
    setColorRGB(color.mValue.mRGB.mRed, color.mValue.mRGB.mGreen, color.mValue.mRGB.mBlue);
  }

  void ContentStreamImpl::setColorRGB(ColorName colorName){
    setColorRGB(Color::createRGB(colorName));
  }

  void ContentStreamImpl::setColorCMYK(float cyan, float magenta, float yellow, float black){
    mCurrentGraphicsState->mColorCMYK = Color::createCMYK(cyan, magenta, yellow, black);
    push(makeOperator_k(cyan, magenta, yellow, black));
  }

  void ContentStreamImpl::setColorCMYK(Color const &color){
    setColorCMYK(color.mValue.mCMYK.mCyan, color.mValue.mCMYK.mMagenta, color.mValue.mCMYK.mYellow, color.mValue.mCMYK.mBlack);
  }

  void ContentStreamImpl::setColorCMYK(ColorName colorName){
    setColorCMYK(Color::createCMYK(colorName));
  }

  void ContentStreamImpl::setColor(Color const &color){
    mCurrentGraphicsState->mColor = color;
    switch(color.mType){
      case ColorTypeGray:
        push(makeOperator_sc(color.mValue.mGray.mGray));
        break;

      case ColorTypeIndex:
        push(makeOperator_sc(color.mValue.mIndex.mIndex));
        break;

      case ColorTypeRGB:
        push(makeOperator_sc(color.mValue.mRGB.mRed, color.mValue.mRGB.mGreen, color.mValue.mRGB.mBlue));
        break;

      case ColorTypeCMYK:
        push(makeOperator_sc(color.mValue.mCMYK.mCyan, color.mValue.mCMYK.mMagenta, color.mValue.mCMYK.mYellow, color.mValue.mCMYK.mBlack));
        break;

      default:
        RUNTIME_EXCEPTION("unsupported color type");
    }
  }

  void ContentStreamImpl::setStrokingColorGray(float gray){
    push(makeOperator_G(gray));
  }

  void ContentStreamImpl::setStrokingColorGray(Color const &color){
    push(makeOperator_G(color.mValue.mGray.mGray));
  }

  void ContentStreamImpl::setStrokingColorIndex(uint32_t index){
    Color const color = Color::createIndex(index);
    setStrokingColor(color);
  }

  void ContentStreamImpl::setStrokingColorRGB(float red, float green, float blue){
    push(makeOperator_RG(red, green, blue));
  }

  void ContentStreamImpl::setStrokingColorRGB(Color const &color){
    setStrokingColorRGB(color.mValue.mRGB.mRed, color.mValue.mRGB.mGreen, color.mValue.mRGB.mBlue);
  }

  void ContentStreamImpl::setStrokingColorRGB(ColorName colorName){
    setStrokingColorRGB(Color::createRGB(colorName));
  }

  void ContentStreamImpl::setStrokingColorCMYK(float cyan, float magenta, float yellow, float black){
    push(makeOperator_K(cyan, magenta, yellow, black));
  }

  void ContentStreamImpl::setStrokingColorCMYK(Color const &color){
    setStrokingColorCMYK(color.mValue.mCMYK.mCyan, color.mValue.mCMYK.mMagenta, color.mValue.mCMYK.mYellow, color.mValue.mCMYK.mBlack);
  }

  void ContentStreamImpl::setStrokingColorCMYK(ColorName colorName){
    setStrokingColorCMYK(Color::createCMYK(colorName));
  }

  void ContentStreamImpl::setStrokingColor(Color const & color){
    mCurrentGraphicsState->mStrokingColor = color;
    switch(color.mType){
      case ColorTypeGray:
        push(makeOperator_SC(color.mValue.mGray.mGray));
        break;

      case ColorTypeIndex:
        push(makeOperator_SC(color.mValue.mIndex.mIndex));
        break;

      case ColorTypeRGB:
        push(makeOperator_SC(color.mValue.mRGB.mRed, color.mValue.mRGB.mGreen, color.mValue.mRGB.mBlue));
        break;

      case ColorTypeCMYK:
        push(makeOperator_SC(color.mValue.mCMYK.mCyan, color.mValue.mCMYK.mMagenta, color.mValue.mCMYK.mYellow, color.mValue.mCMYK.mBlack));
        break;

      default:
        RUNTIME_EXCEPTION("unsupported color type");
    }
  }

  void ContentStreamImpl::beginPath(float x, float y){
    push(makeOperator_m(x, y));
  }

  void ContentStreamImpl::closePath(PathPaintingOp paintingOp){
    switch (paintingOp){
      case PathPaintingOpNope:
        push(makeOperator_n());
        break;

      case PathPaintingOpStroke:
        push(makeOperator_S());
        break;

      case PathPaintingOpCloseAndStroke:
        push(makeOperator_s());
        break;

      case PathPaintingOpFillNW:
        push(makeOperator_f());
        break;

      case PathPaintingOpFillEO:
        push(makeOperator_f_ast());
        break;

      case PathPaintingOpFillAndStrokeNW:
        push(makeOperator_B());
        break;

      case PathPaintingOpFillAndStrokeEO:
        push(makeOperator_B_ast());
        break;

      case PathPaintingOpCloseFillStrokeNW:
        push(makeOperator_b());
        break;

      case PathPaintingOpCloseFillStrokeEO:
        push(makeOperator_b_ast());
        break;

      case PathPaintingOpClippingNW:{
        push(makeOperator_W());
        push(makeOperator_n());
        break;
      }

      case PathPaintingOpClippingEO:
        push(makeOperator_W_ast());
        push(makeOperator_n());
        break;
    }
  }

  void ContentStreamImpl::closeSubPath(void){
    push(makeOperator_h());
  }

  void ContentStreamImpl::addLine(float x, float y){
    push(makeOperator_l(x, y));
  }

  void ContentStreamImpl::addRectangle(float x, float y, float width, float height){
    push(makeOperator_re(x, y, width, height));
  }

  void ContentStreamImpl::addRectangle(struct Rectangle const &rect){
    push(makeOperator_re(rect.mLowerLeftX, rect.mLowerLeftY, rect.mUpperRightX - rect.mLowerLeftX, rect.mUpperRightY - rect.mLowerLeftY));
  }

  void ContentStreamImpl::addEllipse(struct Rectangle const &rect){
    Point pts[2];
    addArc(rect, 0.f, 2.f * Pi, pts[0], pts[1]);
    closeSubPath();
  }

  void ContentStreamImpl::addArc(struct Rectangle const &rect, float angFrom, float angTo, Point &start, Point &end){
    float const a = getWidth(rect) / 2.f;
    float const b = getHeight(rect) / 2.f;

    float const x0 = (rect.mLowerLeftX + rect.mUpperRightX) / 2.f;
    float const y0 = (rect.mLowerLeftY + rect.mUpperRightY) / 2.f;

    // Calculates two middle points (p1, p2) for Bezier curve, which should pass throw points a = p0, b, c, d = p3.
    auto getP1 = [](float a, float b, float c, float d)->float {return (-5.f * a + 18.f * b - 9.f * c + 2.f * d) / 6.f;};
    auto getP2 = [](float a, float b, float c, float d)->float {return (2.f * a - 9.f * b + 18.f * c - 5.f * d) / 6.f;};

    int const curves = std::roundl((std::fabs(angTo - angFrom) / (Pi / 2.f) + 0.5f));
    float const dAngle = (angTo - angFrom) / (curves * 3);

    std::size_t const count = curves * 3;

    std::vector<float> x(count + 1);
    std::vector<float> y(count + 1);

    start = {x0 + a * std::cos(angFrom), y0 + b * std::sin(angFrom)};
    end = {x0 + a * std::cos(angTo), y0 + b * std::sin(angTo)};

    x[0] = start.mX;
    y[0] = start.mY;

    for (std::size_t i = 1; i < count; ++i){
      x[i] = x0 + a * std::cos(angFrom + i * dAngle);
      y[i] = y0 + b * std::sin(angFrom + i * dAngle);
    }

    x[count] = end.mX;
    y[count] = end.mY;

    beginPath(start.mX, start.mY);
    for (std::size_t i = 0; i < curves; ++i){
      Point const point1 = {getP1(x[i * 3], x[i * 3 + 1], x[i * 3 + 2], x[i * 3 + 3]), getP1(y[i * 3], y[i * 3 + 1], y[i * 3 + 2], y[i * 3 + 3])};
      Point const point2 = {getP2(x[i * 3], x[i * 3 + 1], x[i * 3 + 2], x[i * 3 + 3]), getP2(y[i * 3], y[i * 3 + 1], y[i * 3 + 2], y[i * 3 + 3])};
      Point const point3 = {x[i * 3 + 3], y[i * 3 + 3]};
      addBezierCurve12(point1.mX, point1.mY, point2.mX, point2.mY, point3.mX, point3.mY);
    }
  }

  void ContentStreamImpl::addBezierCurve12(float x1, float y1, float x2, float y2, float x3, float y3){
    push(makeOperator_c(x1, y1, x2, y2, x3, y3));
  }

  void ContentStreamImpl::addBezierCurve02(float x2, float y2, float x3, float y3){
    push(makeOperator_v(x2, y2, x3, y3));
  }

  void ContentStreamImpl::addBezierCurve13(float x1, float y1, float x3, float y3){
    push(makeOperator_y(x1, y1, x3, y3));
  }

  void ContentStreamImpl::showXObject(char const *name){
    push(makeOperator_Do(name));
  }

}}
