#include <font/CompositeFont.h>

#include <Exception.h>

#include <font/CID/FontCIDType0.h>
#include <font/CID/FontCIDType2.h>
#include <font/TrueType/FontTrueType.h>

namespace kernel{ namespace font{

  CompositeFont::CompositeFont(SimpleFont const *descendantFont, pdfout::WritingMode writingMode)
    : mDescendantFont(descendantFont),
    mWritingMode(writingMode){

    if (!mDescendantFont)
      RUNTIME_EXCEPTION("Invalid descendant font.");
  }

  CompositeFont::~CompositeFont(void) noexcept {}

  SimpleFont const *CompositeFont::getDescendantFont(void) const{
    return mDescendantFont;
  }

  pdfout::WritingMode CompositeFont::getWritingMode(void) const{
    return mWritingMode;
  }

  CMapName CompositeFont::getCMapName(void) const{
    return (getWritingMode() == pdfout::WritingModeHorizontal) ? CMapName_Identity_H : CMapName_Identity_V;
  }

  std::unique_ptr<CompositeFont> CompositeFont::createCompositeFont(SimpleFont const *descendantFont, pdfout::WritingMode writingMode){
    if (truetype::FontTrueType const *font = dynamic_cast<truetype::FontTrueType const *>(descendantFont))
      return std::make_unique<cid::FontCIDType2>(font, writingMode);

    return nullptr;
  }

}}
