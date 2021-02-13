#pragma once

#include <encryption/Encoder.h>

namespace kernel{ namespace encryption{

  class EncoderAes3 : public Encoder{
  public:
    EncoderAes3(std::vector<uint8_t> const &key);
    ~EncoderAes3(void);

    std::unique_ptr<kernel::filter::Filter> makeEncoder(Object const *obj) const;

  private:
    EncoderAes3(EncoderAes3 const &) = delete;
    EncoderAes3 &operator=(EncoderAes3 const &) = delete;
  };

}}
