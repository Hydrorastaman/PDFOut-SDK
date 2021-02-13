#include <font/TrueType/TableHead.h>

#include <memory>
#include <algorithm>

#include <Types.h>
#include <Exception.h>
#include <font/TrueType/TableGlyf.h>

#undef min
#undef max

using namespace pdfout;

namespace kernel{ namespace font{ namespace truetype{

  TableHead::TableHead(void)
    : Table(),
    mData(){
    memset(&mData, 0, sizeof(TableHeadData));
  }

  TableHead::~TableHead(void){
  }

  void TableHead::load(pdfout::InputStream *stream){
    stream->read(&mData, sizeof(TableHeadData), 1);
    if (getEndian() == EndianLittle){
      swapByteOrder(mData.mVersion);
      swapByteOrder(mData.mFontRevision);
      swapByteOrder(mData.mFontRevision);
      swapByteOrder(mData.mCheckSumAdjustment);
      swapByteOrder(mData.mMagicNumber);
      swapByteOrder(mData.mFlags);
      swapByteOrder(mData.mUnitsPerEm);
      swapByteOrder(mData.mCreated);
      swapByteOrder(mData.mModified);
      swapByteOrder(mData.mXMin);
      swapByteOrder(mData.mYMin);
      swapByteOrder(mData.mXMax);
      swapByteOrder(mData.mYMax);
      swapByteOrder(mData.mMacStyle);
      swapByteOrder(mData.mLowestRecPPEM);
      swapByteOrder(mData.mFontDirectionHint);
      swapByteOrder(mData.mIndexToLocFormat);
      swapByteOrder(mData.mGlyphDataFormat);
    }
  }

  void TableHead::save(pdfout::OutputStream *stream) const{
    TableHeadData data;
    memcpy(&data, &mData, sizeof(TableHeadData));

    if (getEndian() == EndianLittle){
      swapByteOrder(data.mVersion);
      swapByteOrder(data.mFontRevision);
      swapByteOrder(data.mFontRevision);
      swapByteOrder(data.mCheckSumAdjustment);
      swapByteOrder(data.mMagicNumber);
      swapByteOrder(data.mFlags);
      swapByteOrder(data.mUnitsPerEm);
      swapByteOrder(data.mCreated);
      swapByteOrder(data.mModified);
      swapByteOrder(data.mXMin);
      swapByteOrder(data.mYMin);
      swapByteOrder(data.mXMax);
      swapByteOrder(data.mYMax);
      swapByteOrder(data.mMacStyle);
      swapByteOrder(data.mLowestRecPPEM);
      swapByteOrder(data.mFontDirectionHint);
      swapByteOrder(data.mIndexToLocFormat);
      swapByteOrder(data.mGlyphDataFormat);
    }

    stream->write(&data, sizeof(TableHeadData), 1);
  }

  uint16_t TableHead::getMacStyle(void) const{
    return mData.mMacStyle;
  }

  int16_t TableHead::getXMax(void) const{
    return mData.mXMax;
  }

  int16_t TableHead::getXMin(void) const{
    return mData.mXMin;
  }

  Rectangle TableHead::getBBox(void) const{
    // 4.2.1 Other Coordinate Spaces:
    // "The transformation from glyph space to text space is defined by the font matrix. For most types of fonts, this matrix is predefined to map 1000 units of glyph space to 1 unit of text space"
    float const scale = 1000.f / mData.mUnitsPerEm;
    Rectangle const bbox = {mData.mXMin * scale, mData.mYMin * scale, mData.mXMax * scale, mData.mYMax * scale};
    return bbox;
  }

  uint16_t TableHead::getUnitsPerEm(void) const {return mData.mUnitsPerEm;}

  std::unique_ptr<TableHead> TableHead::createSubset(FontInfo const &fontInfo) const{
    std::unique_ptr<TableHead> head = std::make_unique<TableHead>(*this);

    if (fontInfo.empty()){
      head->mData.mXMax = 0;
      head->mData.mXMin = 0;
      head->mData.mYMax = 0;
      head->mData.mYMin = 0;
      return head;
    }

    int16_t xMax = 0;
    int16_t xMin = 1000;
    int16_t yMax = 0;
    int16_t yMin = 1000;

    for (auto const &elem : fontInfo){
      xMax = std::max(xMax, elem.second.mXMax);
      xMin = std::min(xMin, elem.second.mXMin);
      yMax = std::max(yMax, elem.second.mYMax);
      yMin = std::min(yMin, elem.second.mYMin);
    }

    head->mData.mXMax = xMax;
    head->mData.mXMin = xMin;
    head->mData.mYMax = yMax;
    head->mData.mYMin = yMin;

    return head;
  }

  std::unique_ptr<TableHead> TableHead::createSuperset(TableHead const *head) const{
    std::unique_ptr<TableHead> superset = std::make_unique<TableHead>(*this);

    if (!head)
      return superset;

    if (mData.mVersion != head->mData.mVersion)
      NOT_IMPLEMENTED();

    if (head->mData.mFontRevision != mData.mFontRevision ||
        head->mData.mUnitsPerEm != mData.mUnitsPerEm ||
        head->mData.mMacStyle != mData.mMacStyle ||
        head->mData.mLowestRecPPEM != mData.mLowestRecPPEM ||
        head->mData.mFlags != mData.mFlags ||
        head->mData.mCreated != mData.mCreated ||
        head->mData.mModified != mData.mModified)
      RUNTIME_EXCEPTION("Trying to merge two different TrueType fonts");

    superset->mData.mXMax = std::max(mData.mXMax, head->mData.mXMax);
    superset->mData.mXMin = std::min(mData.mXMin, head->mData.mXMin);
    superset->mData.mYMax = std::max(mData.mYMax, head->mData.mYMax);
    superset->mData.mYMin = std::min(mData.mYMin, head->mData.mYMin);

    return superset;
  }

}}}
