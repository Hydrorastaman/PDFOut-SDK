#pragma once

#include <string>

#include <font/CMapName.h>
#include <object/ObjectStream.h>


namespace kernel{ namespace font{

  enum CMapName;

  enum CMapStreamKey{
    CMapStreamKeyCMapName,
    CMapStreamKeyCIDSystemInfo,
    CMapStreamKeyWMode,
    CMapStreamKeyUseCMap
  };

  class CMapStream : public ObjectStream{
  public:
    CMapStream(std::string const &registry, std::string const &ordering, int supplement, std::string const &cmapName);
    ~CMapStream(void) {}

    using ObjectStream::addKey;
    void addKey(CMapStreamKey key, std::unique_ptr<Object> value);

    void addCodespaceRange(uint32_t charCodeBegin, uint32_t charCodeEnd, uint32_t CIDBegin);
    void addNotdefRange(uint32_t charCodeBegin, uint32_t charCodeEnd, uint32_t CIDBegin);
    void addCIDRange(uint32_t from, uint32_t to);

  protected:
    void beforeSerialize(void) const;

  private:
    static std::unordered_map<CMapStreamKey, std::pair<std::string, uint32_t>> mCMapStreamMap;

  private:
    CMapStream(CMapStream const &) = delete;
    CMapStream &operator=(CMapStream const &) = delete;
  };
}}
