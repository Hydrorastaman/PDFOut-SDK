#include <ExtGStateImpl.h>
#include <ResourceUid.h>
#include <UserObjectManager.h>

#include <ToString.h>
#include <object/ObjectNumber.h>
#include <object/ObjectBoolean.h>
#include <object/ObjectInteger.h>
#include <object/ObjectIndirectReference.h>
#include <graphics/GraphicsStateParameterDictionary.h>

using namespace pdfout;
using namespace kernel;

namespace{

  std::unordered_map<std::string, RenderingIntent> strToRi{
    {"AbsoluteColorimetric", RenderingIntentAbsoluteColorimetric},
    {"RelativeColorimetric", RenderingIntentRelativeColorimetric},
    {"Saturation",           RenderingIntentSaturation},
    {"Perceptual",           RenderingIntentPerceptual}
  };

  std::unordered_map<std::string, BlendMode> strToBm{
    {"Normal", BlendModeNormal},
    {"Multiply", BlendModeMultiply},
    {"Screen", BlendModeScreen},
    {"Overlay", BlendModeOverlay},
    {"Darken", BlendModeDarken},
    {"Lighten", BlendModeLighten},
    {"ColorDodge", BlendModeColorDodge},
    {"ColorBurn", BlendModeColorBurn},
    {"HardLight", BlendModeHardLight},
    {"SoftLight", BlendModeSoftLight},
    {"Difference", BlendModeDifference},
    {"Exclusion", BlendModeExclusion},
    {"Hue", BlendModeHue},
    {"Saturation", BlendModeSaturation},
    {"Color", BlendModeColor},
    {"Luminosity", BlendModeLuminosity}
  };

}

namespace pdfout{ namespace impl{

  ExtGStateImpl::ExtGStateImpl(void)
    : ExtGState(),
    mUID(ResourceUid::getResourceUid(ResourceTypeGS)),
    mExtGState(std::make_unique<GraphicsStateParameterDictionary>()){
  }

  ExtGStateImpl::~ExtGStateImpl(void) {}

  std::unique_ptr<kernel::ObjectIndirectReference> ExtGStateImpl::serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const{
    mExtGState->serialize(stream, params);
    return mExtGState->getReference();
  }

  std::string const &ExtGStateImpl::getUID(void) const{
    return mUID;
  }

  void ExtGStateImpl::setLineWidth(float lineWidth){
    mExtGState->addKey(GraphicsStateParameterDictionaryKeyLW, std::make_unique<ObjectNumber>(lineWidth));
  }

  float ExtGStateImpl::getLineWidth(void) const{
    return getNumber(GraphicsStateParameterDictionaryKeyLW, 1.f);
  }

  void ExtGStateImpl::setLineCap(LineCap lineCap){
    mExtGState->addKey(GraphicsStateParameterDictionaryKeyLC, std::make_unique<ObjectInteger>(lineCap));
  }

  LineCap ExtGStateImpl::getLineCap(void) const{
    return (LineCap) getInteger(GraphicsStateParameterDictionaryKeyLC, LineCapButt);
  }

  void ExtGStateImpl::setLineJoin(LineJoin lineJoin){
    mExtGState->addKey(GraphicsStateParameterDictionaryKeyLJ, std::make_unique<ObjectInteger>(lineJoin));
  }

  LineJoin ExtGStateImpl::getLineJoin(void) const{
    return (LineJoin) getInteger(GraphicsStateParameterDictionaryKeyLJ, LineJoinMiter);
  }

  void ExtGStateImpl::setMiterLimit(float miterLimit){
    mExtGState->addKey(GraphicsStateParameterDictionaryKeyML, std::make_unique<ObjectNumber>(miterLimit));
  }

  float ExtGStateImpl::getMiterLimit(void) const{
    return getNumber(GraphicsStateParameterDictionaryKeyML, 10.f);
  }

  void ExtGStateImpl::setRenderingIntent(RenderingIntent renderingIntent){
    mExtGState->addKey(GraphicsStateParameterDictionaryKeyRI, std::make_unique<ObjectName>(toString(renderingIntent)));
  }

  RenderingIntent ExtGStateImpl::getRenderingIntent(void) const{
    std::string const ri = getName(GraphicsStateParameterDictionaryKeyRI, toString(RenderingIntentRelativeColorimetric));
    auto iter = strToRi.find(ri);
    if (iter == std::end(strToRi))
      RUNTIME_EXCEPTION("Invalid Rendering Intent value");

    return strToRi.find(ri)->second;
  }

  void ExtGStateImpl::setStrokingOverprint(bool strokingOverprint){
    mExtGState->addKey(GraphicsStateParameterDictionaryKeyOP, std::make_unique<ObjectBoolean>(strokingOverprint));
  }

  bool ExtGStateImpl::getStrokingOverprint(void) const{
    return getBoolean(GraphicsStateParameterDictionaryKeyOP, false);
  }

  void ExtGStateImpl::setOverprint(bool overprint){
    mExtGState->addKey(GraphicsStateParameterDictionaryKeyop, std::make_unique<ObjectBoolean>(overprint));
  }

  bool ExtGStateImpl::getOverprint(void) const{
    return getBoolean(GraphicsStateParameterDictionaryKeyop, false);
  }

  void ExtGStateImpl::setOverprintMode(OverprintMode mode){
    mExtGState->addKey(GraphicsStateParameterDictionaryKeyOPM, std::make_unique<ObjectInteger>(mode));
  }

  OverprintMode ExtGStateImpl::getOverprintMode(void) const{
    return (OverprintMode) getInteger(GraphicsStateParameterDictionaryKeyOPM, OverprintModeZero);
  }

  void ExtGStateImpl::setFlatness(float flatness){
    mExtGState->addKey(GraphicsStateParameterDictionaryKeyFL, std::make_unique<ObjectNumber>(flatness));
  }

  float ExtGStateImpl::getFlatness(void) const{
    return getNumber(GraphicsStateParameterDictionaryKeyFL, 1.f);
  }

  void ExtGStateImpl::setSmoothness(float smoothness){
    mExtGState->addKey(GraphicsStateParameterDictionaryKeySM, std::make_unique<ObjectNumber>(smoothness));
  }

  float ExtGStateImpl::getSmoothness(void) const{
    return getNumber(GraphicsStateParameterDictionaryKeySM, 1.f);
  }

  void ExtGStateImpl::setStrokeAdjustment(bool strokeAdjustment){
    mExtGState->addKey(GraphicsStateParameterDictionaryKeySA, std::make_unique<ObjectBoolean>(strokeAdjustment));
  }

  bool ExtGStateImpl::getStrokeAdjustment(void) const{
    return getBoolean(GraphicsStateParameterDictionaryKeySA, false);
  }

  void ExtGStateImpl::setBlendMode(BlendMode blendMode){
    mExtGState->addKey(GraphicsStateParameterDictionaryKeyBM, std::make_unique<ObjectName>(toString(blendMode)));
  }

  BlendMode ExtGStateImpl::getBlendMode(void) const{
    std::string const bm = getName(GraphicsStateParameterDictionaryKeyBM, toString(BlendModeNormal));
    auto iter = strToBm.find(bm);
    if (iter == std::end(strToBm))
      RUNTIME_EXCEPTION("Invalid Blend Mode value");

    return strToBm.find(bm)->second;
  }

  int32_t ExtGStateImpl::getInteger(kernel::GraphicsStateParameterDictionaryKey key, int32_t default) const{
    Object const *obj = mExtGState->findKey(key);
    if (obj->getType() == ObjectTypeInteger)
      return ((ObjectInteger const *) obj)->getValue();

    return default;
  }

  float ExtGStateImpl::getNumber(kernel::GraphicsStateParameterDictionaryKey key, float default) const{
    Object const *obj = mExtGState->findKey(key);
    if (obj->getType() == ObjectTypeNumber)
      return ((ObjectNumber const *) obj)->getValue();

    return default;
  }

  std::string ExtGStateImpl::getName(kernel::GraphicsStateParameterDictionaryKey key, std::string const &default) const{
    Object const *obj = mExtGState->findKey(key);
    if (obj->getType() == ObjectTypeName)
      return ((ObjectName const *) obj)->getValue();

    return default;
  }

  bool ExtGStateImpl::getBoolean(kernel::GraphicsStateParameterDictionaryKey key, bool default) const{
    Object const *obj = mExtGState->findKey(key);
    if (obj->getType() == ObjectTypeBoolean)
      return ((ObjectBoolean const *) obj)->getValue();

    return default;
  }

}}
