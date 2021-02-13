#pragma once

#include <map>
#include <memory>
#include <cstdint>

#include <Types.h>
#include <font/TrueType/Table.h>
#include <font/TrueType/Cmap.h>

namespace pdfout{

  class InputStream;
  class OutputStream;

}

namespace kernel{ namespace font{ namespace truetype{
  struct CmapInfo{
    uint16_t mPlatform; /**< Platform ID */
    uint16_t mEncoding; /**< Platform-specific encoding ID */
    uint16_t mLanguage; /**< Language ID */

    bool operator<(CmapInfo const &rhs) const{
      return (mPlatform < rhs.mPlatform) ||
             (mPlatform == rhs.mPlatform && mEncoding < rhs.mEncoding) ||
             (mPlatform == rhs.mPlatform && mEncoding == rhs.mEncoding && mLanguage < rhs.mLanguage);
    }
  };

  class TableCmap : public Table{
  public:
    TableCmap(void);
    TableCmap(TableCmap const &obj);
    ~TableCmap(void);

    void load(pdfout::InputStream *stream);
    void save(pdfout::OutputStream *stream) const;

    std::unique_ptr<TableCmap> createSubset(std::vector<uint32_t> const &cids) const;
    std::unique_ptr<TableCmap> createSuperset(TableCmap const *cmap) const;

    bool isExists(pdfout::PlatformId platfromId, uint16_t encodingId) const;
    Cmap *getCmap(pdfout::PlatformId platfromId, uint16_t encodingId) const;
    Cmap *getUnicodeCmap(void) const;

  private:
    TableCmap &operator=(TableCmap const &) = delete;

  private:
    std::map<CmapInfo, std::unique_ptr<Cmap>> mTables;
  };

}}}
