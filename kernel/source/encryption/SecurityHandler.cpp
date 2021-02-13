#include <encryption/SecurityHandler.h>
#include <encryption/EncryptionDictionary.h>
#include <encryption/CryptFilter.h>
#include <encryption/CryptFilterDictionary.h>
#include <object/ObjectNumber.h>
#include <object/ObjectInteger.h>

namespace kernel{ namespace encryption{

  SecurityHandler::SecurityHandler(uint32_t v, uint32_t keyLength)
    : mV(v),
    mKeyLength(keyLength),
    mCryptFilters(){
  }

  SecurityHandler::~SecurityHandler(void) {}

  void SecurityHandler::addCryptFilter(std::unique_ptr<CryptFilter> cryptFilter){
    if (!cryptFilter)
      return;

    std::string const name = cryptFilter->getName();
    auto iter = mCryptFilters.find(name);
    if (iter != std::end(mCryptFilters)){
      std::string const message = "Two or more CryptFilter with the same name \"" + name + "\"";
      RUNTIME_EXCEPTION(message);
    }

    mCryptFilters[name] = std::move(cryptFilter);
  }

  void SecurityHandler::setStrF(std::string const &filterName){
    validateFilterName(filterName);
    mStrF = filterName;
  }

  void SecurityHandler::setStmF(std::string const &filterName){
    validateFilterName(filterName);
    mStmF = filterName;
  }

  void SecurityHandler::setEFF(std::string const &filterName){
    validateFilterName(filterName);
    mEFF = filterName;
  }

  void SecurityHandler::serialize(kernel::encryption::EncryptionDictionary *encryptionDictionary, pdfout::OutputStream *stream, SerializeParams *params) const{
    encryptionDictionary->addKey(EncryptionDictionaryKeyV, std::make_unique<ObjectInteger>(mV));

    //if (mV == 2 || mV == 3)
    encryptionDictionary->addKey(EncryptionDictionaryKeyLength, std::make_unique<ObjectInteger>(mKeyLength));

    if (mV == 4 || mV == 5){
      if (!mStrF.empty())
        encryptionDictionary->addKey(EncryptionDictionaryKeyStrF, std::make_unique<ObjectName>(mStrF));

      if (!mStmF.empty())
        encryptionDictionary->addKey(EncryptionDictionaryKeyStmF, std::make_unique<ObjectName>(mStmF));

      if (!mEFF.empty())
        encryptionDictionary->addKey(EncryptionDictionaryKeyEFF, std::make_unique<ObjectName>(mEFF));

      std::unique_ptr<ObjectDictionary> CF = std::make_unique<ObjectDictionary>(IndirectTypeNonIndirectable);
      for (auto const &elem : mCryptFilters){
        auto ref = elem.second->serialize(stream, params);
        CF->insert(elem.first, std::move(ref));
      }

      encryptionDictionary->addKey(EncryptionDictionaryKeyCF, std::move(CF));
    }
  }

  void SecurityHandler::validateFilterName(std::string const &filterName) const{
    auto iter = mCryptFilters.find(filterName);
    if (iter == std::end(mCryptFilters)){
      std::string const message = "Failed to find CryptFilter with name \"" + filterName + "\"";
      RUNTIME_EXCEPTION(message);
    }
  }

}}
