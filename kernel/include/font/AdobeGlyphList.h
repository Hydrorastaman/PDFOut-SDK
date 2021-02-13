#pragma once
#include <string>
#include <cstdint>

namespace kernel{ namespace font{

  /**
   * @brief Unicode sequence information
   */
  struct UnicodeInfo{
    uint8_t mCount;     /**< count of unicode symbols in the sequence */
    uint16_t mValue[4]; /**< unicode sequence */
  };

  /**
   * @brief Gets Unicode sequence by Unicode name
   */
  void getUnicodeByName(std::string const &uniName, UnicodeInfo &uniInfo);
}}
