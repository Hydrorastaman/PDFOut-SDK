#pragma once

#include <vector>
#include <cstdint>
#include <memory>

#include <stream/InputStream.h>
#include <stream/OutputStream.h>
#include <font/TrueType/Glyph.h>

namespace kernel{ namespace font{ namespace truetype{

  class SimpleGlyph : public Glyph{
  public:
    SimpleGlyph(pdfout::InputStream *stream, GlyphDescription const &description);
    ~SimpleGlyph(void);

    void save(pdfout::OutputStream *stream) const;
    std::unique_ptr<Glyph> clone(std::vector<uint16_t> const &gids) const;

    GlyphDescription const &getGlyphDescription(void) const;
    uint16_t getMaxPointsNum(void) const;
    uint16_t getInstructionLength(void) const;

  private:
    void load(pdfout::InputStream *stream, GlyphDescription const &description);
    uint16_t readFlags(pdfout::InputStream *stream, std::vector<uint8_t> &flags, uint16_t points) const;

  private:
    //SimpleGlyph(SimpleGlyph const &obj);
    SimpleGlyph &operator=(SimpleGlyph const &) = delete;

  private:
    GlyphDescription mDescription;
    std::vector<uint16_t> mEndPtsOfContours;
    uint16_t mInstructionLength;
    std::vector<uint8_t> mInstructions;
    std::vector<uint8_t> mFlags;
    std::vector<uint8_t> mCoordinates; /**< Attention: byte order for two-byte values are not swapped */
  };

}}}
