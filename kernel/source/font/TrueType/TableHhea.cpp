#include <font/TrueType/TableHhea.h>

#include <memory>
#include <algorithm>

#include <Exception.h>
#include <font/TrueType/TableHmtx.h>

#undef min
#undef max

namespace kernel{ namespace font{ namespace truetype{

  TableHhea::TableHhea(void)
    : Table(){
  }

  TableHhea::~TableHhea(void){
    }

  void TableHhea::load(pdfout::InputStream *stream){
    stream->read(&mData, sizeof(TableHheaData), 1);
    memset(mData.mReserved, 0, sizeof(int16_t) * 4);
    if (getEndian() == EndianLittle){
      swapByteOrder(mData.mVersion);
      swapByteOrder(mData.mAscender);
      swapByteOrder(mData.mDescender);
      swapByteOrder(mData.mLineGap);
      swapByteOrder(mData.mAdvanceWidthMax);
      swapByteOrder(mData.mMinLeftSideBearing);
      swapByteOrder(mData.mMinRightSideBearing);
      swapByteOrder(mData.mXMaxExtent);
      swapByteOrder(mData.mCaretSlopeRise);
      swapByteOrder(mData.mCaretSlopeRun);
      swapByteOrder(mData.mCaretOffset);
      swapByteOrder(mData.mMetricDataFormat);
      swapByteOrder(mData.mNumberOfHMetrics);
    }
  }

  void TableHhea::save(pdfout::OutputStream *stream) const{
    TableHheaData data;
    memcpy(&data, &mData, sizeof(TableHheaData));

    if (getEndian() == EndianLittle){
      swapByteOrder(data.mVersion);
      swapByteOrder(data.mAscender);
      swapByteOrder(data.mDescender);
      swapByteOrder(data.mLineGap);
      swapByteOrder(data.mAdvanceWidthMax);
      swapByteOrder(data.mMinLeftSideBearing);
      swapByteOrder(data.mMinRightSideBearing);
      swapByteOrder(data.mXMaxExtent);
      swapByteOrder(data.mCaretSlopeRise);
      swapByteOrder(data.mCaretSlopeRun);
      swapByteOrder(data.mCaretOffset);
      swapByteOrder(data.mMetricDataFormat);
      swapByteOrder(data.mNumberOfHMetrics);
    }

    stream->write(&data, sizeof(TableHheaData), 1);
  }

  uint16_t TableHhea::getNumberOfHMetrics(void) const {return mData.mNumberOfHMetrics;}
  int16_t TableHhea::getAscender(void) const {return mData.mAscender;}
  int16_t TableHhea::getDescender(void) const {return mData.mDescender;}
  int16_t TableHhea::getLineGap(void) const {return mData.mLineGap;}

  std::unique_ptr<TableHhea> TableHhea::createSubset(std::vector<uint16_t> const &gids, FontInfo const &fontInfo) const{
    std::unique_ptr<TableHhea> hhea = std::make_unique<TableHhea>(*this);
    hhea->mData.mNumberOfHMetrics = gids.size();

    int16_t minLsb = 1000;
    int16_t minRSB = 1000;
    int16_t maxAdvWidth = 0;
    int16_t maxXExtent = 0;

    for (auto const &elem : fontInfo){
      minLsb = std::min(minLsb, elem.second.mLSB);
      minRSB = std::min(minRSB, elem.second.mRSB);
      maxAdvWidth = std::max(maxAdvWidth, elem.second.mAdvWidth);
      maxXExtent = std::max(maxXExtent, elem.second.mXExtent);
    }


    hhea->mData.mMinLeftSideBearing = minLsb;
    hhea->mData.mMinRightSideBearing = minRSB;
    hhea->mData.mAdvanceWidthMax = maxAdvWidth;
    hhea->mData.mXMaxExtent = maxXExtent;

    return hhea;
  }

  std::unique_ptr<TableHhea> TableHhea::createSuperset(TableHhea const *hhea) const{
    std::unique_ptr<TableHhea> superset = std::make_unique<TableHhea>(*this);

    if (!hhea)
      return superset;

    if (mData.mVersion != hhea->mData.mVersion)
      NOT_IMPLEMENTED();

    superset->mData.mAscender = std::max(mData.mAscender , hhea->mData.mAscender);
    superset->mData.mDescender = std::min(mData.mDescender , hhea->mData.mDescender);
    superset->mData.mLineGap = std::max(mData.mLineGap , hhea->mData.mLineGap);
    superset->mData.mAdvanceWidthMax = std::max(mData.mAdvanceWidthMax , hhea->mData.mAdvanceWidthMax);
    superset->mData.mMinLeftSideBearing = std::min(mData.mMinLeftSideBearing , hhea->mData.mMinLeftSideBearing);
    superset->mData.mMinRightSideBearing = std::min(mData.mMinRightSideBearing , hhea->mData.mMinRightSideBearing);
    superset->mData.mXMaxExtent = std::max(mData.mXMaxExtent , hhea->mData.mXMaxExtent);
    superset->mData.mCaretSlopeRise = std::max(mData.mCaretSlopeRise , hhea->mData.mCaretSlopeRise);
    superset->mData.mCaretSlopeRun = std::max(mData.mCaretSlopeRun , hhea->mData.mCaretSlopeRun);
    superset->mData.mCaretOffset = std::max(mData.mCaretOffset , hhea->mData.mCaretOffset);
    superset->mData.mMetricDataFormat = std::max(mData.mMetricDataFormat , hhea->mData.mMetricDataFormat);
    superset->mData.mNumberOfHMetrics = std::max(mData.mNumberOfHMetrics , hhea->mData.mNumberOfHMetrics);

    return superset;
  }

}}}
