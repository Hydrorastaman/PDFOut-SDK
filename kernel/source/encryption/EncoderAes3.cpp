#include <encryption/EncoderAes3.h>
#include <filter/FilterAes.h>

namespace kernel{ namespace encryption{

  EncoderAes3::EncoderAes3(std::vector<uint8_t> const &key)
    : Encoder(key){
  }

  EncoderAes3::~EncoderAes3(void) {}

  std::unique_ptr<kernel::filter::Filter> EncoderAes3::makeEncoder(Object const *obj) const{
    std::vector<uint8_t> const &key = getKey();
    return std::make_unique<filter::FilterAes>(key.data(), key.size());
  }

}}
