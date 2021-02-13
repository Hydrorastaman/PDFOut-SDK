#pragma once

#include <map>
#include <string>

#include <font/CMapName.h>
#include <font/AdobeGlyphList.h>
#include <object/ObjectStream.h>

namespace kernel{ namespace font{

  class ToUnicodeCMapStream : public ObjectStream{
  public:
    ToUnicodeCMapStream(void);
    ~ToUnicodeCMapStream(void);

    void setRegistry(std::string const &registry);
    void setOrdering(std::string const &ordering);
    void setSupplement(uint16_t supplement);
    void setCMapName(std::string const &cmapName);
    void setCMapInfo(CMapName cmapName);

    void setCMapVersion(uint16_t version);
    void setCMapType(uint16_t type);

    void addBfChar(uint16_t charCode, UnicodeInfo const &uniInfo);
    void addBfChar(uint16_t charCode, std::vector<uint32_t> const &unicode);

  protected:
    void beforeSerialize(void) const;
    void writeToStream(std::string const &str) const;

  private:
    ToUnicodeCMapStream(ToUnicodeCMapStream const &) = delete;
    ToUnicodeCMapStream &operator=(ToUnicodeCMapStream const &) = delete;

  private:
    std::string mRegistry;
    std::string mOrdering;
    std::string mCMapName;
    uint16_t mSupplement;
    uint16_t mVersion;
    uint16_t mType;
    std::map<uint16_t, uint16_t> mCharToUnicode; /**< char code : unicode value*/
    std::map<uint16_t, UnicodeInfo> mCharToUnicodeMulti; /**< char code : unicode value[] */
  };

}}
