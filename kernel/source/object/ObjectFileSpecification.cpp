#include <object/ObjectFileSpecification.h>

namespace{

  std::string const keyType = "Type";
  std::string const valueType = "Filespec";

  std::string const keyFS = "FS";
  std::string const keyF = "F";
  std::string const keyUF = "UF";
  std::string const keyDOS = "DOS";
  std::string const keyMac = "Mac";
  std::string const keyUnix = "Unix";
  std::string const keyID = "ID";
  std::string const keyV = "V";
  std::string const keyEF = "EF";
  std::string const keyRF = "RF";
  std::string const keyDesc = "Desc";
  std::string const keyCI = "CI";
}

namespace kernel{

  std::unordered_map<ObjectFileSpecificationKey, std::pair<std::string, uint32_t>> ObjectFileSpecification::mObjectFileSpecificationMap{
    {ObjectFileSpecificationKeyFS,   {keyFS,   ObjectTypeName}},
    {ObjectFileSpecificationKeyF,    {keyF,    OBJECT_TYPE_STRING}},
    {ObjectFileSpecificationKeyUF,   {keyUF,   OBJECT_TYPE_TEXT_STRING}},
    {ObjectFileSpecificationKeyDOS,  {keyDOS,  ObjectTypeByteString}},
    {ObjectFileSpecificationKeyMac,  {keyMac,  ObjectTypeByteString}},
    {ObjectFileSpecificationKeyUnix, {keyUnix, ObjectTypeByteString}},
    {ObjectFileSpecificationKeyID,   {keyID,   ObjectTypeArray}},
    {ObjectFileSpecificationKeyV,    {keyV,    ObjectTypeBoolean}},
    {ObjectFileSpecificationKeyEF,   {keyEF,   ObjectTypeDictionary}},
    {ObjectFileSpecificationKeyRF,   {keyRF,   ObjectTypeDictionary}},
    {ObjectFileSpecificationKeyDesc, {keyDesc, OBJECT_TYPE_TEXT_STRING}},
    {ObjectFileSpecificationKeyCI,   {keyCI,   ObjectTypeDictionary}}
  };

  ObjectFileSpecification::ObjectFileSpecification(void)
    : Object(ObjectTypeFileSpecification){

    mFileDictionary->insert(keyType, std::make_unique<ObjectName>(valueType));
  }

  ObjectFileSpecification::~ObjectFileSpecification(void) {}

  void ObjectFileSpecification::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
    mFileDictionary->serialize(stream, params);
  }

  void ObjectFileSpecification::addKey(ObjectFileSpecificationKey key, std::unique_ptr<Object> value){
    mFileDictionary->addKey(mObjectFileSpecificationMap, key, std::move(value));
  }

}
