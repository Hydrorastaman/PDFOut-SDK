#pragma once

#include <vector>
#include <memory>
#include <cstdint>

namespace kernel{
  class Object;

  namespace filter{
    class Filter;
  }
}

namespace kernel{ namespace encryption{

  class Encoder{
  public:
    Encoder(std::vector<uint8_t> const &key);
    virtual ~Encoder(void);

    virtual std::unique_ptr<kernel::filter::Filter> makeEncoder(Object const *obj) const = 0;

  protected:
    std::vector<uint8_t> const &getKey(void) const;

  private:
    Encoder(Encoder const &) = delete;
    Encoder &operator=(Encoder const &) = delete;

  private:
    std::vector<uint8_t> mKey;
  };

}}
