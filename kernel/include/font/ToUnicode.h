#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace kernel{ namespace font{

  /**
   * @brief Extracts Unicode values sequese from glyph name
   */
  void toUnicode(uint16_t glyphId, std::string const &glyphName, std::vector<uint32_t> &unicode);

}}
