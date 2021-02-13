#pragma once

#include <atomic>
#include <string>
#include <cstdint>

#include <memory>

namespace kernel{

  class ObjectIndirectReference;

  enum IndirectType{
    IndirectTypeNonIndirectable = 0,
    IndirectTypeIndirectable
  };

  class Indirectable{
  public:
    virtual ~Indirectable(void) {}

    virtual std::unique_ptr<ObjectIndirectReference> getReference(void) const;
    void setReference(ObjectIndirectReference *ref);

    static std::unique_ptr<ObjectIndirectReference> generateReference(void);

    bool isIndirectable(void) const {return mIndirectable == IndirectTypeIndirectable;};

  protected:
    Indirectable(IndirectType indirectable) : mNumber(indirectable == IndirectTypeIndirectable ? gCounter.fetch_add(1) : 0), mIndirectable(indirectable) {}

    Indirectable(Indirectable const &obj) : mNumber(obj.mIndirectable == IndirectTypeIndirectable ? gCounter.fetch_add(1) : 0), mIndirectable(obj.mIndirectable) {}

    Indirectable &operator=(Indirectable const &obj){
      if (this != &obj){
        mNumber = obj.mNumber;
        mIndirectable = obj.mIndirectable;
      }
      return *this;
    }

  private:
    uint32_t mNumber;
    IndirectType mIndirectable;
    static std::atomic_uint32_t gCounter;
  };
}
