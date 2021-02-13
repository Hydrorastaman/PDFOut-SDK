#include <font/TrueType/TableHmtx.h>

#include <memory>
#include <algorithm>

#include <Exception.h>
#include <font/TrueType/TableDttf.h>
#include <font/TrueType/TableGlyf.h>

#undef min
#undef max

namespace kernel{ namespace font{ namespace truetype{

  TableHmtx::TableHmtx(uint16_t numOfMetrics, uint16_t numGlyphs)
    : Table(),
    mNumOfMetrics(numOfMetrics),
    mNumGlyphs(numGlyphs){
  }

  TableHmtx::~TableHmtx(void){
  }

  void TableHmtx::load(pdfout::InputStream *stream){
    Endian const endian = getEndian();
    mHMetrix.resize(mNumOfMetrics);
    stream->read(mHMetrix.data(), sizeof(LongHorMetrics), mNumOfMetrics);
    if (endian == EndianLittle){
      for (auto &elem : mHMetrix){
        swapByteOrder(elem.mAdvanceWidth);
        swapByteOrder(elem.mLsb);
      }
    }   

    std::size_t count = mNumGlyphs - mNumOfMetrics;
    mLeftSideBearing.resize(count);
    stream->read(mLeftSideBearing.data(), sizeof(int16_t), count);
    if (endian == EndianLittle)
      for (auto &elem : mLeftSideBearing)
          swapByteOrder(elem);
  }

  void TableHmtx::save(pdfout::OutputStream *stream) const{
    if (getEndian() == EndianLittle){
      LongHorMetrics metrix;
      for (auto const &elem : mHMetrix){
        metrix = elem;
        swapByteOrder(metrix.mAdvanceWidth);
        swapByteOrder(metrix.mLsb);
        stream->write(&metrix, sizeof(LongHorMetrics), 1);
      }

      int16_t lsb = 0;
      for (auto const &elem : mLeftSideBearing){
        lsb = elem;
        swapByteOrder(lsb);
        stream->write(&lsb, sizeof(int16_t), 1);
      }
    }
    else{
      stream->write(mHMetrix.data(), sizeof(LongHorMetrics), mHMetrix.size());
      stream->write(mLeftSideBearing.data(), sizeof(int16_t), mLeftSideBearing.size());
    }
  }

  void TableHmtx::update(TableDttf *dttf){
    std::vector<LongHorMetrics> hmetrix(dttf->mData.mMaxGlyphIndexUsed + 1, {0, 0});
    std::size_t index = 0;
    for (std::size_t i = 0, size = dttf->mGlyphIndexArray.size(); i < size; ++i){
      index = dttf->mGlyphIndexArray[i];
      hmetrix[index] = mHMetrix[i];
    }

    mNumGlyphs = dttf->mData.mMaxGlyphIndexUsed + 1;
    mHMetrix = std::move(hmetrix);
  }

  std::unique_ptr<TableHmtx> TableHmtx::createSubset(std::vector<uint16_t> const &gids) const{
    std::unique_ptr<TableHmtx> hmtx = std::make_unique<TableHmtx>(gids.size(), gids.size());
    uint16_t maxIndex = mNumOfMetrics - 1;
    uint16_t defaultWidth = mHMetrix[maxIndex].mAdvanceWidth;

    hmtx->mHMetrix.resize(gids.size());
    for (std::size_t i = 0; i < gids.size(); ++i){
      uint16_t const gid = gids[i];
      if (gid <= maxIndex)
        hmtx->mHMetrix[i] = mHMetrix[gid];
      else
        hmtx->mHMetrix[i] = {defaultWidth, mLeftSideBearing[gid - mNumOfMetrics]};
    }

    return hmtx;
  }

  std::unique_ptr<TableHmtx> TableHmtx::createSuperset(TableHmtx const *hmtx) const{
    std::unique_ptr<TableHmtx> superset = std::make_unique<TableHmtx>(*this);

    if (!hmtx)
      return superset;

    uint16_t const numGlyphs = std::max(mNumGlyphs, hmtx->mNumGlyphs);
    uint16_t const numOfMetrics = std::max(mNumOfMetrics, hmtx->mNumOfMetrics);

    std::vector<LongHorMetrics> hMetrics(numOfMetrics, {0, 0});
    memcpy(hMetrics.data(), mHMetrix.data(), mHMetrix.size() * sizeof(LongHorMetrics));

    for (std::size_t i = 0, size = hmtx->mHMetrix.size(); i < size; ++i){
      if (hmtx->mHMetrix[i].mAdvanceWidth)
        memcpy(&hMetrics[i], &hmtx->mHMetrix[i], sizeof(LongHorMetrics));
    }

    std::size_t lsbSize = numGlyphs - numOfMetrics;
    std::vector<int16_t> lsb(lsbSize, 0);

    if (lsbSize){
      if (mNumGlyphs >= hmtx->mNumGlyphs){
        std::size_t const offset = numOfMetrics - mHMetrix.size();
        memcpy(lsb.data(), &mLeftSideBearing.data()[offset], lsbSize);
      }
      else{
        std::size_t const offset = numOfMetrics - hmtx->mHMetrix.size();
        memcpy(lsb.data(), &hmtx->mLeftSideBearing.data()[offset], lsbSize);
      }
    }

    superset->mNumOfMetrics = numOfMetrics;
    superset->mNumGlyphs = numGlyphs;
    superset->mHMetrix = std::move(hMetrics);
    superset->mLeftSideBearing = std::move(lsb);

    return superset;
  }

  void TableHmtx::updateFontInfo(FontInfo &fontInfo) const{
    for (std::size_t i = 0; i < mHMetrix.size(); ++i){
      fontInfo[i].mAdvWidth = mHMetrix[i].mAdvanceWidth;
      fontInfo[i].mLSB = mHMetrix[i].mLsb;
      fontInfo[i].mRSB = mHMetrix[i].mAdvanceWidth - mHMetrix[i].mLsb - (fontInfo[i].mXMax - fontInfo[i].mXMin);
      fontInfo[i].mXExtent = mHMetrix[i].mLsb + (fontInfo[i].mXMax - fontInfo[i].mXMin);
    }

    int16_t const advWidth = mHMetrix[mHMetrix.size() - 1].mAdvanceWidth;
    uint16_t const hMetrixSize = mHMetrix.size();
    for (std::size_t i = 0; i < mLeftSideBearing.size(); ++i){
      uint16_t const gid = hMetrixSize + i;
      fontInfo[gid].mAdvWidth = advWidth;
      fontInfo[gid].mLSB = mLeftSideBearing[i];
      fontInfo[gid].mRSB = advWidth - mLeftSideBearing[i] - (fontInfo[gid].mXMax - fontInfo[gid].mXMin);
      fontInfo[gid].mXExtent = mLeftSideBearing[i] + (fontInfo[gid].mXMax - fontInfo[gid].mXMin);
    }
  }

}}}
