#pragma once

#include <vector>

namespace kernel{ namespace font{ namespace cff{

  /**
   * @brief Operand types
   */
  enum class OperandType{
    Integer, /**< Integer number */
    Float,   /**< Float number */
    SID,     /**< uint16_t String ID */
    Boolean, /**< Integer type with values 0 (false) and 1 (true)  */
    Array,   /**< One or more numbers */
    Delta    /**< A number or a delta-encoded array of numbers */
  };

  struct Operand{
    OperandType mType;
    int32_t mInteger;
    float mFloat;
    bool mBoolean;
    std::vector<float> mArray;

    operator float() const noexcept{
      return (mType == OperandType::Float) ? mFloat : mInteger;
    }

    operator bool() const noexcept{
      return mBoolean;
    }

    operator int32_t() const noexcept{
      return mInteger;
    }

    operator uint16_t() const noexcept{
      return (uint16_t) mInteger;
    }

    operator std::vector<float>() const noexcept{
      return mArray;
    }

    Operand(int32_t number)
      : mType(OperandType::Integer),
      mInteger(number),
      mFloat(0.f),
      mBoolean(false){
    }

    Operand(float number)
      : mType(OperandType::Float),
      mInteger(0),
      mFloat(number),
      mBoolean(false){
    }

    Operand(bool boolean)
      : mType(OperandType::Boolean),
      mInteger(0),
      mFloat(0.f),
      mBoolean(boolean){
    }

    Operand(std::vector<float> const &array, bool isDelta = false)
      : mType(isDelta ? OperandType::Delta : OperandType::Array),
      mInteger(0),
      mFloat(0.f),
      mBoolean(false),
      mArray(array){
    }
  };

}}}
