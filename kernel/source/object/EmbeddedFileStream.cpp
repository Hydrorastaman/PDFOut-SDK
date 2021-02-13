#include <object/EmbeddedFileStream.h>

namespace{

  std::string const keyType = "Type";
  std::string const valueType = "EmbeddedFile";

  std::string const keySubtype = "Subtype";

  std::string const keyParams = "Params";
  std::string const keySize = "Size";
  std::string const keyCreationDate = "CreationDate";
  std::string const keyModDate = "ModDate";
  std::string const keyMac = "Mac";
  std::string const keyCheckSum = "CheckSum";

}

namespace kernel{

  std::unordered_map<EmbeddedFileStreamKey, std::pair<std::string, uint32_t>> EmbeddedFileStream::mObjectStreamMap{
    {EmbeddedFileStreamKeyParams,       {keyParams,       ObjectTypeDictionary}},
    {EmbeddedFileStreamKeySize,         {keySize,         ObjectTypeInteger}},
    {EmbeddedFileStreamKeyCreationDate, {keyCreationDate, ObjectTypeDate}},
    {EmbeddedFileStreamKeyModDate,      {keyModDate,      ObjectTypeDate}},
    {EmbeddedFileStreamKeyMac,          {keyMac,          ObjectTypeDictionary}},
    {EmbeddedFileStreamKeyCheckSum,     {keyCheckSum,     OBJECT_TYPE_STRING}}
  };

  EmbeddedFileStream::EmbeddedFileStream(std::string const &subtype)
    : ObjectStream(){

    ObjectStream::insert(keyType, std::make_unique<ObjectName>(valueType));
    ObjectStream::insert(keySubtype, std::make_unique<ObjectName>(subtype));
  }

  EmbeddedFileStream::~EmbeddedFileStream(void) {}

  void EmbeddedFileStream::addKey(EmbeddedFileStreamKey key, std::unique_ptr<Object> value){
    ObjectStream::addKey(mObjectStreamMap, key, std::move(value));
  }
}
