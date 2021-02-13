#include <graphics/color/ColorspaceIndexed.h>
#include <object/ObjectIndirectReference.h>

#include <ToString.h>
#include <object/ObjectArray.h>
#include <object/ObjectName.h>
#include <object/ObjectByteString.h>

namespace kernel{ namespace graphics{ namespace color{

  ColorSpaceIndexed::ColorSpaceIndexed(ColorSpaceFamily base, int32_t hival, uint8_t const *color, uint32_t components)
    : ColorSpaceImpl(ColorSpaceFamilyIndexed),
    mBase(base),
    mHival(hival),
    mColor(color, color + components * (hival + 1)){
  }

  ColorSpaceIndexed::~ColorSpaceIndexed(void) {}

  std::unique_ptr<kernel::ObjectIndirectReference> ColorSpaceIndexed::serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const{
    std::unique_ptr<ObjectArray> array(std::make_unique<ObjectArray>());

    array->insert(std::make_unique<ObjectName>(toString(ColorSpaceFamilyIndexed)));
    array->insert(std::make_unique<ObjectName>(toString(mBase)));
    array->insert(std::make_unique<ObjectInteger>(mHival));
    array->insert(std::make_unique<ObjectByteString>(mColor.data(), mColor.size()));

    array->serialize(stream, params);
    std::unique_ptr<ObjectIndirectReference> ref = array->getReference();
    return ref;
  }

}}}
