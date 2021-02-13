#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <Types.h>
#include <Color.h>

#ifdef _MSC_VER
  #define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#elif __GNUC__
  #define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#define MAX_ULONG_LENGTH 12
#define MAX_FLOAT_LENGTH (3 + FLT_MANT_DIG - FLT_MIN_EXP)

namespace pdfout{
  class InputStream;
  class OutputStream;
}

namespace kernel{

  enum Endian{
    EndianLittle,
    EndianBig
  };

  Endian getEndian(void);

  void swapByteOrder(uint16_t &value);
  void swapByteOrder(int16_t &value);
  void swapByteOrder(uint32_t &value);
  void swapByteOrder(int32_t &value);
  void swapByteOrder(uint64_t &value);
  void swapByteOrder(int64_t &value);

  template<typename T>
  void swapByteOrder(std::vector<T> &values){
    for (auto &elem: values)
      swapByteOrder(elem);
  }

  template<typename T>
  void swapByteOrder(T *ptr, std::size_t count){
    for (std::size_t i = 0; i < count; ++i)
      swapByteOrder(ptr[i]);
  }

  template <typename T>
  T getPadTo4(T value){
    return (4 - value % 4) % 4;
  }

  template <typename T>
  T getMaxPowOf2LessThan(T value){
    T pow = 0;
    while (value >>= 1)
      ++pow;

    return pow;
  }

  /**
   * @brief Gets MD5 hash
   */
  std::string getMD5(void const *data, std::size_t size);
  void getMD5(void const *data, std::size_t size, std::vector<uint8_t> &md5);

  /**
   * @brief Gets SHA256 hash
   * @return Hex string
   */
  std::string getSHA256(void const *data, std::size_t size);
  std::string getSHA256(pdfout::InputStream *istream);
  void getSHA256(void const *data, std::size_t size, std::vector<uint8_t> &sha256);

  void writeEOL(pdfout::OutputStream *stream);

  /**
   * @brief Generates UUID
   */
  std::vector<uint8_t> getUIID(void);

  /**
   * @brief Generates random numbers
   */
  void fillRandom(uint8_t *out, std::size_t count);
  void fillRandom(std::vector<uint8_t> &out);

  /**
   * @brief Converts ::Color to array of float
   */
  std::vector<float> colorToArray(pdfout::Color const &color);

  /**
   * @brief Serializes float value to string
   */
  void floatToString(char *ptr, float value);
}
