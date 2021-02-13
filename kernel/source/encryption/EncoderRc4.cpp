#include <encryption/EncoderRc4.h>

#include <Exception.h>
#include <InternalTools.h>
#include <filter/FilterRc4.h>

#include <object/Object.h>
#include <object/ObjectIndirectReference.h>

// Crypto++
#include <md5.h>

using namespace CryptoPP;

namespace kernel{ namespace encryption{

  EncoderRc4::EncoderRc4(std::vector<uint8_t> const &key)
    : Encoder(key){
  }

  EncoderRc4::~EncoderRc4(void) {}

  std::unique_ptr<kernel::filter::Filter> EncoderRc4::makeEncoder(Object const *obj) const{
    if (!obj || !obj->isIndirectable())
      RUNTIME_EXCEPTION("Invalid object pointer");

    auto ref = obj->getReference();
    if (!ref)
      RUNTIME_EXCEPTION("Invalid object indirect reference");

    uint32_t const num = ref->getNumber();
    uint16_t const gen = ref->getGeneration();
    std::size_t const extSize = 5;
    uint8_t ext[extSize] = {num & 0xFF, (num >> 8) & 0xFF, (num >> 16) & 0xFF, gen & 0xFF, (gen >> 8) & 0xFF};

    MD5 hash;
    std::vector<uint8_t> const &key = getKey();
    hash.Update(key.data(), key.size());
    hash.Update(ext, extSize);

    std::vector<uint8_t> md5(CryptoPP::MD5::DIGESTSIZE, 0);
    hash.Final(md5.data());

    std::size_t const length = std::min(key.size() + 5, (std::size_t) 16);
    return std::make_unique<filter::FilterRc4>(md5.data(), length);
  }

}}
