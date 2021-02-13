#include <Halftone.h>

#include <memory>

#include <object/ObjectNumber.h>
#include <object/ObjectInteger.h>
#include <object/ObjectBoolean.h>
#include <object/ObjectByteString.h>

#include <graphics/halftone/HalftoneType1Dictionary.h>
#include <graphics/halftone/HalftoneType5Dictionary.h>
#include <graphics/halftone/HalftoneType6Dictionary.h>
#include <graphics/halftone/HalftoneType10Dictionary.h>
#include <graphics/halftone/HalftoneType16Dictionary.h>

#include <HalftoneImpl.h>
#include <FunctionImpl.h>
#include <UserObjectManager.h>

namespace kernel{
  typedef UserObjectManager<pdfout::impl::HalftoneImpl> HalftoneManager;
}

using namespace kernel;
using namespace pdfout::impl;

namespace pdfout{

  Halftone *Halftone::createHalftoneType1(char const *halftoneName, float frequency, float angle, Function *spotFunction, bool accurateScreens, Function *transferFunction){
    std::unique_ptr<HalftoneType1Dictionary> halftone = std::make_unique<HalftoneType1Dictionary>();

    std::unique_ptr<ObjectByteString> name(std::make_unique<kernel::ObjectByteString>(halftoneName, strlen(halftoneName)));
    halftone->addKey(HalftoneType1DictionaryKeyHalftoneName, std::move(name));

    halftone->addKey(HalftoneType1DictionaryKeyFrequency, std::make_unique<ObjectNumber>(frequency));
    halftone->addKey(HalftoneType1DictionaryKeyAngle, std::make_unique<ObjectNumber>(angle));

    if (spotFunction)
      halftone->addKey(HalftoneType1DictionaryKeySpotFunction, std::unique_ptr<Object>(((FunctionImpl *) spotFunction)->getObject()->clone()));

    halftone->addKey(HalftoneType1DictionaryKeyAccurateScreens, std::make_unique<ObjectBoolean>(accurateScreens));
  
    if (transferFunction)
      halftone->addKey(HalftoneType1DictionaryKeyTransferFunction, std::unique_ptr<Object>(((FunctionImpl *) transferFunction)->getObject()->clone()));

    return HalftoneManager::getInstance().insert(std::unique_ptr<HalftoneImpl>(new HalftoneImpl(halftone.release())));
  }

  Halftone *Halftone::createHalftoneType6(char const *halftoneName, int width, int height, Function *transferFunction){
    std::unique_ptr<HalftoneType6Dictionary> halftone = std::make_unique<HalftoneType6Dictionary>();

    std::unique_ptr<ObjectByteString> name(std::make_unique<kernel::ObjectByteString>(halftoneName, strlen(halftoneName)));
    halftone->addKey(HalftoneType6DictionaryKeyHalftoneName, std::move(name));

    halftone->addKey(HalftoneType6DictionaryKeyWidth, std::make_unique<ObjectInteger>(width));
    halftone->addKey(HalftoneType6DictionaryKeyHeight, std::make_unique<ObjectInteger>(height));

    if (transferFunction)
      halftone->addKey(HalftoneType6DictionaryKeyTransferFunction, std::unique_ptr<Object>(((FunctionImpl *) transferFunction)->getObject()->clone()));

    return HalftoneManager::getInstance().insert(std::unique_ptr<HalftoneImpl>(new HalftoneImpl(halftone.release())));
  }

  Halftone *Halftone::createHalftoneType10(char const *halftoneName, int xSquare, int ySquare, Function *transferFunction){
    std::unique_ptr<HalftoneType10Dictionary> halftone = std::make_unique<HalftoneType10Dictionary>();

    std::unique_ptr<ObjectByteString> name(std::make_unique<kernel::ObjectByteString>(halftoneName, strlen(halftoneName)));
    halftone->addKey(HalftoneType10DictionaryKeyHalftoneName, std::move(name));

    halftone->addKey(HalftoneType10DictionaryKeyXsquare, std::make_unique<ObjectInteger>(xSquare));
    halftone->addKey(HalftoneType10DictionaryKeyYsquare, std::make_unique<ObjectInteger>(ySquare));

    if (transferFunction)
      halftone->addKey(HalftoneType10DictionaryKeyTransferFunction, std::unique_ptr<Object>(((FunctionImpl *) transferFunction)->getObject()->clone()));

    return HalftoneManager::getInstance().insert(std::unique_ptr<HalftoneImpl>(new HalftoneImpl(halftone.release())));
  }

  Halftone *Halftone::createHalftoneType16(char const *halftoneName, int width, int height, int width2, int height2, Function *transferFunction){
    std::unique_ptr<HalftoneType16Dictionary> halftone = std::make_unique<HalftoneType16Dictionary>();

    std::unique_ptr<ObjectByteString> name(std::make_unique<kernel::ObjectByteString>(halftoneName, strlen(halftoneName)));
    halftone->addKey(HalftoneType16DictionaryKeyHalftoneName, std::move(name));

    halftone->addKey(HalftoneType16DictionaryKeyWidth, std::make_unique<ObjectInteger>(width));
    halftone->addKey(HalftoneType16DictionaryKeyHeight, std::make_unique<ObjectInteger>(height));

    if (width2 && height2){
      halftone->addKey(HalftoneType16DictionaryKeyWidth2, std::make_unique<ObjectInteger>(width2));
      halftone->addKey(HalftoneType16DictionaryKeyHeight2, std::make_unique<ObjectInteger>(height2));
    }

    if (transferFunction)
      halftone->addKey(HalftoneType16DictionaryKeyTransferFunction, std::unique_ptr<Object>(((FunctionImpl *) transferFunction)->getObject()->clone()));

    return HalftoneManager::getInstance().insert(std::unique_ptr<HalftoneImpl>(new HalftoneImpl(halftone.release())));
  }

  Halftone *Halftone::createHalftoneType5(char const *halftoneName, Colorant *colorants, Halftone **halftones, std::size_t colorantsSize, Halftone *default){
    std::unique_ptr<HalftoneType5Dictionary> halftone = std::make_unique<HalftoneType5Dictionary>();

    std::unique_ptr<ObjectByteString> name(std::make_unique<kernel::ObjectByteString>(halftoneName, strlen(halftoneName)));
    halftone->addKey(HalftoneType5DictionaryKeyHalftoneName, std::move(name));

    if (default)
      halftone->addKey(HalftoneType5DictionaryKeyDefault, std::unique_ptr<Object>(((HalftoneImpl *) default)->getObject()->clone()));

    HalftoneImpl *halftoneImpl = nullptr;
    if (colorants && halftones){
      for (std::size_t i = 0; i < colorantsSize; ++i){
        std::unique_ptr<Object> halftoneClone(((HalftoneImpl *) halftones[i])->getObject()->clone());

        switch (colorants[i]){
          case ColorantGray:
            halftone->addKey(HalftoneType5DictionaryKeyGray, std::move(halftoneClone));
            break;

          case ColorantRed:
            halftone->addKey(HalftoneType5DictionaryKeyRed, std::move(halftoneClone));
            break;

          case ColorantGreen:
            halftone->addKey(HalftoneType5DictionaryKeyGreen, std::move(halftoneClone));
            break;

          case ColorantBlue:
            halftone->addKey(HalftoneType5DictionaryKeyBlue, std::move(halftoneClone));
            break;

          case ColorantCyan:
            halftone->addKey(HalftoneType5DictionaryKeyCyan, std::move(halftoneClone));
            break;

          case ColorantMagenta:
            halftone->addKey(HalftoneType5DictionaryKeyMagenta, std::move(halftoneClone));
            break;

          case ColorantYellow:
            halftone->addKey(HalftoneType5DictionaryKeyYellow, std::move(halftoneClone));
            break;

          case ColorantBlack:
            halftone->addKey(HalftoneType5DictionaryKeyBlack, std::move(halftoneClone));
            break;
        }
      }
    }

    return HalftoneManager::getInstance().insert(std::unique_ptr<HalftoneImpl>(new HalftoneImpl(halftone.release())));
  }
}
