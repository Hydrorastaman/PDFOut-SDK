#pragma once

#include <object/ObjectDictionary.h>
#include <font/CMapName.h>

namespace kernel{ namespace font{

  enum CIDSystemInfoDictionaryKey{
    CIDSystemInfoDictionaryKeyRegistry,  // Required
    CIDSystemInfoDictionaryKeyOrdering,  // Required
    CIDSystemInfoDictionaryKeySupplement // Required
  };

  class CIDSystemInfoDictionary : public ObjectDictionary{
  public:
    explicit CIDSystemInfoDictionary(CMapName cmap = CMapName_Identity_H);
    ~CIDSystemInfoDictionary(void) {}

    void addKey(CIDSystemInfoDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<CIDSystemInfoDictionaryKey, std::pair<std::string, uint32_t>> mCIDSystemInfoDictionaryMap;

  private:
    CIDSystemInfoDictionary(CIDSystemInfoDictionary const &) = delete;
    CIDSystemInfoDictionary &operator=(CIDSystemInfoDictionary const &) = delete;
  };
}}
