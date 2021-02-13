#include <font/TrueType/TableGlyf.h>

#include <algorithm>

#include <Exception.h>
#include <font/TrueType/Glyph.h>
#include <font/TrueType/TableLoca.h>
#include <font/TrueType/SimpleGlyph.h>
#include <font/TrueType/CompoundGlyph.h>

#undef min
#undef max

namespace kernel{ namespace font{ namespace truetype{

  TableGlyf::TableGlyf(TableLoca const *tableLoca)
    : Table(),
    mTableLoca(tableLoca),
    mGlyphs(){
  }

  TableGlyf::TableGlyf(TableGlyf const &obj)
    : Table(obj),
    mTableLoca(obj.mTableLoca),
    mGlyphs(obj.mGlyphs.size()){

    for (std::size_t i = 0, size = mGlyphs.size(); i < size; ++i){
      mGlyphs[i].mLength = obj.mGlyphs[i].mLength;
      mGlyphs[i].mOffset = obj.mGlyphs[i].mOffset;
      if (obj.mGlyphs[i].mGlyph)
        mGlyphs[i].mGlyph = obj.mGlyphs[i].mGlyph->clone({});
      else
        mGlyphs[i].mGlyph = nullptr;
    }
  }

  TableGlyf::~TableGlyf(void) {}

  void TableGlyf::load(pdfout::InputStream *stream){
    Endian const endian = getEndian();

    GlyphDescription description = {0};
    uint64_t const startOffset = stream->tellg();
    uint32_t offset = 0;
    uint32_t length = 0;

    mGlyphs.resize(mTableLoca->mNumGlyphs);

    for (std::size_t i = 0, count = mTableLoca->mNumGlyphs; i < count; ++i){
      length = mTableLoca->mOffsets[i + 1] - mTableLoca->mOffsets[i];
      offset = mTableLoca->mOffsets[i];

      mGlyphs[i].mLength = length;
      mGlyphs[i].mOffset = offset;

      if (!length){
        mGlyphs[i].mGlyph = nullptr;
        continue;
      }

      stream->seekg(startOffset + offset);

      memset(&description, 0, sizeof(GlyphDescription));
      stream->read(&description, sizeof(GlyphDescription), 1);

      if (endian == EndianLittle){
        swapByteOrder(description.mNumberOfContours);
        swapByteOrder(description.mXMin);
        swapByteOrder(description.mYMin);
        swapByteOrder(description.mXMax);
        swapByteOrder(description.mYMax);
      }

      if (description.mNumberOfContours >= 0)
        mGlyphs[i].mGlyph = std::make_unique<SimpleGlyph>(stream, description);
      else
        mGlyphs[i].mGlyph = std::make_unique<CompoundGlyph>(stream, description);
    }
  }

  void TableGlyf::save(pdfout::OutputStream *stream) const{
    uint64_t const startOffset = stream->tellp();

    for (auto const &elem : mGlyphs){
      if (!elem.mLength)
        continue;

      stream->seekp(startOffset + elem.mOffset);
      elem.mGlyph->save(stream);
    }

    uint8_t stub[4] = {0};
    uint64_t const tableLen = stream->tellp() - startOffset;
    if (!tableLen)
      stream->write(stub, sizeof(uint8_t), 4);
    else{
      uint64_t const stubSize = getPadTo4(stream->tellp() - startOffset);
      stream->write(stub, sizeof(uint8_t), stubSize);
    }
  }

  std::unique_ptr<TableGlyf> TableGlyf::createSubset(std::vector<uint16_t> const &gids) const{
    std::unique_ptr<TableGlyf> glyf = std::make_unique<TableGlyf>(nullptr);
    glyf->mGlyphs.resize(gids.size());

    uint32_t offset = 0;
    uint16_t gid = 0;
    for (std::size_t i = 0; i < gids.size(); ++i){
      gid = gids[i];

      if (gid >= mGlyphs.size())
        continue;

      auto const &elem = mGlyphs[gid];
      glyf->mGlyphs[i].mLength = elem.mLength;
      glyf->mGlyphs[i].mOffset = offset;

      if (elem.mLength)
        glyf->mGlyphs[i].mGlyph = elem.mGlyph->clone(gids);
      else
        glyf->mGlyphs[i].mGlyph = nullptr;

      offset += elem.mLength;
    }

    return glyf;
  }

  std::unique_ptr<TableGlyf> TableGlyf::createSuperset(TableGlyf const *glyf, TableLoca const *loca) const{
    std::unique_ptr<TableGlyf> superset = std::make_unique<TableGlyf>(*this);
    superset->mTableLoca = loca;

    if (!glyf)
      return superset;

    std::size_t const numGlyphs = loca->mNumGlyphs;
    std::vector<TableGlyfInfo> glyphs(numGlyphs);

    for (std::size_t i = 0; i < numGlyphs; ++i){
      glyphs[i].mLength = loca->mOffsets[i + 1] - loca->mOffsets[i];
      glyphs[i].mOffset = loca->mOffsets[i];
    }

    for (std::size_t i = 0, size = mGlyphs.size(); i < size; ++i){
      if (mGlyphs[i].mGlyph && mGlyphs[i].mLength && mGlyphs[i].mLength == glyphs[i].mLength)
        glyphs[i].mGlyph = mGlyphs[i].mGlyph->clone({});
    }

    for (std::size_t i = 0, size = glyf->mGlyphs.size(); i < size; ++i){
      if (glyf->mGlyphs[i].mGlyph && glyf->mGlyphs[i].mLength && glyf->mGlyphs[i].mLength == glyphs[i].mLength && !glyphs[i].mGlyph)
        glyphs[i].mGlyph = glyf->mGlyphs[i].mGlyph->clone({});
    }

    superset->mGlyphs = std::move(glyphs);
    return superset;
  }

  void TableGlyf::extractCompoundGids(std::vector<uint16_t> const &gids, std::vector<uint16_t> &extraGids) const{
    uint16_t gid = 0;
    for (std::size_t i = 0; i < gids.size(); ++i){
      gid = gids[i];
      if (gid >= mGlyphs.size())
        continue;

      auto const &elem = mGlyphs[gid];

      if (elem.mLength && elem.mGlyph->getType() == GlyphTypeCompound){
        CompoundGlyph const *glyph = (CompoundGlyph const *) elem.mGlyph.get();
        std::vector<uint16_t> components = glyph->getComponents();
        for (auto elem : components){
          auto iter = std::find(std::begin(gids), std::end(gids), elem);
          if (iter != std::end(gids))
            continue;

          auto iter2 = std::find(std::begin(extraGids), std::end(extraGids), elem);
          if (iter2 != std::end(extraGids))
            continue;

          extraGids.push_back(elem);
        }
      }
    }
  }

  void TableGlyf::updateFontInfo(FontInfo &fontInfo) const{
    for (std::size_t gid = 0; gid < mGlyphs.size(); ++gid){
      auto const &elem = mGlyphs[gid];

      if (!elem.mLength)
        continue;

      GlyphDescription const desc = elem.mGlyph->getGlyphDescription();
      Glyph const *glyf = elem.mGlyph.get();
      GlyphType const type = glyf->getType();

      fontInfo[gid].mXMax = desc.mXMax;
      fontInfo[gid].mXMin = desc.mXMin;
      fontInfo[gid].mYMax = desc.mYMax;
      fontInfo[gid].mYMin = desc.mYMin;

      if (type == GlyphTypeSimple){
        SimpleGlyph const *simpleGlyph = (SimpleGlyph const *) glyf;
        fontInfo[gid].mPoints = simpleGlyph->getMaxPointsNum();
        fontInfo[gid].mContours = desc.mNumberOfContours;
        fontInfo[gid].mSizeOfInstructions = simpleGlyph->getInstructionLength();
        fontInfo[gid].mCompositeContours = 0;
        fontInfo[gid].mCompositePoints = 0;
        fontInfo[gid].mComponentElements = 0;
        fontInfo[gid].mComponentDepth = 0;
      }
      else{
        CompoundGlyph const *compGlyph = (CompoundGlyph const *) glyf;
        fontInfo[gid].mPoints = 0;
        fontInfo[gid].mContours = 0;
        fontInfo[gid].mSizeOfInstructions = 0;
        fontInfo[gid].mCompositeContours = getCompoundMaxContours(gid);
        fontInfo[gid].mCompositePoints = getCompoundMaxPointsNum(gid);
        fontInfo[gid].mComponentElements = compGlyph->getComponents().size();
        fontInfo[gid].mComponentDepth = getComponentDepth(gid) - 1;
      }
    }
  }

  Glyph const *TableGlyf::getGlyph(uint16_t gid) const{
    return mGlyphs[gid].mGlyph.get();
  }

  uint16_t TableGlyf::getCompoundMaxPointsNum(uint16_t gid) const{
    if (gid >= mGlyphs.size())
      return 0;

    auto const &elem = mGlyphs[gid];

    Glyph const *glyf = elem.mGlyph.get();
    if (!glyf)
      return 0;

    GlyphType const type = glyf->getType();

    if (type == GlyphTypeSimple)
      return ((SimpleGlyph const *) glyf)->getMaxPointsNum();

    uint16_t maxPoints = 0;
    if (type == GlyphTypeCompound){
      CompoundGlyph const *compGlyph = (CompoundGlyph const *) glyf;
      std::vector<uint16_t> components = compGlyph->getComponents();
      for (auto gid : components)
        maxPoints += getCompoundMaxPointsNum(gid);

      return maxPoints;
    }

    return 0;
  }

  uint16_t TableGlyf::getCompoundMaxContours(uint16_t gid) const{
    if (gid >= mGlyphs.size())
      return 0;

    auto const &elem = mGlyphs[gid];

    Glyph const *glyf = elem.mGlyph.get();
    if (!glyf)
      return 0;

    GlyphType const type = glyf->getType();

    if (type == GlyphTypeSimple)
      return ((SimpleGlyph const *) glyf)->getGlyphDescription().mNumberOfContours;

    uint16_t maxContours = 0;
    if (type == GlyphTypeCompound){
      CompoundGlyph const *compGlyph = (CompoundGlyph const *) glyf;
      std::vector<uint16_t> components = compGlyph->getComponents();
      for (auto gid : components)
        maxContours += getCompoundMaxContours(gid);

      return maxContours;
    }

    return 0;
  }

  uint16_t TableGlyf::getComponentDepth(uint16_t gid) const{
    if (gid >= mGlyphs.size())
      return 0;

    auto const &elem = mGlyphs[gid];

    Glyph const *glyf = elem.mGlyph.get();
    if (!glyf)
      return 0;

    GlyphType const type = glyf->getType();

    if (type == GlyphTypeSimple)
      return 1;

    uint16_t maxDepth = 0;
    if (type == GlyphTypeCompound){
      CompoundGlyph const *compGlyph = (CompoundGlyph const *) glyf;
      std::vector<uint16_t> components = compGlyph->getComponents();
      for (auto gid : components)
        maxDepth = std::max(maxDepth, getComponentDepth(gid));

      return maxDepth + 1;
    }

    return 0;
  }

}}}
