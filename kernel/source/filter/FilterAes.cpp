#include <filter/FilterAes.h>

#include <InternalTools.h>
#include <stream/OutputStream.h>
#include <stream/InputStream.h>
#include <stream/MemoryOStream.h>
#include <stream/MemoryIStream.h>

// crypto++
#include <filters.h>

using namespace CryptoPP;

namespace kernel{ namespace filter{

  FilterAes::FilterAes(uint8_t const *key, std::size_t length)
    : Filter(),
    mEncryptor(),
    mDecryptor(),
    mKey(key, key + length){
  }

  FilterAes::FilterAes(std::vector<uint8_t> const &key, std::unique_ptr<Filter> attachment)
    : Filter(std::move(attachment)){
  }

  FilterAes::FilterAes(std::vector<uint8_t> const &key, Filter *attachment)
    : Filter(attachment){
  }

  FilterAes::~FilterAes(void) {}

  FilterAes *FilterAes::clone(void) const{
    return nullptr;
  }

  bool FilterAes::encode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream){
    if (!istream || !ostream)
      return false;

    std::vector<uint8_t> iv(CryptoPP::AES::BLOCKSIZE, 0);
    fillRandom(iv);
    mEncryptor.SetKeyWithIV(mKey.data(), mKey.size(), iv.data());

    ostream->write(iv.data(), sizeof(uint8_t), iv.size());

    uint32_t const buffSize = 4096;
    uint8_t buff[buffSize] = {0};
    uint64_t read = 0;

    StreamTransformationFilter ss(mEncryptor, nullptr, BlockPaddingSchemeDef::PKCS_PADDING);

    while (read = istream->read(&buff[0], sizeof(uint8_t), buffSize))
      ss.Put(buff, read);

    ss.MessageEnd();

    std::size_t const ready = ss.MaxRetrievable();
    std::vector<uint8_t> cipher(ready, 0);
    ss.Get(cipher.data(), ready);
    ostream->write(cipher.data(), sizeof(uint8_t), cipher.size());

    return true;
  }

  bool FilterAes::decode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream){
    if (!istream || !ostream)
      return false;

    std::vector<uint8_t> iv(CryptoPP::AES::BLOCKSIZE);
    istream->read(iv.data(), sizeof(uint8_t), iv.size());
    mDecryptor.SetKeyWithIV(mKey.data(), mKey.size(), iv.data());

    uint32_t const buffSize = CryptoPP::AES::BLOCKSIZE * 256;
    uint8_t buff[buffSize] = {0};
    uint64_t read = 0;

    StreamTransformationFilter ss(mDecryptor, nullptr, BlockPaddingSchemeDef::PKCS_PADDING);

    while (read = istream->read(&buff[0], sizeof(uint8_t), buffSize))
      ss.Put(buff, read);

    ss.MessageEnd();

    std::size_t const ready = ss.MaxRetrievable();
    std::vector<uint8_t> cipher(ready, 0);
    ss.Get(cipher.data(), ready);
    ostream->write(cipher.data(), sizeof(uint8_t), cipher.size());

    return true;
  }

}}
