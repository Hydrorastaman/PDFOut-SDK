#include <Indirectable.h>
#include <object/ObjectIndirectReference.h>

namespace kernel{

  std::atomic_uint32_t Indirectable::gCounter = 1;

  std::unique_ptr<ObjectIndirectReference> Indirectable::getReference(void) const{
    if (mIndirectable)
      return std::make_unique<ObjectIndirectReference>(mNumber, 0);
    else
      return std::unique_ptr<ObjectIndirectReference>(nullptr);
  }

  void Indirectable::setReference(ObjectIndirectReference *ref){
    mNumber = ref->getNumber();
  }

  std::unique_ptr<ObjectIndirectReference> Indirectable::generateReference(void){
    return std::make_unique<ObjectIndirectReference>(gCounter.fetch_add(1), 0);
  }

}
