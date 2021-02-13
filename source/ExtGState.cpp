#include <ExtGState.h>
#include <ExtGStateImpl.h>
#include <UserObjectManager.h>

namespace kernel{
  typedef UserObjectManager<pdfout::impl::ExtGStateImpl> ExtGStateManager;
}

using namespace pdfout::impl;
using namespace kernel;

namespace pdfout{

  ExtGState *ExtGState::createExtGState(void){
    return ExtGStateManager::getInstance().insert(std::unique_ptr<ExtGStateImpl>(new ExtGStateImpl()));
  }

}
