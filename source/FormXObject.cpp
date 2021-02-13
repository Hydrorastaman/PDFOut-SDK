#ifdef __NOT_IMPLEMENTED___
#include <FormXObject.h>

#include <stream/OutputStream.h>
#include <document/DocumentCrossReferenceTable.h>
#include <xobject/FormXObjectType1Stream.h>
#include <object/ObjectRectangle.h>
#include <object/ObjectIndirectReference.h>
#include <object/ObjectArray.h>
#include <ResourceManager.h>

#include <UserObjectManager.h>

namespace kernel{
  typedef UserObjectManager<FormXObject> FormXObjectManager;
}

using namespace kernel;
using namespace kernel::xobject;

FormXObject::FormXObject(kernel::ResourceManager *resourceManager, struct Rectangle const & rect)
  : mResourceManager(resourceManager),
  mRect(rect),
  mRef(Indirectable::generateReference()),
  mContentStream(std::make_unique<ContentStream>(resourceManager)),
  mMatrix(){
}

FormXObject::~FormXObject(void) {}

FormXObject *FormXObject::createFormXObject(kernel::ResourceManager *resourceManager, struct Rectangle const & rect){
  std::unique_ptr<FormXObject> formXObj = std::make_unique<FormXObject>(resourceManager, rect);
  return FormXObjectManager::getInstance().insert(std::move(formXObj));
}

void FormXObject::serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const{
  std::unique_ptr<FormXObjectType1Stream> formStream = std::make_unique<FormXObjectType1Stream>();
  formStream->setReference(mRef.get());

  formStream->addKey(FormXObjectType1DictionaryKeyBBox, std::make_unique<ObjectRectangle>(mRect));
  formStream->addKey(FormXObjectType1DictionaryKeyResources, mResourceManager->getReference());

  std::vector<float> const matrix{mMatrix.mA, mMatrix.mB, mMatrix.mC, mMatrix.mD, mMatrix.mE, mMatrix.mF};
  formStream->addKey(FormXObjectType1DictionaryKeyMatrix, std::make_unique<ObjectArray>(matrix, IndirectTypeNonIndirectable));

  mContentStream->serialize(formStream.get());
  formStream->serialize(stream, params);
}

std::unique_ptr<kernel::ObjectIndirectReference> FormXObject::getReference(void) const{
  return mRef->getReference();
}

ContentStream *FormXObject::getContents(void) const{
  return mContentStream.get();
}

void FormXObject::setMatrix(TransformationMatrix const &matrix){
  mMatrix = matrix;
}
#endif
