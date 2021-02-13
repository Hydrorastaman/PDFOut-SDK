#pragma once

#include <string>
#include <vector>

#include <font/CFF/Operand.h>

namespace kernel{ namespace font{ namespace cff{

  enum class Operator{
    version            = 0,
    Notice             = 1,
    FullName           = 2,
    FamilyName         = 3,
    Weight             = 4,
    FontBBox           = 5,
    BlueValues         = 6,
    OtherBlues         = 7,
    FamilyBlues        = 8,
    FamilyOtherBlues   = 9,
    StdHW              = 10,
    StdVW              = 11,
    ESC                = 12, // escape byte for two-byte operators
    Copyright          = 12 << 8 | 0,
    isFixedPitch       = 12 << 8 | 1,
    ItalicAngle        = 12 << 8 | 2,
    UnderlinePosition  = 12 << 8 | 3,
    UnderlineThickness = 12 << 8 | 4,
    PaintType          = 12 << 8 | 5,
    CharstringType     = 12 << 8 | 6,
    FontMatrix         = 12 << 8 | 7,
    StrokeWidth        = 12 << 8 | 8,
    BlueScale          = 12 << 8 | 9,
    BlueShift          = 12 << 8 | 10,
    BlueFuzz           = 12 << 8 | 11,
    StemSnapH          = 12 << 8 | 12,
    StemSnapV          = 12 << 8 | 13,
    ForceBold          = 12 << 8 | 14,
    LanguageGroup      = 12 << 8 | 17,
    ExpansionFactor    = 12 << 8 | 18,
    initialRandomSeed  = 12 << 8 | 19,
    SyntheticBase      = 12 << 8 | 20,
    PostScript         = 12 << 8 | 21,
    BaseFontName       = 12 << 8 | 22,
    BaseFontBlend      = 12 << 8 | 23,
    ROS                = 12 << 8 | 31,
    CIDFontVersion     = 12 << 8 | 30,
    CIDFontRevision    = 12 << 8 | 32,
    CIDFontType        = 12 << 8 | 33,
    CIDCount           = 12 << 8 | 34,
    UIDBase            = 12 << 8 | 35,
    FDArray            = 12 << 8 | 36,
    FDSelect           = 12 << 8 | 37,
    FontName           = 12 << 8 | 38,
    UniqueID           = 13,
    XUID               = 14,
    charset            = 15,
    Encoding           = 16,
    CharStrings        = 17,
    Private            = 18,
    Subrs              = 19,
    defaultWidthX      = 20,
    nominalWidthX      = 21,
    shortint           = 28,
    longint            = 29,
    BCD                = 30
  };

  bool isOperator(uint16_t index);
  Operator getOperator(uint16_t index);
  uint32_t getOperandsCount(Operator op);
  std::vector<OperandType> const &getOperandsType(uint16_t index);

}}}
