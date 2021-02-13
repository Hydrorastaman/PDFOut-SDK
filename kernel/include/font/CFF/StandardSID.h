#pragma once

#include <string>
#include <stdint.h>

namespace kernel{ namespace font{ namespace cff{

  uint16_t getStandardSIDCount();
  std::string getStandardSID(uint16_t index);

}}}
