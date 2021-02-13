#include <font/TrueType/CompoundGlyph.h>

namespace kernel{ namespace font{ namespace truetype{

  CompoundGlyph::CompoundGlyph(pdfout::InputStream *stream, GlyphDescription const &description)
    : Glyph(GlyphTypeCompound){
    load(stream, description);
  }

  CompoundGlyph::~CompoundGlyph(void){
  }

  void CompoundGlyph::save(pdfout::OutputStream *stream) const{
    Endian const endian = getEndian();

    GlyphDescription description;
    memcpy(&description, &mDescription, sizeof(GlyphDescription));
    if (endian == EndianLittle){
      swapByteOrder(description.mNumberOfContours);
      swapByteOrder(description.mXMin);
      swapByteOrder(description.mYMin);
      swapByteOrder(description.mXMax);
      swapByteOrder(description.mYMax);
    }

    stream->write(&description, sizeof(GlyphDescription), 1);

    uint16_t flags = 0;
    uint16_t glyphIndex = 0;
    for (auto const &elem : mData){
      flags = elem.mFlags;
      glyphIndex = elem.mGlyphIndex;
      if (endian == EndianLittle){
        swapByteOrder(flags);
        swapByteOrder(glyphIndex);
      }

      stream->write(&flags, sizeof(uint16_t), 1);
      stream->write(&glyphIndex, sizeof(uint16_t), 1);
      stream->write(elem.mAdditionalData.data(), sizeof(uint8_t), elem.mAdditionalData.size());
    }

    uint16_t instructionLength = mInstructions.size();
    if (endian == EndianLittle)
      swapByteOrder(instructionLength);

    stream->write(&instructionLength, sizeof(uint16_t), 1);
    stream->write(mInstructions.data(), sizeof(uint8_t), mInstructions.size());
  }

  std::unique_ptr<Glyph> CompoundGlyph::clone(std::vector<uint16_t> const &gids) const{
    Endian const endian = getEndian();
    std::unique_ptr<Glyph> obj(new CompoundGlyph(*this));
    CompoundGlyph *glyph = (CompoundGlyph *) obj.get();
    for (auto &elem : glyph->mData){
      for (std::size_t i = 0; i < gids.size(); ++i){
        if (elem.mGlyphIndex == gids[i])
          elem.mGlyphIndex = i;
      }
    }

    return obj;
  }

  std::vector<uint16_t> CompoundGlyph::getComponents(void) const{
    std::vector<uint16_t> components;
    components.reserve(mData.size());
    for (auto const &elem : mData)
      components.push_back(elem.mGlyphIndex);

    return components;
  }

  GlyphDescription const &CompoundGlyph::getGlyphDescription(void) const{
    return mDescription;
  }

  void CompoundGlyph::load(pdfout::InputStream *stream, GlyphDescription const &description){
    Endian const endian = getEndian();

    uint16_t flags = 0;
    uint16_t glyphIndex = 0;
    uint64_t offset = stream->tellg();

    memcpy(&mDescription, &description, sizeof(GlyphDescription));

    do{
      stream->read(&flags, sizeof(uint16_t), 1);
      stream->read(&glyphIndex, sizeof(uint16_t), 1);

      if (endian == EndianLittle){
        swapByteOrder(flags);
        swapByteOrder(glyphIndex);
      }

      std::size_t size = 0;
      if (flags & ComponentFlagArg1And2AreWords)
        size += 2 * sizeof(uint16_t);
      else
        size += 2 * sizeof(uint8_t);

      if (flags & ComponentFlagWeHaveAScale)
        size += sizeof(int16_t);
      else if (flags & ComponentFlagWeHaveAnXAndYScale)
        size += 2 * sizeof(int16_t);
      else if (flags & ComponentFlagWeHaveATwoByTwo)
        size += 4 * sizeof(int16_t);

      CompoundData data = {flags, glyphIndex};
      data.mAdditionalData.resize(size);
      stream->read(data.mAdditionalData.data(), sizeof(uint8_t), size);
      mData.push_back(std::move(data));
    }
    while (flags & ComponentFlagMoreComponents);

    if (flags & ComponentFlagWeHaveInstructions){
      uint16_t instructionLength = 0;
      stream->read(&instructionLength, sizeof(uint16_t), 1);
      if (endian == EndianLittle)
        swapByteOrder(instructionLength);

      mInstructions.resize(instructionLength);
      stream->read(mInstructions.data(), sizeof(uint8_t), instructionLength);
    }
  }

}}}
