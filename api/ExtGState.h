#pragma once

#include "Types.h"
#include "PdfOutConfig.h"

namespace pdfout{

  class PDFOUT_EXPORTS ExtGState{
  protected:
    ExtGState(void) {}
    virtual ~ExtGState(void) {}

  public:
    static ExtGState *createExtGState(void);

    virtual void setLineWidth(float lineWidth) = 0;
    virtual float getLineWidth(void) const = 0;

    virtual void setLineCap(LineCap lineCap) = 0;
    virtual LineCap getLineCap(void) const = 0;

    virtual void setLineJoin(LineJoin lineJoin) = 0;
    virtual LineJoin getLineJoin(void) const = 0;

    virtual void setMiterLimit(float miterLimit) = 0;
    virtual float getMiterLimit(void) const = 0;

    virtual void setRenderingIntent(RenderingIntent renderingIntent) = 0;
    virtual RenderingIntent getRenderingIntent(void) const = 0;

    virtual void setStrokingOverprint(bool strokingOverprint) = 0;
    virtual bool getStrokingOverprint(void) const = 0;

    virtual void setOverprint(bool overprint) = 0;
    virtual bool getOverprint(void) const = 0;

    virtual void setOverprintMode(OverprintMode mode) = 0;
    virtual OverprintMode getOverprintMode(void) const = 0;

    virtual void setFlatness(float flatness) = 0;
    virtual float getFlatness(void) const = 0;

    virtual void setSmoothness(float smoothness) = 0;
    virtual float getSmoothness(void) const = 0;

    virtual void setStrokeAdjustment(bool strokeAdjustment) = 0;
    virtual bool getStrokeAdjustment(void) const = 0;

    virtual void setBlendMode(BlendMode blendMode) = 0;
    virtual BlendMode getBlendMode(void) const = 0;

  private:
    ExtGState(ExtGState const &);
    ExtGState &operator=(ExtGState const &);
  };
}
