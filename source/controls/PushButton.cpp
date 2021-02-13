#ifdef __NOT_IMPLEMENTED___

#include <controls/PushButton.h>

#include <array>

#include <stream/OutputStream.h>
#include <object/ObjectIndirectReference.h>
#include <object/ObjectArray.h>
#include <object/ObjectRectangle.h>
#include <object/ObjectUTF16BETextString.h>
#include <object/ObjectInteger.h>

#include <interactive/acroform/AcroFormButtonDictionary.h>
#include <interactive/annotation/WidgetAnnotationDictionary.h>
#include <interactive/annotation/AppearanceDictionary.h>
#include <interactive/annotation/WidgetAnnotationMKDictionary.h>
#include <FormXObject.h>
#include <UString.h>

using namespace kernel;
using namespace kernel::interactive;

PushButton::PushButton(kernel::ResourceManager *resourceManager, struct Rectangle const &rect)
  : Control(resourceManager, rect),
  mMkDictionary(std::make_unique<WidgetAnnotationMKDictionary>()){
}

PushButton::~PushButton(void) {}

std::unique_ptr<kernel::ObjectIndirectReference> PushButton::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{

  std::unique_ptr<WidgetAnnotationDictionary> widget = std::make_unique<WidgetAnnotationDictionary>();
  std::unique_ptr<ObjectIndirectReference> ref = widget->getReference();

  widget->setReference(mWidgetRef.get());
  widget->addKey(AcroFormFieldDictionaryKeyFf, std::make_unique<ObjectInteger>(AcroFormButtonFlagsPushbutton));
  widget->addKey(AnnotationDictionaryKeyRect, std::make_unique<ObjectRectangle>(mRect));
  //widget->addKey(AnnotationDictionaryKeyF, std::make_unique<ObjectInteger>(AnnotationFlagPrint));
  widget->addKey(AcroFormFieldDictionaryKeyT, std::make_unique<ObjectUTF16BETextString>(UString(L"push button", Encoding_WCHAR_T)));
  widget->addKey(WidgetAnnotationDictionaryKeyH, std::make_unique<ObjectName>("P"));

  // MK dictionary
  widget->addKey(WidgetAnnotationDictionaryKeyMK, std::move(mMkDictionary->getReference()));

  // AP dictionary
  std::unique_ptr<AppearanceDictionary> AP = std::make_unique<AppearanceDictionary>();

  if (!mNormalAppearance.empty())
    AP->addKey(AppearanceDictionaryKeyN, std::move(mNormalAppearance.begin()->second->getReference()));

  if (!mRolloverAppearance.empty())
    AP->addKey(AppearanceDictionaryKeyR, std::move(mRolloverAppearance.begin()->second->getReference()));

  if (!mDownAppearance.empty())
    AP->addKey(AppearanceDictionaryKeyD, std::move(mDownAppearance.begin()->second->getReference()));

  if (!AP->isEmpty()){
    widget->addKey(AnnotationDictionaryKeyAP, std::move(AP));
    //widget->addKey(AnnotationDictionaryKeyAS, std::make_unique<ObjectName>(mDefaultApperance.empty() ? "Yes" : mDefaultApperance));
  }

  // Kids array
  std::unique_ptr<ObjectArray> kids = std::make_unique<ObjectArray>(IndirectTypeNonIndirectable);
  kids->insert(widget->getReference());

  widget->serialize(stream, params);
  mMkDictionary->serialize(stream, params);

  for (auto const &elem: mFormXObjects)
    elem->serialize(stream, params);

  return ref;
}

void PushButton::setBorderColor(Color const &color){
  switch (color.mType){
    case ColorTypeGray:
    case ColorTypeRGB:
    case ColorTypeCMYK:{
      mMkDictionary->addKey(WidgetAnnotationMKDictionaryKeyBC, std::make_unique<ObjectArray>(colorToArray(color), IndirectTypeNonIndirectable));
      break;
    }
  }
}

void PushButton::setBkColor(Color const &color){
  switch (color.mType){
    case ColorTypeGray:
    case ColorTypeRGB:
    case ColorTypeCMYK:{
      mMkDictionary->addKey(WidgetAnnotationMKDictionaryKeyBG, std::make_unique<ObjectArray>(colorToArray(color), IndirectTypeNonIndirectable));
      break;
    }
  }
}

void PushButton::setNCaption(pdfout::UString const &caption){
  mMkDictionary->addKey(WidgetAnnotationMKDictionaryKeyCA, std::make_unique<ObjectUTF16BETextString>(caption));
}

void PushButton::setRCaption(pdfout::UString const &caption){
  mMkDictionary->addKey(WidgetAnnotationMKDictionaryKeyRC, std::make_unique<ObjectUTF16BETextString>(caption));
}

void PushButton::setDCaption(pdfout::UString const &caption){
  mMkDictionary->addKey(WidgetAnnotationMKDictionaryKeyAC, std::make_unique<ObjectUTF16BETextString>(caption));
}

#endif
