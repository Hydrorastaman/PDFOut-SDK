#pragma once

#include <font/AdobeGlyphList.h>

namespace kernel{ namespace font{

  void getUnicodeInMacintoshOrder(uint16_t glyphId, UnicodeInfo &uniInfo);
  std::string getGlyphNameInMacintoshOrder(uint16_t glyphId);

}}
