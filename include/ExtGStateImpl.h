#pragma once

#include <memory>
#include <string>

#include <ExtGState.h>

namespace pdfout{
  class OutputStream;
}

namespace kernel{
  enum GraphicsStateParameterDictionaryKey;
  class GraphicsStateParameterDictionary;
  class ObjectIndirectReference;
  struct SerializeParams;
}

namespace pdfout{ namespace impl{

  class ExtGStateImpl : public pdfout::ExtGState{
  public:
    ExtGStateImpl(void);
    ~ExtGStateImpl(void);

    std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

    std::string const &getUID(void) const;

    void setLineWidth(float lineWidth);
    float getLineWidth(void) const;

    void setLineCap(LineCap lineCap);
    LineCap getLineCap(void) const;

    void setLineJoin(LineJoin lineJoin);
    LineJoin getLineJoin(void) const;

    void setMiterLimit(float miterLimit);
    float getMiterLimit(void) const;

    void setRenderingIntent(RenderingIntent renderingIntent);
    RenderingIntent getRenderingIntent(void) const;

    void setStrokingOverprint(bool strokingOverprint);
    bool getStrokingOverprint(void) const;

    void setOverprint(bool overprint);
    bool getOverprint(void) const;

    void setOverprintMode(OverprintMode mode);
    OverprintMode getOverprintMode(void) const;

    void setFlatness(float flatness);
    float getFlatness(void) const;

    void setSmoothness(float smoothness);
    float getSmoothness(void) const;

    void setStrokeAdjustment(bool strokeAdjustment);
    bool getStrokeAdjustment(void) const;

    void setBlendMode(BlendMode blendMode);
    BlendMode getBlendMode(void) const;

  private:
    int32_t getInteger(kernel::GraphicsStateParameterDictionaryKey key, int32_t default) const;
    float getNumber(kernel::GraphicsStateParameterDictionaryKey key, float default) const;
    std::string getName(kernel::GraphicsStateParameterDictionaryKey key, std::string const &default) const;
    bool getBoolean(kernel::GraphicsStateParameterDictionaryKey key, bool default) const;

  private:
    ExtGStateImpl(ExtGStateImpl const &) = delete;
    ExtGStateImpl &operator=(ExtGStateImpl const &) = delete;

  private:
    std::string mUID;
    std::unique_ptr<kernel::GraphicsStateParameterDictionary> mExtGState;
    //float mLineWidth; // LW
    //LineCap mLineCap; // LC
    //LineJoin mLineJoin; // LJ
    //float mMiterLimit; // ML
    // D
    //RenderingIntent mRenderingIntent; // RI
    //bool mStrokingOverprint; // OP
    //bool mOverprint; // op
    //OverprintMode mOverprintMode; // OPM
    // Font
    // BG
    // BG2
    // UCR
    // UCR2
    // TR
    // TR2
    // HT
    // FL
    // SM
    // SA
    // BM
    // SMask
    // CA
    // ca
    // AIS
    // TK
  };

}}
