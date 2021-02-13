#include <font/CFF/CffDictionary.h>

#include <Exception.h>
#include <font/CFF/CffIndex.h>
#include <font/CFF/StandardSID.h>

namespace kernel{ namespace font{ namespace cff{

  Dictionary::Dictionary(pdfout::InputStream *stream)
    : mDictionary(){

    Index const index(stream);
    for (std::size_t i = 0, size = index.getSize(); i < size; ++i){
      auto const &source = index[i];
      unpack(source.data(), source.size(), mDictionary[i]);
    }
  }

  Dictionary::~Dictionary(){
  }

  std::size_t Dictionary::getSize() const noexcept{
    return mDictionary.size();
  }

  bool Dictionary::hasOperator(std::size_t index, Operator op) const noexcept{
    auto dict = mDictionary.find(index);
    if (dict == std::end(mDictionary))
      return false;

    auto value = dict->second.find(op);
    if (value == std::end(dict->second))
      return false;

    return true;
  }

  std::vector<Operand> const &Dictionary::getOperands(std::size_t index, Operator op) const{
    auto dict = mDictionary.find(index);
    if (dict == std::end(mDictionary))
      RUNTIME_EXCEPTION("Out of range");

    auto value = dict->second.find(op);
    if (value == std::end(dict->second))
      RUNTIME_EXCEPTION("Operator not found");

    return value->second;
  }

  void Dictionary::unpack(uint8_t const *source, std::size_t length, DictionaryData &dictionary){
    uint8_t const *ptr = source;

    auto isOperandArray = [](uint16_t operand)->bool{
      auto const &operands = getOperandsType(operand);
      auto const type = operands[0];
      return type == OperandType::Array || type == OperandType::Delta;
    };

    std::vector<Operand> params;

    while (ptr < source + length){
      uint16_t b0 = ptr[0];
      if (b0 == 12){
        ++ptr;
        b0 = b0 << 8 | ptr[0];
      }

      if (isOperator(b0)){
        Operator const op = getOperator(b0);
        bool const isArray = isOperandArray(b0);
        auto &operands = dictionary[op];

        if (isArray){
          std::vector<float> vec;
          for (auto const &elem : params){
            vec.push_back((float) elem);
          }

          operands.push_back(Operand(vec));
        }
        else{
          operands = std::move(params);
        }

        params.clear();
        ++ptr;
      }
      else if (b0 == 28 || b0 == 29 || b0 >= 32 && b0 <= 254){
        int32_t number = 0;
        ptr += readIntegerOperand(ptr, number);
        params.push_back(Operand(number));
      }
      else if (b0 == 30){
        float number = 0.f;
        ptr += readRealOperand(ptr, number);
        params.push_back(Operand(number));
      }
      else{
        RUNTIME_EXCEPTION("Invalid data format");
      }
    }
  }

  std::size_t Dictionary::readIntegerOperand(uint8_t const *source, int32_t &value) const{
    int32_t const b0 = (int32_t) source[0];

    if (b0 >= 32 && b0 <= 246){
      value = b0 - 139;
      return 1;
    }
    else if (b0 >= 247 && b0 <= 250){
      int32_t const b1 = (int32_t) source[1];

      value = (b0 - 247) * 256 + b1 + 108;
      return 2;
    }
    else if (b0 >= 251 && b0 <= 254){
      int32_t const b1 = (int32_t) source[1];
      int32_t const b2 = (int32_t) source[2];

      value = -(b0 - 251) * 256 - b1 - 108;
      return 3;
    }
    else if (b0 == 28){
      int32_t const b1 = (int32_t) source[1];
      int32_t const b2 = (int32_t) source[2];

      value = b1 << 8 | b2;
      return 3;
    }
    else if (b0 == 29){
      int32_t const b1 = (int32_t) source[1];
      int32_t const b2 = (int32_t) source[2];
      int32_t const b3 = (int32_t) source[3];
      int32_t const b4 = (int32_t) source[4];

      value = b1 << 24 | b2 << 16 | b3 << 8 | b4;
      return 5;
    }

    return 0;
  }

  std::size_t Dictionary::readRealOperand(uint8_t const *source, float &value) const{
    uint8_t const b0 = source[0];
    if (b0 != 30)
      return 0;

    auto nibbleToString = [](uint8_t nibble)->std::string{
      switch (nibble){
        case 0x0: return "0";
        case 0x1: return "1";
        case 0x2: return "2";
        case 0x3: return "3";
        case 0x4: return "4";
        case 0x5: return "5";
        case 0x6: return "6";
        case 0x7: return "7";
        case 0x8: return "8";
        case 0x9: return "9";
        case 0xA: return ".";
        case 0xB: return "E";
        case 0xC: return "E-";
        case 0xD: return "";
        case 0xE: return "-";
        case 0xF: return "";
        default: return "";
      }
    };

    std::size_t offset = 1;
    std::string str = "";
    while (true){
      uint8_t const n0 = source[offset] >> 4;
      uint8_t const n1 = source[offset] & 0x0F;

      str += nibbleToString(n0) + nibbleToString(n1);
      if (n0 == 0xF || n1 == 0xF)
        break;

      ++offset;
    }

    char *end = nullptr;
    value = strtof(&str[0], &end);
    return offset + 1;
  }

}}}
