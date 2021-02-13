#include <InternalTools.h>

// Crypto++
#include <md5.h>
#include <hex.h>
#include <sha.h>
#include <osrng.h>
#include <randpool.h>

#include <stream/InputStream.h>
#include <stream/OutputStream.h>

namespace{
  uint8_t const EOL[2] = {0x0D, 0x0A};
}

using namespace pdfout;

namespace kernel{

  Endian getEndian(void){
    union{
      uint32_t mInt;
      char mChar[4];
    } value = {0x01020304};

    return (value.mChar[0] == 1) ? EndianBig : EndianLittle; 
  }

  void swapByteOrder(uint16_t &value) {value = ((value >> 8) & 0x00FF) | ((value << 8) & 0xFF00);}

  void swapByteOrder(int16_t &value) {value = ((value >> 8) & 0x00FF) | ((value << 8) & 0xFF00);}

  void swapByteOrder(uint32_t &value){
    value = ((value >> 24) & 0x000000FF) |
            ((value << 8) & 0x00FF0000) |
            ((value >> 8) & 0x0000FF00) |
            ((value << 24) & 0xFF000000);
  }

  void swapByteOrder(int32_t &value){
    value = ((value >> 24) & 0x000000FF) |
            ((value << 8) & 0x00FF0000) |
            ((value >> 8) & 0x0000FF00) |
            ((value << 24) & 0xFF000000);
  }

  void swapByteOrder(uint64_t &value){
    value = ((value >> 56) & 0x00000000000000FF) |
            ((value << 40) & 0x00FF000000000000) |
            ((value << 24) & 0x0000FF0000000000) |
            ((value << 8)  & 0x000000FF00000000) |
            ((value >> 8)  & 0x00000000FF000000) |
            ((value >> 24) & 0x0000000000FF0000) |
            ((value >> 40) & 0x000000000000FF00) |
            ((value << 56) & 0xFF00000000000000);
  }

  void swapByteOrder(int64_t &value){
    value = ((value >> 56) & 0x00000000000000FF) |
            ((value << 40) & 0x00FF000000000000) |
            ((value << 24) & 0x0000FF0000000000) |
            ((value << 8)  & 0x000000FF00000000) |
            ((value >> 8)  & 0x00000000FF000000) |
            ((value >> 24) & 0x0000000000FF0000) |
            ((value >> 40) & 0x000000000000FF00) |
            ((value << 56) & 0xFF00000000000000);
  }

  std::string getMD5(void const *data, std::size_t size){
    byte digest[CryptoPP::MD5::DIGESTSIZE] = {0};
    CryptoPP::MD5 hash;
    hash.CalculateDigest(digest, (byte *) data, size);

    CryptoPP::HexEncoder encoder;
    std::string output;
    encoder.Attach(new CryptoPP::StringSink(output));
    encoder.Put(digest, sizeof(digest));
    encoder.MessageEnd();

    return output;
  }

  void getMD5(void const *data, std::size_t size, std::vector<uint8_t> &md5){
    md5.resize(CryptoPP::MD5::DIGESTSIZE, 0);
    CryptoPP::MD5 hash;
    hash.CalculateDigest(md5.data(), (byte *) data, size);
  }

  std::string getSHA256(InputStream *istream){
    CryptoPP::SHA256 sha;

    uint64_t currPos = istream->tellg();
    istream->seekg(0);
    std::size_t const buffSize = 1048576; // 1Mb
    std::vector<uint8_t> buff(buffSize);
    uint64_t read = 0;
    while (read = istream->read(buff.data(), sizeof(uint8_t), buffSize))
      sha.Update(buff.data(), read * sizeof(uint8_t));

    istream->seekg(currPos);

    uint8_t hash[CryptoPP::SHA256::DIGESTSIZE] = {0};
    sha.Final(hash);

    std::string hexResult = "";
    CryptoPP::StringSource(hash, CryptoPP::SHA256::DIGESTSIZE, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hexResult)));
    return hexResult;
  }

  std::string getSHA256(void const *data, std::size_t size){
    CryptoPP::SHA256 sha;
    sha.Update((byte *) data, size);
    uint8_t hash[CryptoPP::SHA256::DIGESTSIZE] = {0};
    sha.Final(hash);

    std::string hexResult = "";
    CryptoPP::StringSource(hash, CryptoPP::SHA256::DIGESTSIZE, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hexResult)));
    return hexResult;
  }

  void getSHA256(void const *data, std::size_t size, std::vector<uint8_t> &sha256){
    CryptoPP::SHA256 sha;
    sha.Update((byte *) data, size);
    sha256.resize(CryptoPP::SHA256::DIGESTSIZE, 0);
    sha.Final(sha256.data());
  }

  void writeEOL(pdfout::OutputStream *stream){
    stream->write(EOL, sizeof(uint8_t), 2);
  }

  std::vector<uint8_t> getUIID(void){
    using namespace CryptoPP;

    SecByteBlock seed(16);
    OS_GenerateRandomBlock(false, seed, seed.size());

    RandomPool rng;
    rng.IncorporateEntropy(seed, seed.size());

    std::vector<uint8_t> uuid(seed.size());
    MD5 hash;
    hash.CalculateDigest(uuid.data(), (byte *) seed.data() , seed.size());

    return uuid;
  }

  void fillRandom(uint8_t *out, std::size_t count){
    using namespace CryptoPP;

    OS_GenerateRandomBlock(false, out, count);

    RandomPool rng;
    rng.IncorporateEntropy(out, count);
  }

  void fillRandom(std::vector<uint8_t> &out){
    fillRandom(out.data(), out.size());
  }

  std::vector<float> colorToArray(pdfout::Color const &color){
    using namespace pdfout;

    switch (color.mType){
      case ColorTypeRGB:
        return {color.mValue.mRGB.mRed, color.mValue.mRGB.mGreen, color.mValue.mRGB.mBlue};

      case ColorTypeCMYK:
        return {color.mValue.mCMYK.mCyan, color.mValue.mCMYK.mMagenta, color.mValue.mCMYK.mYellow, color.mValue.mCMYK.mBlack};

      case ColorTypeGray:
        return {color.mValue.mGray.mGray};

      case ColorTypeIndex:
        return {(float) color.mValue.mIndex.mIndex};
    }

    return {};
  }

  void floatToString(char *ptr, float value){
    float integral = 0.f;
    float fract = std::modf(value, &integral);

    if (!fract) {
      sprintf(ptr, "%i ", (int32_t) integral);
      return;
    }

    fract = std::modf(value * 1000, &integral);
    if (!fract) {
      sprintf(ptr, "%.3f ", value);
      return;
    }

    fract = std::modf(value * 1000000, &integral);
    if (integral) {
      sprintf(ptr, "%.6f ", value);
      return;
    }

    sprintf(ptr, "%.9f ", value);
  }
}
