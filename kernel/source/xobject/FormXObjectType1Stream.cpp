#include <xobject/FormXObjectType1Stream.h>

#include <ResourceManager.h>
#include <object/ObjectName.h>
#include <object/ObjectInteger.h>

namespace{

  std::string const valueSubtype = "Form";

  std::string const keyFormType = "FormType";
  int const valueFormType = 1;

  std::string const keyBBox = "BBox";
  std::string const keyMatrix = "Matrix";
  std::string const keyResources = "Resources";
  std::string const keyGroup = "Group";
  std::string const keyRef = "Ref";
  std::string const keyMetadata = "Metadata";
  std::string const keyPieceInfo = "PieceInfo";
  std::string const keyLastModified = "LastModified";
  std::string const keyStructParent = "StructParent";
  std::string const keyStructParents = "StructParents";
  std::string const keyOPI = "OPI";
  std::string const keyOC = "OC";

}

namespace kernel{ namespace xobject{

  std::unordered_map<FormXObjectType1DictionaryKey, std::pair<std::string, uint32_t>> FormXObjectType1Stream::mFormXObjectType1StreamMap{
    {FormXObjectType1DictionaryKeyBBox,          {keyBBox,          ObjectTypeRectangle}},
    {FormXObjectType1DictionaryKeyMatrix,        {keyMatrix,        ObjectTypeArray}},
    {FormXObjectType1DictionaryKeyResources,     {keyResources,     ObjectTypeDictionary}},
    {FormXObjectType1DictionaryKeyGroup,         {keyGroup,         ObjectTypeDictionary}},
    {FormXObjectType1DictionaryKeyRef,           {keyRef,           ObjectTypeDictionary}},
    {FormXObjectType1DictionaryKeyMetadata,      {keyMetadata,      ObjectTypeStream}},
    {FormXObjectType1DictionaryKeyPieceInfo,     {keyPieceInfo,     ObjectTypeDictionary}},
    {FormXObjectType1DictionaryKeyLastModified,  {keyLastModified,  ObjectTypeDate}},
    {FormXObjectType1DictionaryKeyStructParent,  {keyStructParent,  ObjectTypeInteger}},
    {FormXObjectType1DictionaryKeyStructParents, {keyStructParents, ObjectTypeInteger}},
    {FormXObjectType1DictionaryKeyOPI,           {keyOPI,           ObjectTypeDictionary}},
    {FormXObjectType1DictionaryKeyOC,            {keyOC,            ObjectTypeDictionary}}
  };

  FormXObjectType1Stream::FormXObjectType1Stream(void)
    : FormXObjectStream(valueSubtype){
    ObjectStream::insert(keyFormType, std::make_unique<ObjectInteger>(valueFormType));
  }

  FormXObjectType1Stream::~FormXObjectType1Stream(void) {}

  void FormXObjectType1Stream::addKey(FormXObjectType1DictionaryKey key, std::unique_ptr<Object> value){
    ObjectStream::addKey(mFormXObjectType1StreamMap, key, std::move(value));
  }

}}
