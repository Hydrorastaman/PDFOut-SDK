#include <encryption/EncoderAes2.h>

#include <Exception.h>
#include <InternalTools.h>
#include <object/Object.h>
#include <filter/FilterAes.h>
#include <object/ObjectIndirectReference.h>

// Crypto++
#include <md5.h>

using namespace CryptoPP;

namespace kernel{ namespace encryption{

  EncoderAes2::EncoderAes2(std::vector<uint8_t> const &key)
    : Encoder(key){
  }

  EncoderAes2::~EncoderAes2(void) {}

  std::unique_ptr<kernel::filter::Filter> EncoderAes2::makeEncoder(Object const *obj) const{
    if (!obj || !obj->isIndirectable())
      RUNTIME_EXCEPTION("Invalid object pointer");

    auto ref = obj->getReference();
    if (!ref)
      RUNTIME_EXCEPTION("Invalid object indirect reference");

    uint32_t const num = ref->getNumber();
    uint16_t const gen = ref->getGeneration();
    std::size_t const extSize = 9;
    uint8_t ext[extSize] = {num & 0xFF, (num >> 8) & 0xFF, (num >> 16) & 0xFF, gen & 0xFF, (gen >> 8) & 0xFF, 0x73, 0x41, 0x6C, 0x54};

    MD5 hash;
    std::vector<uint8_t> const &key = getKey();
    hash.Update(key.data(), key.size());
    hash.Update(ext, extSize);

    std::vector<uint8_t> md5(CryptoPP::MD5::DIGESTSIZE, 0);
    hash.Final(md5.data());

    return std::make_unique<filter::FilterAes>(md5.data(), 16);
  }

}}
