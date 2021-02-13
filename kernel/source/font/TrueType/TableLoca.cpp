#include <font/TrueType/TableLoca.h>

#include <map>
#include <memory>
#include <vector>
#include <algorithm>

#include <Exception.h>
#include <font/TrueType/TableDttf.h>
#include <font/TrueType/SimpleGlyph.h>
#include <font/TrueType/CompoundGlyph.h>

#undef min
#undef max

namespace kernel{ namespace font{ namespace truetype{

  TableLoca::TableLoca(uint16_t numGlyphs, int16_t indexToLocFormat)
    : Table(),
    mNumGlyphs(numGlyphs),
    mIndexToLocFormat(indexToLocFormat),
    mOffsets(numGlyphs + 1){
  }

  TableLoca::~TableLoca(void){
  }

  void TableLoca::load(pdfout::InputStream *stream){
    if (mIndexToLocFormat == 0)
      loadOffsets<uint16_t>(stream);
    else
      loadOffsets<uint32_t>(stream);
  }

  void TableLoca::save(pdfout::OutputStream *stream) const{
    if (mIndexToLocFormat == 0)
      saveOffsets<uint16_t>(stream);
    else
      saveOffsets<uint32_t>(stream);
  }

  void TableLoca::unpack(TableDttf *dttf){
    uint32_t const noOffset = (uint32_t) (-1);
    std::vector<uint32_t> offsets(dttf->mData.mMaxGlyphIndexUsed + 2, noOffset);
    std::size_t index = 0;
    for (std::size_t i = 0, size = dttf->mGlyphIndexArray.size(); i < size; ++i){
      index = dttf->mGlyphIndexArray[i];
      offsets[index] = mOffsets[i];
    }

    offsets[dttf->mData.mMaxGlyphIndexUsed + 1] = mOffsets[dttf->mData.mGlyphCount];

    int32_t const size = offsets.size();
    uint32_t prevValue = offsets[size - 1];
    for (int32_t i = size - 1; i >= 0; --i)
      if (offsets[i] == noOffset)
        offsets[i] = prevValue;
      else
        prevValue = offsets[i];

    mNumGlyphs = dttf->mData.mMaxGlyphIndexUsed + 1;
    mOffsets = std::move(offsets);
  }

  std::unique_ptr<TableLoca> TableLoca::createSubset(std::vector<uint16_t> const &gids) const{
    std::unique_ptr<TableLoca> loca = std::make_unique<TableLoca>(gids.size(), mIndexToLocFormat);

    uint32_t offset = 0;

    for (std::size_t i = 0; i < gids.size(); ++i){
      loca->mOffsets[i] = offset;
      uint16_t const gid = gids[i];
      uint32_t const glyphLen = mOffsets[gid + 1] - mOffsets[gid];
      offset += glyphLen;
    }

    loca->mOffsets[gids.size()] = offset;

    return loca;
  }

  std::unique_ptr<TableLoca> TableLoca::createSuperset(TableLoca const *loca) const{
    std::unique_ptr<TableLoca> superset = std::make_unique<TableLoca>(*this);

    if (!loca)
      return superset;

    uint16_t const numGlyphs = std::max(mNumGlyphs, loca->mNumGlyphs);
    std::vector<uint32_t> glyphLen((std::size_t) numGlyphs, 0);

    for (std::size_t i = 0; i < mNumGlyphs; ++i)
      glyphLen[i] = mOffsets[i + 1] - mOffsets[i];

    for (std::size_t i = 0; i < loca->mNumGlyphs; ++i){
      if (!glyphLen[i])
        glyphLen[i] = loca->mOffsets[i + 1] - loca->mOffsets[i];
    }

    uint32_t offset = 0;
    std::vector<uint32_t> offsets(numGlyphs + 1, 0);
    for (std::size_t i = 0, size = glyphLen.size(); i < size; ++i){
      offsets[i] = offset;
      offset += glyphLen[i];
    }

    offsets[numGlyphs] = offset;

    superset->mNumGlyphs = numGlyphs;
    superset->mOffsets = std::move(offsets);

    return superset;
  }

  std::vector<uint32_t> TableLoca::getGidOffset(std::vector<uint16_t> const &gids) const{
    std::vector<uint32_t> offsets(gids.size());

    for (std::size_t i = 0; i < gids.size(); ++i)
      offsets[i] = mOffsets[gids[i]];

    return offsets;
  }

  uint32_t TableLoca::getGidOffset(uint16_t gid) const{
    return mOffsets[gid];
  }

}}}
