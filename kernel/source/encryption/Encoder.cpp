#include <encryption/Encoder.h>

namespace kernel{ namespace encryption{

  Encoder::Encoder(std::vector<uint8_t> const &key)
    : mKey(key){
  }

  Encoder::~Encoder(void) {}

  std::vector<uint8_t> const &Encoder::getKey(void) const{
    return mKey;
  }

}}
