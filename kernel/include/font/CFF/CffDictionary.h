#pragma once

#include <map>
#include <unordered_map>

#include <Exception.h>
#include <font/CFF/Operand.h>
#include <font/CFF/Operator.h>

namespace pdfout{
  class InputStream;
}

namespace kernel{ namespace font{ namespace cff{

  struct SizeOffset{
    int32_t mSize;
    int32_t mOffset;
  };

  using SID = uint16_t;
  static const SID InvalidSID = (SID) -1;
  static const int32_t InvalidID = -std::numeric_limits<int32_t>::max();
  static const int32_t InvalidIndex = -std::numeric_limits<int32_t>::max();

  class Dictionary{
  private:
    using DictionaryData = std::unordered_map<Operator, std::vector<Operand>>;

  protected:
    Dictionary(pdfout::InputStream *stream);

  public:
    virtual ~Dictionary();
    Dictionary(Dictionary const &) = delete;
    Dictionary &operator=(Dictionary const &) = delete;

    std::size_t getSize() const noexcept;

  protected:
    bool hasOperator(std::size_t index, Operator op) const noexcept;
    std::vector<Operand> const &getOperands(std::size_t index, Operator op) const;

    template <typename T>
    T getFirstOperandOrDefault(std::size_t index, Operator op, T const &default) const noexcept{
      if (!hasOperator(index, op))
        return default;

      auto const &tmp = getOperands(index, op);
      T const value = (T) tmp[0];
      return value;
    }

    template <typename T>
    std::vector<T> getFirstOperandOrDefault(std::size_t index, Operator op, std::vector<T> const &default) const noexcept{
      if (!hasOperator(index, op))
        return default;

      auto const &tmp = getOperands(index, op);
      if (tmp[0].mType != OperandType::Array && tmp[0].mType != OperandType::Delta)
        return default;

      std::vector<T> result;
      result.reserve(tmp[0].mArray.size());

      for (auto const &elem : tmp[0].mArray){
        result.emplace_back((T) elem);
      }

      return result;
    }

  private:
    void unpack(uint8_t const *source, std::size_t length, DictionaryData &dictionary);
    std::size_t readIntegerOperand(uint8_t const *source, int32_t &value) const;
    std::size_t readRealOperand(uint8_t const *source, float &value) const;

  private:
    std::map<uint32_t, DictionaryData> mDictionary;
  };

}}}
