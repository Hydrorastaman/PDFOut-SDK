#pragma once

#include <vector>
#include <cstdint>
#include <memory>

#include <stream/InputStream.h>
#include <stream/OutputStream.h>
#include <font/TrueType/Glyph.h>

namespace kernel{ namespace font{ namespace truetype{

  class CompoundGlyph : public Glyph{
  private:
    struct CompoundData{
      uint16_t mFlags;
      uint16_t mGlyphIndex;
      std::vector<uint8_t> mAdditionalData;
    };

  public:
    CompoundGlyph(pdfout::InputStream *stream, GlyphDescription const &description);
    ~CompoundGlyph(void);

    void save(pdfout::OutputStream *stream) const;
    std::unique_ptr<Glyph> clone(std::vector<uint16_t> const &gids) const;

    std::vector<uint16_t> getComponents(void) const;
    GlyphDescription const &getGlyphDescription(void) const;

  private:
    void load(pdfout::InputStream *stream, GlyphDescription const &description);

  private:
    //CompoundGlyph(CompoundGlyph const &obj);
    CompoundGlyph &operator=(CompoundGlyph const &) = delete;

  private:
    GlyphDescription mDescription;
    std::vector<CompoundData> mData;
    std::vector<uint8_t> mInstructions;
  };

}}}
