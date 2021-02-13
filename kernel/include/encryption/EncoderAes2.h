#pragma once

#include <encryption/Encoder.h>

namespace kernel{ namespace encryption{

  class EncoderAes2 : public Encoder{
  public:
    EncoderAes2(std::vector<uint8_t> const &key);
    ~EncoderAes2(void);

    std::unique_ptr<kernel::filter::Filter> makeEncoder(Object const *obj) const;

  private:
    EncoderAes2(EncoderAes2 const &) = delete;
    EncoderAes2 &operator=(EncoderAes2 const &) = delete;
  };

}}
