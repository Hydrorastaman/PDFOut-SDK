#ifdef __NOT_IMPLEMENTED___

#include <controls/Control.h>
#include <ResourceUid.h>
#include <FormXObject.h>

#include <object/ObjectIndirectReference.h>

using namespace kernel;
using namespace kernel::interactive;

Control::Control(kernel::ResourceManager *resourceManager, struct Rectangle const &rect)
  : mUID(ResourceUid::getResourceUid(ResourceTypeWidget)),
  mWidgetRef(Indirectable::generateReference()),
  mRect(rect),
  mResourceManager(resourceManager){
}

Control::~Control(void) {}

std::string const &Control::getUID(void) const{
  return mUID;
}

std::unique_ptr<kernel::ObjectIndirectReference> Control::getReference(void) const{
  return mWidgetRef->getReference();
}

struct Rectangle Control::getRect(void) const{
  return mRect;
}

FormXObject *Control::createAppearance(void){
  std::unique_ptr<FormXObject> appearance = std::make_unique<FormXObject>(mResourceManager, mRect);
  FormXObject *rawPtr = appearance.get();
  mFormXObjects.push_back(std::move(appearance));
  return rawPtr;
}

void Control::setNormalAppearance(FormXObject *xObj, std::string const &name){
  mNormalAppearance[name] = xObj;
}

void Control::setRolloverAppearance(FormXObject *xObj, std::string const &name){
  mRolloverAppearance[name] = xObj;
}

void Control::setDownAppearance(FormXObject *xObj, std::string const &name){
  mDownAppearance[name] = xObj;
}

void Control::setDefaultApperance(std::string const &name){
  mDefaultApperance = name;
}

#endif
