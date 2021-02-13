#include <font/TrueType/TableKern.h>

#include <vector>

#include <Exception.h>

namespace kernel{ namespace font{ namespace truetype{

  TableKern::TableKern(void)
    : Table(),
    mData(){
  }

  TableKern::~TableKern(void) {
  }

  void TableKern::load(pdfout::InputStream *stream) {
    Endian const endian = getEndian();

    TableKernHeader header = {0};
    stream->read(&header, sizeof(TableKernHeader), 1);
    if (endian == EndianLittle){
      swapByteOrder(header.mVersion);
      swapByteOrder(header.mTables);
    }

    if (header.mVersion != TableKernVersion0)
      RUNTIME_EXCEPTION("Unsupported KERN table version.");

    for (int16_t i = 0; i < header.mTables; ++i){
      TableKernSubHeader subHeader = {0};
      stream->read(&subHeader, sizeof(TableKernSubHeader), 1);
      if (endian == EndianLittle){
        swapByteOrder(subHeader.mVersion);
        swapByteOrder(subHeader.mLength);
        swapByteOrder(subHeader.mCoverage);
      }

      if (subHeader.mVersion != TableKernFormat0){
        stream->seekg(stream->tellg() + subHeader.mLength - sizeof(TableKernSubHeader));
        continue;
      }

      // supports only Format 0
      TableKernSubTable0 table0 = {0};
      stream->read(&table0, sizeof(TableKernSubTable0), 1);
      if (endian == EndianLittle){
        swapByteOrder(table0.mNPairs);
        swapByteOrder(table0.mSearchRange);
        swapByteOrder(table0.mEntrySelector);
        swapByteOrder(table0.mRangeShift);
      }

      std::vector<TableKernDataFormat0> data(table0.mNPairs);
      stream->read(data.data(), sizeof(TableKernDataFormat0), data.size());
      if (endian == EndianLittle){
        for (auto &elem : data){
          swapByteOrder(elem.mLeft);
          swapByteOrder(elem.mRight);
          swapByteOrder(elem.mValue);
        }
      }

      for (auto const &elem : data)
        mData[elem.mLeft][elem.mRight] = elem.mValue;
    }
  }

  void TableKern::save(pdfout::OutputStream *stream) const {
    Endian const endian = getEndian();

    if (mData.empty())
      return;

    TableKernHeader header = {0};
    header.mVersion = TableKernVersion0;
    header.mTables = 1;

    if (endian == EndianLittle){
      swapByteOrder(header.mVersion);
      swapByteOrder(header.mTables);
    }

    stream->write(&header, sizeof(TableKernHeader), 1);

    TableKernSubHeader subHeader = {0};
    subHeader.mVersion = TableKernFormat0;
    subHeader.mCoverage = TableKernCoverageHorizontal;

    std::size_t npairs = 0;
    for (auto const &elem : mData)
      npairs += elem.second.size();

    subHeader.mLength = sizeof(TableKernSubHeader) + npairs * sizeof(TableKernDataFormat0);

    if (endian == EndianLittle){
      swapByteOrder(subHeader.mVersion);
      swapByteOrder(subHeader.mLength);
      swapByteOrder(subHeader.mCoverage);
    }

    stream->write(&subHeader, sizeof(TableKernSubHeader), 1);

      // supports only Format 0
    TableKernSubTable0 table0 = {0};
    table0.mNPairs = npairs;
    table0.mSearchRange = (std::pow(2, std::floor(std::log2(npairs))))* sizeof(TableKernDataFormat0);
    table0.mEntrySelector = std::log2(npairs);
    table0.mRangeShift = npairs * sizeof(TableKernDataFormat0) - table0.mSearchRange;

    if (endian == EndianLittle){
      swapByteOrder(table0.mNPairs);
      swapByteOrder(table0.mSearchRange);
      swapByteOrder(table0.mEntrySelector);
      swapByteOrder(table0.mRangeShift);
    }

    stream->write(&table0, sizeof(TableKernSubTable0), 1);

    std::vector<TableKernDataFormat0> data(npairs);
    for (auto const &lhs : mData){
      for (auto const &rhs : lhs.second)
        data.push_back({lhs.first, rhs.first, rhs.second});
    }

    if (endian == EndianLittle){
      for (auto &elem : data){
        swapByteOrder(elem.mLeft);
        swapByteOrder(elem.mRight);
        swapByteOrder(elem.mValue);
      }
    }

    stream->write(data.data(), sizeof(TableKernDataFormat0), data.size());
  }

  int16_t TableKern::getKerningWidth(uint16_t gidLhs, uint16_t gidRhs) const{
    auto lhs = mData.find(gidLhs);
    if (lhs == std::end(mData))
      return 0;

    auto rhs = lhs->second.find(gidRhs);
    if (rhs == std::end(lhs->second))
      return 0;

    return rhs->second;
  }

  std::unique_ptr<TableKern> TableKern::createSubset(std::vector<uint16_t> const &gids) const{
    return nullptr;
  }

  std::unique_ptr<TableKern> TableKern::createSuperset(TableKern const *kern) const{
    return nullptr;
  }

}}}
