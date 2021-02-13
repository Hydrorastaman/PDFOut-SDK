#include <font/CIDSystemInfoDictionary.h>

#include <string>
#include <algorithm>

#include <object/ObjectASCIIString.h>
#include <object/ObjectInteger.h>

namespace{
  std::string const keyRegistry = "Registry";
  std::string const keyOrdering = "Ordering";
  std::string const keySupplement = "Supplement";

}

namespace kernel{ namespace font{

  std::unordered_map<CIDSystemInfoDictionaryKey, std::pair<std::string, uint32_t>> CIDSystemInfoDictionary::mCIDSystemInfoDictionaryMap{
    {CIDSystemInfoDictionaryKeyRegistry,   {keyRegistry,   ObjectTypeASCIIString}},
    {CIDSystemInfoDictionaryKeyOrdering,   {keyOrdering,   ObjectTypeASCIIString}},
    {CIDSystemInfoDictionaryKeySupplement, {keySupplement, ObjectTypeInteger}}
  };

  CIDSystemInfoDictionary::CIDSystemInfoDictionary(CMapName cmap)
    : ObjectDictionary(IndirectTypeIndirectable){

    CMapNameInfo cmapInfo = getCMapNameInfo(cmap);
    if (cmapInfo.mCmapName != CMapName_UNKNOWN){
      ObjectDictionary::insert(keyRegistry, std::make_unique<ObjectASCIIString>(cmapInfo.mRegistry));
      ObjectDictionary::insert(keyOrdering, std::make_unique<ObjectASCIIString>(cmapInfo.mOrdering));
      ObjectDictionary::insert(keySupplement, std::make_unique<ObjectInteger>(cmapInfo.mSupplement));
    }
  }

  void CIDSystemInfoDictionary::addKey(CIDSystemInfoDictionaryKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mCIDSystemInfoDictionaryMap, key, std::move(value));
  }
}}
