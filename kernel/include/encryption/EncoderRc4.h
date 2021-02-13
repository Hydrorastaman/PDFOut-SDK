#pragma once

#include <encryption/Encoder.h>

namespace kernel{ namespace encryption{

  class EncoderRc4 : public Encoder{
  public:
    EncoderRc4(std::vector<uint8_t> const &key);
    ~EncoderRc4(void);

    std::unique_ptr<kernel::filter::Filter> makeEncoder(Object const *obj) const;

  private:
    EncoderRc4(EncoderRc4 const &) = delete;
    EncoderRc4 &operator=(EncoderRc4 const &) = delete;
  };

}}
