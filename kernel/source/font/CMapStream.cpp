#include <font/CMapStream.h>

#include <font/CMapName.h>
#include <object/ObjectName.h>
#include <object/ObjectInteger.h>
#include <object/ObjectDictionary.h>

namespace{
  std::string const keyType = "Type";
  std::string const valueType = "CMap";

  std::string const keyCMapName = "CMapName";
  std::string const keyCIDSystemInfo = "CIDSystemInfo";
  std::string const keyWMode = "WMode";
  std::string const keyUseCMap = "UseCMap";

  std::string const version = "10.001";
}

namespace kernel{ namespace font{

  std::unordered_map<CMapStreamKey, std::pair<std::string, uint32_t>> CMapStream::mCMapStreamMap{
    {CMapStreamKeyCMapName,      {keyCMapName,      ObjectTypeName}},
    {CMapStreamKeyCIDSystemInfo, {keyCIDSystemInfo, ObjectTypeDictionary}},
    {CMapStreamKeyWMode,         {keyWMode,         ObjectTypeInteger}},
    {CMapStreamKeyUseCMap,       {keyUseCMap,       ObjectTypeName | ObjectTypeStream}}
  };

  CMapStream::CMapStream(std::string const &registry, std::string const &ordering, int supplement, std::string const &cmapName)
    : ObjectStream(){

    ObjectStream::insert(keyType, std::make_unique<ObjectName>(valueType));
      std::string const cmapHeader =    "%!PS-Adobe-3.0 Resource-CMap\r\n"
                                        "%%DocumentNeededResources: ProcSet (CIDInit)\r\n"
                                        "%%IncludeResource: ProcSet (CIDInit)\r\n";
      std::string const beginResource = "%%BeginResource: CMap (" + cmapName + "\r\n";
      std::string const title =         "%%Title: (" + cmapName + " " + registry + " " + ordering + " " + std::to_string(supplement) + ")\r\n";
      std::string const version =       "%%Version: " + version + "\r\n";
      std::string const endComments =   "%%EndComments\r\n";
      std::string const cidInit =       "/CIDInit /ProcSet findresource begin\r\n"
                                        "12 dict begin\r\n"
                                        "begincmap";

      write(cmapHeader.data(), sizeof(uint8_t), cmapHeader.length());
      write(beginResource.data(), sizeof(uint8_t), beginResource.length());
      write(title.data(), sizeof(uint8_t), title.length());
      write(version.data(), sizeof(uint8_t), version.length());
      write(endComments.data(), sizeof(uint8_t), endComments.length());
      write(cidInit.data(), sizeof(uint8_t), cidInit.length());
  }

  void CMapStream::addKey(CMapStreamKey key, std::unique_ptr<Object> value){
    ObjectStream::addKey(mCMapStreamMap, key, std::move(value));
  }

  void CMapStream::beforeSerialize(void) const{
  }

}}
