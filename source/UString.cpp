#include <UString.h>

#include <ToString.h>
#include <Exception.h>
#include <InternalTools.h>

#define LIBIDN_STATIC
#include <stringprep.h>

#define USING_STATIC_LIBICONV 1
#include <iconv.h>

#include "unicode/UBA63.h"

namespace{
  class Iconv{
  public:
    Iconv(iconv_t handle) : mHandle(handle) {}
    ~Iconv(void) {if (mHandle) iconv_close(mHandle);}

    operator iconv_t() const {return mHandle;}

  private:
    Iconv(Iconv const &) = delete;
    Iconv &operator=(Iconv const &) = delete;

  private:
    iconv_t mHandle;
  };
}

namespace pdfout{

  UString::UString(char const *source)
    : mEncoding(gDefaultEncoding),
    mData(nullptr),
    mSize(0){

    initialize((uint8_t const *) source, strlen(source) + 1);
  }

  UString::UString(char const *source, Encoding encoding)
    : mEncoding(encoding),
    mData(nullptr),
    mSize(0){

    initialize((uint8_t const *) source, strlen(source) + 1);
  }

  UString::UString(wchar_t const *source, Encoding encoding)
    : mEncoding(encoding),
    mData(nullptr),
    mSize(0){

    initialize((uint8_t const *) source, (wcslen(source) + 1) * sizeof(wchar_t));
  }

  UString::UString(uint8_t const *source, std::size_t length, Encoding encoding)
    : mEncoding(encoding),
    mData(nullptr),
    mSize(0){

    initialize(source, length);
  }

  UString::~UString(void){
    release();
  }

  UString::UString(UString const &rhs)
    : mEncoding(rhs.mEncoding),
    mData(nullptr),
    mSize(0){

    initialize(rhs.mData, rhs.mSize);
  }

  UString::UString(UString &&rhs)
    : mEncoding(rhs.mEncoding),
    mData(std::move(rhs.mData)),
    mSize(std::move(rhs.mSize)){

    rhs.mData = nullptr;
    rhs.mSize = 0;
  }

  UString &UString::operator=(UString const &rhs){
    if (this != &rhs){
      release();
      mEncoding = rhs.mEncoding;
      initialize(rhs.mData, rhs.mSize);
    }

    return *this;
  }

  UString &UString::operator=(UString &&rhs){
    if (this != &rhs){
      release();
      mEncoding = rhs.mEncoding;
      mData = std::move(rhs.mData);
      mSize = rhs.mSize;

      rhs.mData = nullptr;
      rhs.mSize = 0;
    }

    return *this;
  }

  UString &UString::operator+=(UString const &rhs){
    if (this != &rhs){
      UString const nullstr = UString("\0", Encoding_CHAR).encode(mEncoding);

      if (mEncoding != rhs.mEncoding){
        UString const encoded = rhs.encode(mEncoding);
        std::size_t const size = getSize() + encoded.getSize() - nullstr.getSize();
        std::size_t const pos = getSize() - nullstr.getSize();
        std::vector<uint8_t> concatenated(size, 0);
        memcpy(concatenated.data(), getData(), getSize());
        memcpy(&concatenated.data()[pos], encoded.getData(), encoded.getSize());
        release();
        initialize(concatenated.data(), concatenated.size());
      }
      else{
        std::size_t const size = getSize() + rhs.getSize() - nullstr.getSize();
        std::size_t const pos = getSize() - nullstr.getSize();
        std::vector<uint8_t> concatenated(size, 0);
        memcpy(concatenated.data(), getData(), getSize());
        memcpy(&concatenated.data()[pos], rhs.getData(), rhs.getSize());
        release();
        initialize(concatenated.data(), concatenated.size());
      }
    }

    return *this;
  }

  Encoding UString::getEncoding(void) const{
    return mEncoding;
  }

  uint8_t const *UString::getData(void) const{
    return mData;
  }

  std::size_t UString::getSize(void) const{
    return mSize;
  }

  UString UString::encode(Encoding encoding) const{
    if (mEncoding == encoding)
      return UString(getData(), getSize(), mEncoding);

    std::vector<uint8_t> encodedStr;
    if (convertEncoding(getData(), getSize(), mEncoding, encoding, encodedStr))
      return UString(encodedStr.data(), encodedStr.size(), encoding);

    return UString((uint8_t *) "", 0, encoding);
  }

  UString UString::saslPrepProfile(void) const{
    UString const utf8Str = encode(Encoding_UTF8);

    char *rawOut = 0;
    int rc = stringprep_profile((char const *) utf8Str.getData(), &rawOut, "SASLprep", STRINGPREP_NO_UNASSIGNED);
    if (rc != STRINGPREP_OK)
      RUNTIME_EXCEPTION("Failed to SASLprep for input string.");
 
    if (rawOut){
      UString out((uint8_t const *) rawOut, strlen(rawOut), Encoding_UTF8);
      free(rawOut);
      return out;
    }

    return UString((uint8_t *) "", 0, Encoding_UTF8);
  }

  UString UString::applyUnicodeBidiAlgorithm(std::vector<uint32_t> &transpositions) const {
    using unicode::UBA;
    using unicode::UBA63;

    std::unique_ptr<UBA> uba = std::make_unique<UBA63>();
    UString utf32 = encode(Encoding::Encoding_UTF32LE);
    std::size_t const utf32Length = utf32.mSize / sizeof(uint32_t);
    if (utf32Length != transpositions.size())
      transpositions.resize(utf32Length);

    std::vector<uint32_t> result = uba->resolve((uint32_t const *) utf32.mData, transpositions.data(), utf32Length);
    memcpy(utf32.mData, result.data(), utf32.mSize);

    return utf32.encode(Encoding::Encoding_WCHAR_T);
  }

  void UString::setDefaultEncoding(Encoding encoding){
    gDefaultEncoding = encoding;
  }

  Encoding UString::getDefaultEncoding(void){
    return gDefaultEncoding;
  }

  void UString::initialize(uint8_t const *source, std::size_t lenght){
    if (lenght){
      mSize = lenght;
      mData = new uint8_t[mSize];
      memset(mData, 0, mSize);
      memcpy(mData, source, mSize);
    }
  }

  void UString::release(void){
    if (mData){
      delete[] mData;
      mSize = 0;
    }
  }

  bool UString::convertEncoding(void const *data, std::size_t size, Encoding from, Encoding to, std::vector<uint8_t> &result) const{
    if (!data)
      return false;

    Iconv convert_hnd(iconv_open(kernel::toString(to), kernel::toString(from)));
    if (convert_hnd == (iconv_t)(-1))
      return false;
 
    char const *in = (char const *)(data);
    std::size_t allocated_size = size * 6;
    std::vector<char> buff(allocated_size);
    char *out = buff.data();
    std::size_t reverse_size = allocated_size;
 
    if (iconv(convert_hnd, &in, &size, &out, &reverse_size) == (std::size_t) -1){
      int errcode = errno;
      return false;
    }

    result.assign(buff.data(), buff.data() + allocated_size - reverse_size);
    return true;
  }

  UString operator+(pdfout::UString const &lhs, UString const &rhs){
    UString result = lhs;
    result += rhs;
    return result;
  }

  bool operator==(pdfout::UString const &lhs, UString const &rhs){
    if (lhs.getEncoding() == rhs.getEncoding())
      return lhs.getSize() == rhs.getSize() && !memcmp(lhs.getData(), rhs.getData(), lhs.getSize());
    else{
      UString const tmp = rhs.encode(lhs.getEncoding());
      return lhs.getSize() == tmp.getSize() && !memcmp(lhs.getData(), tmp.getData(), lhs.getSize());
    }

    return false;
  }

  bool operator!=(pdfout::UString const &lhs, UString const &rhs){
    return !(lhs == rhs);
  }

  Encoding UString::gDefaultEncoding = Encoding_ASCII;
}
