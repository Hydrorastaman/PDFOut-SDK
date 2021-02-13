#ifdef __NOT_IMPLEMENTED___

#include <Destination.h>

#include <unordered_map>

#include <object/ObjectArray.h>
#include <object/ObjectInteger.h>
#include <object/ObjectName.h>

#include <Page.h>


using namespace kernel;

namespace{
  std::unordered_map<DestinationFit, std::string> const destinationFitToString{
    {DestinationFitXYZ,   "XYZ"},
    {DestinationFitFull,  "Fit"},
    {DestinationFitH,     "FitH"},
    {DestinationFitV,     "FitV"},
    {DestinationFitR,     "FitR"},
    {DestinationFitB,     "FitB"},
    {DestinationFitBH,    "FitBH"},
    {DestinationFitBV,    "FitBV"}
  };

  std::string const &getDestinationFitName(DestinationFit fit){
    auto iter = destinationFitToString.find(fit);
    if (iter != std::end(destinationFitToString))
      return iter->second;

    RUNTIME_EXCEPTION("unknown DestinationFit value");
  }

}

Destination::Destination(void) {}
Destination::~Destination(void) {}

std::unique_ptr<kernel::ObjectArray> Destination::createExplicitDestination(Page const *page, DestinationFit fit, int32_t param0, int32_t param1, int32_t param2, int32_t param3){
  std::unique_ptr<ObjectArray> destination = std::make_unique<ObjectArray>(IndirectTypeIndirectable);

  destination->insert(std::move(page->getReference()));
  destination->insert(std::make_unique<ObjectName>(getDestinationFitName(fit)));

  std::unique_ptr<ObjectInteger> num0 = std::make_unique<ObjectInteger>(param0);
  std::unique_ptr<ObjectInteger> num1 = std::make_unique<ObjectInteger>(param1);
  std::unique_ptr<ObjectInteger> num2 = std::make_unique<ObjectInteger>(param2);
  std::unique_ptr<ObjectInteger> num3 = std::make_unique<ObjectInteger>(param3);

  switch (fit){
    case DestinationFitXYZ:
      destination->insert(std::move(num0)); // left
      destination->insert(std::move(num1)); // top
      destination->insert(std::move(num2)); // zoom
      break;

    case DestinationFitFull:
      break;

    case DestinationFitH:
      destination->insert(std::move(num0)); // top
      break;

    case DestinationFitV:
      destination->insert(std::move(num0)); // left
      break;

    case DestinationFitR:
      destination->insert(std::move(num0)); // left
      destination->insert(std::move(num1)); // bottom
      destination->insert(std::move(num2)); // right
      destination->insert(std::move(num3)); // top
      break;

    case DestinationFitB:
      break;

    case DestinationFitBH:
      destination->insert(std::move(num0)); // top
      break;

    case DestinationFitBV:
      destination->insert(std::move(num0)); // left
      break;
  }

  return destination;
}

#endif
