#include <font/TrueType/TableMaxp.h>

#include <memory>
#include <algorithm>

#include <Exception.h>
#include <font/TrueType/TableGlyf.h>

#undef min
#undef max

namespace kernel{ namespace font{ namespace truetype{

  TableMaxp::TableMaxp(void)
    : Table(){

    memset(&mData, 0, sizeof(TableMaxpData));
  }

  TableMaxp::~TableMaxp(void){
    }

  void TableMaxp::load(pdfout::InputStream *stream){
    stream->read(&mData, sizeof(TableMaxpData), 1);
    if (getEndian() == EndianLittle){
      swapByteOrder(mData.mVersion);
      swapByteOrder(mData.mNumGlyphs);
      swapByteOrder(mData.mMaxPoints);
      swapByteOrder(mData.mMaxContours);
      swapByteOrder(mData.mMaxCompositePoints);
      swapByteOrder(mData.mMaxCompositeContours);
      swapByteOrder(mData.mMaxZones);
      swapByteOrder(mData.mMaxTwilightPoints);
      swapByteOrder(mData.mMaxStorage);
      swapByteOrder(mData.mMaxFunctionDefs);
      swapByteOrder(mData.mMaxInstructionDefs);
      swapByteOrder(mData.mMaxStackElements);
      swapByteOrder(mData.mMaxSizeOfInstructions);
      swapByteOrder(mData.mMaxComponentElements);
      swapByteOrder(mData.mMaxComponentDepth);
    }
  }

  void TableMaxp::save(pdfout::OutputStream *stream) const{
    TableMaxpData data;
    memcpy(&data, &mData, sizeof(TableMaxpData));

    if (getEndian() == EndianLittle){
      swapByteOrder(data.mVersion);
      swapByteOrder(data.mNumGlyphs);
      swapByteOrder(data.mMaxPoints);
      swapByteOrder(data.mMaxContours);
      swapByteOrder(data.mMaxCompositePoints);
      swapByteOrder(data.mMaxCompositeContours);
      swapByteOrder(data.mMaxZones);
      swapByteOrder(data.mMaxTwilightPoints);
      swapByteOrder(data.mMaxStorage);
      swapByteOrder(data.mMaxFunctionDefs);
      swapByteOrder(data.mMaxInstructionDefs);
      swapByteOrder(data.mMaxStackElements);
      swapByteOrder(data.mMaxSizeOfInstructions);
      swapByteOrder(data.mMaxComponentElements);
      swapByteOrder(data.mMaxComponentDepth);
    }

    stream->write(&data, sizeof(TableMaxpData), 1);
  }

  std::unique_ptr<TableMaxp> TableMaxp::createSubset(std::vector<uint16_t> const &gids, FontInfo const &fontInfo) const{
    std::unique_ptr<TableMaxp> maxp = std::make_unique<TableMaxp>(*this);
    maxp->mData.mNumGlyphs = (uint16_t) gids.size();

    uint16_t maxPoints = 0;
    uint16_t maxContours = 0;
    uint16_t maxCompositePoints = 0;
    uint16_t maxCompositeContours = 0;
    uint16_t maxSizeOfInstructions = 0;
    uint16_t maxComponentElements = 0;
    uint16_t maxComponentDepth = 0;

    for (auto const &elem : fontInfo){
      maxPoints = std::max(maxPoints, elem.second.mPoints);
      maxContours = std::max(maxContours, (uint16_t) elem.second.mContours);
      maxCompositePoints = std::max(maxCompositePoints, elem.second.mCompositePoints);
      maxCompositeContours = std::max(maxCompositeContours, elem.second.mCompositeContours);
      maxSizeOfInstructions = std::max(maxSizeOfInstructions, elem.second.mSizeOfInstructions);
      maxComponentElements = std::max(maxComponentElements, elem.second.mComponentElements);
      maxComponentDepth = std::max(maxComponentDepth, elem.second.mComponentDepth);
    }

    maxp->mData.mMaxPoints = maxPoints;
    maxp->mData.mMaxContours = maxContours;
    maxp->mData.mMaxCompositePoints = maxCompositePoints;
    maxp->mData.mMaxCompositeContours = maxCompositeContours;
    maxp->mData.mMaxSizeOfInstructions = maxSizeOfInstructions;
    maxp->mData.mMaxComponentElements = maxComponentElements;
    maxp->mData.mMaxComponentDepth = maxComponentDepth;

    return maxp;
  }

  std::unique_ptr<TableMaxp> TableMaxp::createSuperset(TableMaxp const *maxp) const{
    std::unique_ptr<TableMaxp> superset = std::make_unique<TableMaxp>(*this);

    if (!maxp)
      return superset;

    if (maxp->mData.mVersion != mData.mVersion)
      NOT_IMPLEMENTED();

    superset->mData.mNumGlyphs = std::max(mData.mNumGlyphs, maxp->mData.mNumGlyphs);
    superset->mData.mMaxPoints = std::max(mData.mMaxPoints, maxp->mData.mMaxPoints);
    superset->mData.mMaxContours = std::max(mData.mMaxContours, maxp->mData.mMaxContours);
    superset->mData.mMaxCompositePoints = std::max(mData.mMaxCompositePoints, maxp->mData.mMaxCompositePoints);
    superset->mData.mMaxCompositeContours = std::max(mData.mMaxCompositeContours, maxp->mData.mMaxCompositeContours);
    superset->mData.mMaxZones = std::max(mData.mMaxZones, maxp->mData.mMaxZones);
    superset->mData.mMaxTwilightPoints = std::max(mData.mMaxTwilightPoints, maxp->mData.mMaxTwilightPoints);
    superset->mData.mMaxStorage = std::max(mData.mMaxStorage, maxp->mData.mMaxStorage);
    superset->mData.mMaxFunctionDefs = std::max(mData.mMaxFunctionDefs, maxp->mData.mMaxFunctionDefs);
    superset->mData.mMaxInstructionDefs = std::max(mData.mMaxInstructionDefs, maxp->mData.mMaxInstructionDefs);
    superset->mData.mMaxStackElements = std::max(mData.mMaxStackElements, maxp->mData.mMaxStackElements);
    superset->mData.mMaxSizeOfInstructions = std::max(mData.mMaxSizeOfInstructions, maxp->mData.mMaxSizeOfInstructions);
    superset->mData.mMaxComponentElements = std::max(mData.mMaxComponentElements, maxp->mData.mMaxComponentElements);
    superset->mData.mMaxComponentDepth = std::max(mData.mMaxComponentDepth, maxp->mData.mMaxComponentDepth);

    return superset;
  }

}}}
