#include <font/TrueType/TableCmap.h>

#include <memory>
#include <cstdint>
#include <algorithm>

#include <InternalTools.h>
#include <font/TrueType/CmapFormat0.h>
#include <font/TrueType/CmapFormat2.h>
#include <font/TrueType/CmapFormat4.h>
#include <font/TrueType/CmapFormat6.h>
#include <font/TrueType/CmapFormat12.h>

namespace{

  PACK(struct CmapHeader{
    uint16_t mVersion;   /**< Version number (Set to zero) */
    uint16_t mSubtables; /**< Number of encoding subtables */
  });

  PACK(struct CmapEncodingSubtable{
    uint16_t mPlatform; /**< Platform identifier */
    uint16_t mEncoding; /**< Platform-specific encoding identifier */
    uint32_t mOffset;   /**< Byte offset from beginning of table to the subtable for this encoding */
  });
}

using namespace pdfout;

namespace kernel{ namespace font{ namespace truetype{

  TableCmap::TableCmap(void)
    : Table(){
  }

  TableCmap::TableCmap(TableCmap const &obj)
    : Table(obj){

    for (auto const &elem : obj.mTables)
      mTables[elem.first] = elem.second->clone();
  }

  TableCmap::~TableCmap(void) {}

  void TableCmap::save(pdfout::OutputStream *stream) const{
    Endian const endian = getEndian();
    uint64_t const startOfTable = stream->tellp();

    CmapHeader header;
    header.mVersion = 0;
    header.mSubtables = mTables.size();
    if (endian == EndianLittle){
      swapByteOrder(header.mVersion);
      swapByteOrder(header.mSubtables);
    }
    stream->write(&header, sizeof(CmapHeader), 1);

    std::vector<CmapEncodingSubtable> subtables;
    subtables.reserve(mTables.size());

    stream->seekp(startOfTable + sizeof(CmapHeader) + mTables.size() * sizeof(CmapEncodingSubtable));

    CmapEncodingSubtable subtable;
    for (auto const &elem : mTables){
      subtable.mPlatform = elem.first.mPlatform;
      subtable.mEncoding = elem.first.mEncoding;
      subtable.mOffset = stream->tellp() - startOfTable;
      elem.second->save(stream);
      subtables.push_back(subtable);
    }

    if (endian == EndianLittle){
      for (auto &elem : subtables){
        swapByteOrder(elem.mPlatform);
        swapByteOrder(elem.mEncoding);
        swapByteOrder(elem.mOffset);
      }
    }

    stream->seekp(startOfTable + sizeof(CmapHeader));
    for (auto const &elem : subtables)
      stream->write(&elem, sizeof(CmapEncodingSubtable), 1);
  }

  std::unique_ptr<TableCmap> TableCmap::createSubset(std::vector<uint32_t> const &cids) const{
    std::unique_ptr<TableCmap> subset = std::make_unique<TableCmap>();

    Cmap *cmap = getUnicodeCmap();
    subset->mTables[{cmap->getPlatformId(), cmap->getEncodingId()}] = std::move(cmap->createSubset(cids));

    //for (auto const &elem : mTables)
    //  subset->mTables[elem.first].reset(elem.second->createSubset(cids));

    return subset;
  }

  std::unique_ptr<TableCmap> TableCmap::createSuperset(TableCmap const *cmap) const{
    std::unique_ptr<TableCmap> superset = std::make_unique<TableCmap>(*this);

    if (!cmap)
      return superset;

    for (auto const &elem : cmap->mTables){
      auto iter = superset->mTables.find(elem.first);
      if (iter != std::end(superset->mTables)){
        iter->second = std::move(iter->second->createSuperset(elem.second.get()));
      }
    }

    return superset;
  }

  bool TableCmap::isExists(PlatformId platformId, uint16_t encodingId) const{
    auto iter = std::find_if(std::begin(mTables), std::end(mTables), [platformId, encodingId](std::pair<const CmapInfo, std::unique_ptr<Cmap>> const &value)->bool {return value.first.mPlatform == platformId && value.first.mEncoding == encodingId;});
    if (iter != std::end(mTables))
      return true;

    return false;
  }

  Cmap *TableCmap::getCmap(PlatformId platformId, uint16_t encodingId) const{
    auto iter = std::find_if(std::begin(mTables), std::end(mTables), [platformId, encodingId](std::pair<const CmapInfo, std::unique_ptr<Cmap>> const &value)->bool {return value.first.mPlatform == platformId && value.first.mEncoding == encodingId;});
    if (iter != std::end(mTables))
      return iter->second.get();

    return nullptr;
  }

  Cmap *TableCmap::getUnicodeCmap(void) const{
    static std::vector<std::pair<PlatformId, uint16_t>> const keys = {
      {PlatformIdUnicode, UnicodeEncodingUnicodeNonBmp},
      {PlatformIdUnicode, UnicodeEncodingDefault},
      {PlatformIdUnicode, UnicodeEncodingVersion},
      {PlatformIdUnicode, UnicodeEncodingISO},
      {PlatformIdUnicode, UnicodeEncodingUnicodeBmp},
      {PlatformIdWindows, MicrosoftEncodingUSC2},
      {PlatformIdWindows, MicrosoftEncodingUSC4},
      {PlatformIdWindows, MicrosoftEncodingSymbol}
    };

    for (auto const &elem : keys){
      auto iter = std::find_if(std::begin(mTables), std::end(mTables), [elem](std::pair<const CmapInfo, std::unique_ptr<Cmap>> const &value)->bool {return value.first.mPlatform == elem.first && value.first.mEncoding == elem.second;});
      if (iter != std::end(mTables))
        return iter->second.get();
    }

    return nullptr;
  }

  void TableCmap::load(pdfout::InputStream *stream){
    Endian const endian = getEndian();
    uint64_t const startOfTable = stream->tellg();

    CmapHeader header;
    stream->read(&header, sizeof(CmapHeader), 1);
    if (endian == EndianLittle){
      swapByteOrder(header.mVersion);
      swapByteOrder(header.mSubtables);
    }

    std::vector<CmapEncodingSubtable> subtables(header.mSubtables);
    for (auto i = 0; i < header.mSubtables; ++i){
      stream->read(&subtables[i], sizeof(CmapEncodingSubtable), 1);
      if (endian == EndianLittle){
        swapByteOrder(subtables[i].mPlatform);
        swapByteOrder(subtables[i].mEncoding);
        swapByteOrder(subtables[i].mOffset);
      }
    }

    uint16_t format = 0;
    for (auto const &elem : subtables){
      stream->seekg(startOfTable + elem.mOffset);
      stream->read(&format, sizeof(uint16_t), 1);
      if (endian == EndianLittle)
        swapByteOrder(format);
      stream->seekg(startOfTable + elem.mOffset);

      std::unique_ptr<Cmap> cmap(nullptr);

      switch (format){
        case Format0:
          cmap = std::make_unique<CmapFormat0>(elem.mPlatform, elem.mEncoding);
          break;

        case Format2:
          break;

        case Format4:
          cmap = std::make_unique<CmapFormat4>(elem.mPlatform, elem.mEncoding);
          break;

        case Format6:
          //cmap = std::make_unique<CmapFormat6>(elem.mPlatform, elem.mEncoding);
          break;

        case Format8:
          break;

        case Format10:
          break;

        case Format12:
          //cmap = std::make_unique<CmapFormat12>(elem.mPlatform, elem.mEncoding);
          break;

        case Format13:
          break;

        case Format14:
          break;
      }

      if (cmap){
        cmap->load(stream);
        CmapInfo info = {elem.mPlatform, elem.mEncoding, cmap->getLanguage()};
        mTables.insert(std::make_pair(info, std::move(cmap)));
      }
    }
  }

}}}
