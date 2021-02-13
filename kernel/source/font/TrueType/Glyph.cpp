#include <font/TrueType/Glyph.h>

namespace kernel{ namespace font{ namespace truetype{

  Glyph::Glyph(GlyphType type)
    : mType(type){
  }

  Glyph::Glyph(Glyph const &obj)
    : mType(obj.mType){
  }

  Glyph::~Glyph(void){
  }

  GlyphType Glyph::getType(void) const{
    return mType;
  }

}}}
