#include <font/CFF/Operator.h>

#include <Exception.h>
#include <unordered_map>

namespace kernel{ namespace font{ namespace cff{
  struct OperatorInfo{
    Operator mOperator;
    std::vector<OperandType> mTypes;
  };

  static std::unordered_map<uint16_t, OperatorInfo> const operators = {
    {0, {Operator::version, {OperandType::SID}}},
    {1, {Operator::Notice, {OperandType::SID}}},
    {2, {Operator::FullName, {OperandType::SID}}},
    {3, {Operator::FamilyName, {OperandType::SID}}},
    {4, {Operator::Weight, {OperandType::SID}}},
    {5, {Operator::FontBBox, {OperandType::Array}}},
    {6, {Operator::BlueValues, {OperandType::Delta}}},
    {7, {Operator::OtherBlues, {OperandType::Delta}}},
    {8, {Operator::FamilyBlues, {OperandType::Delta}}},
    {9, {Operator::FamilyOtherBlues, {OperandType::Delta}}},
    {10, {Operator::StdHW, {OperandType::Integer}}},
    {11, {Operator::StdVW, {OperandType::Integer}}},
    {12 << 8 | 0, {Operator::Copyright, {OperandType::SID}}},
    {12 << 8 | 1, {Operator::isFixedPitch, {OperandType::Boolean}}},
    {12 << 8 | 2, {Operator::ItalicAngle, {OperandType::Float}}},
    {12 << 8 | 3, {Operator::UnderlinePosition, {OperandType::Integer}}},
    {12 << 8 | 4, {Operator::UnderlineThickness, {OperandType::Integer}}},
    {12 << 8 | 5, {Operator::PaintType, {OperandType::Integer}}},
    {12 << 8 | 6, {Operator::CharstringType, {OperandType::Integer}}},
    {12 << 8 | 7, {Operator::FontMatrix, {OperandType::Array}}},
    {12 << 8 | 8, {Operator::StrokeWidth, {OperandType::Integer}}},
    {12 << 8 | 9, {Operator::BlueScale, {OperandType::Float}}},
    {12 << 8 | 10, {Operator::BlueShift, {OperandType::Integer}}},
    {12 << 8 | 11, {Operator::BlueFuzz, {OperandType::Integer}}},
    {12 << 8 | 12, {Operator::StemSnapH, {OperandType::Delta}}},
    {12 << 8 | 13, {Operator::StemSnapV, {OperandType::Delta}}},
    {12 << 8 | 14, {Operator::ForceBold, {OperandType::Boolean}}},
    {12 << 8 | 17, {Operator::LanguageGroup, {OperandType::Integer}}},
    {12 << 8 | 18, {Operator::ExpansionFactor, {OperandType::Float}}},
    {12 << 8 | 19, {Operator::initialRandomSeed, {OperandType::Integer}}},
    {12 << 8 | 20, {Operator::SyntheticBase, {OperandType::Integer}}},
    {12 << 8 | 21, {Operator::PostScript, {OperandType::SID}}},
    {12 << 8 | 22, {Operator::BaseFontName, {OperandType::SID}}},
    {12 << 8 | 23, {Operator::BaseFontBlend, {OperandType::Delta}}},
    {12 << 8 | 31, {Operator::ROS, {OperandType::SID, OperandType::SID, OperandType::Integer}}},
    {12 << 8 | 30, {Operator::CIDFontVersion, {OperandType::Integer}}},
    {12 << 8 | 32, {Operator::CIDFontRevision, {OperandType::Integer}}},
    {12 << 8 | 33, {Operator::CIDFontType, {OperandType::Integer}}},
    {12 << 8 | 34, {Operator::CIDCount, {OperandType::Integer}}},
    {12 << 8 | 35, {Operator::UIDBase, {OperandType::Integer}}},
    {12 << 8 | 36, {Operator::FDArray, {OperandType::Integer}}},
    {12 << 8 | 37, {Operator::FDSelect, {OperandType::Integer}}},
    {12 << 8 | 38, {Operator::FontName, {OperandType::SID}}},
    {13, {Operator::UniqueID, {OperandType::Integer}}},
    {14, {Operator::XUID, {OperandType::Array}}},
    {15, {Operator::charset, {OperandType::Integer}}},
    {16, {Operator::Encoding, {OperandType::Integer}}},
    {17, {Operator::CharStrings, {OperandType::Integer}}},
    {18, {Operator::Private, {OperandType::Integer, OperandType::Integer}}},
    {19, {Operator::Subrs, {OperandType::Integer}}},
    {20, {Operator::defaultWidthX, {OperandType::Integer}}},
    {21, {Operator::nominalWidthX, {OperandType::Integer}}}
  };

  bool isOperator(uint16_t index){
    auto iter = operators.find(index);
    if (iter != std::end(operators))
      return true;

    return false;
  }

  Operator getOperator(uint16_t index){
    auto iter = operators.find(index);
    if (iter != std::end(operators))
      return iter->second.mOperator;

    RUNTIME_EXCEPTION("Invalid operator index");
  }

  uint32_t getOperandsCount(Operator op){
    return 0;
  }

  std::vector<OperandType> const &getOperandsType(uint16_t index){
    auto iter = operators.find(index);
    if (iter != std::end(operators))
      return iter->second.mTypes;

    RUNTIME_EXCEPTION("Invalid operator index");
  }

}}}
