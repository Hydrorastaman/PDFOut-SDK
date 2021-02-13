#include <font/TrueType/SimpleGlyph.h>

#include <algorithm>

#undef min
#undef max

namespace kernel{ namespace font{ namespace truetype{

  SimpleGlyph::SimpleGlyph(pdfout::InputStream *stream, GlyphDescription const &description)
    : Glyph(GlyphTypeSimple){
    load(stream, description);
  }

  SimpleGlyph::~SimpleGlyph(void){
  }

  void SimpleGlyph::load(pdfout::InputStream *stream, GlyphDescription const &description){
    Endian const endian = getEndian();

    memcpy(&mDescription, &description, sizeof(GlyphDescription));

    if (!mDescription.mNumberOfContours)
      return;

    mEndPtsOfContours.resize(mDescription.mNumberOfContours);
    stream->read(mEndPtsOfContours.data(), sizeof(uint16_t), mDescription.mNumberOfContours);
    if (endian == EndianLittle){
      for (auto &elem : mEndPtsOfContours)
        swapByteOrder(elem);
    }

    stream->read(&mInstructionLength, sizeof(uint16_t), 1);
    if (endian == EndianLittle)
      swapByteOrder(mInstructionLength);

    mInstructions.resize(mInstructionLength);
    stream->read(mInstructions.data(), sizeof(uint8_t), mInstructionLength);

    uint16_t const points = mEndPtsOfContours[mDescription.mNumberOfContours - 1] + 1;
    uint16_t coordLen = readFlags(stream, mFlags, points);

    mCoordinates.resize(coordLen);
    stream->read(mCoordinates.data(), sizeof(uint8_t), coordLen);
  }

  void SimpleGlyph::save(pdfout::OutputStream *stream) const{
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

    if (endian == EndianLittle){
      std::vector<uint16_t> endPtsContours = mEndPtsOfContours;
      for (auto &elem : endPtsContours)
        swapByteOrder(elem);
      stream->write(endPtsContours.data(), sizeof(uint16_t), endPtsContours.size());

      uint16_t instructionLength = mInstructionLength;
      swapByteOrder(instructionLength);
      stream->write(&instructionLength, sizeof(uint16_t), 1);
    }
    else{
      stream->write(mEndPtsOfContours.data(), sizeof(uint16_t), mEndPtsOfContours.size());
      stream->write(&mInstructionLength, sizeof(uint16_t), 1);
    }

    stream->write(mInstructions.data(), sizeof(uint8_t), mInstructions.size());
    stream->write(mFlags.data(), sizeof(uint8_t), mFlags.size());
    stream->write(mCoordinates.data(), sizeof(uint8_t), mCoordinates.size());
  }

  std::unique_ptr<Glyph> SimpleGlyph::clone(std::vector<uint16_t> const &gids) const{
    std::unique_ptr<Glyph> obj(new SimpleGlyph(*this));
    return obj;
  }

  GlyphDescription const &SimpleGlyph::getGlyphDescription(void) const{
    return mDescription;
  }

  uint16_t SimpleGlyph::getMaxPointsNum(void) const{
    uint16_t maxPointsNum = mEndPtsOfContours[0];
    for (auto elem : mEndPtsOfContours)
      maxPointsNum = std::max(maxPointsNum, elem);

    return maxPointsNum + 1;
  }

  uint16_t SimpleGlyph::getInstructionLength(void) const{
    return mInstructionLength;
  }

  uint16_t SimpleGlyph::readFlags(pdfout::InputStream *stream, std::vector<uint8_t> &flags, uint16_t points) const{
    uint16_t delta = 0;
    uint8_t flag = 0;
    uint16_t flagLen = 0;
    uint16_t len = 0;
    uint64_t const startOffset = stream->tellg();

    for (std::size_t i = 0; i < points; ++i){
      delta = 0;
      stream->read(&flag, sizeof(uint8_t), 1);
      ++flagLen;

      if (flag & OutlineFlagXShortVector)  // x-Short
        ++delta;
      else if (!(flag & OutlineFlagThisXIsSame))
        delta += 2;

      if (flag & OutlineFlagYShortVector)  // y-Short
        ++delta;
      else if (!(flag & OutlineFlagThisYIsSame))
        delta += 2;

      if (flag & OutlineFlagRepeat){  // repeat
        uint8_t repeat = 0;
        stream->read(&repeat, sizeof(uint8_t), 1);
        ++flagLen;

        delta += (delta * repeat);
        i += repeat;
      }

      len += delta;
    }

    stream->seekg(startOffset);
    flags.resize(flagLen);
    stream->read(flags.data(), sizeof(uint8_t), flagLen);

    return len;
  }

}}}
