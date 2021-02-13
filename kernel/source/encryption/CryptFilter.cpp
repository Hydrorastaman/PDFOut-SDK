#include <encryption/CryptFilter.h>

#include <string>
#include <Tools.h>
#include <ToString.h>

#include <object/ObjectName.h>
#include <object/ObjectInteger.h>
#include <encryption/CryptFilterDictionary.h>

namespace kernel{ namespace encryption{

  CryptFilter::CryptFilter(std::string const &name, CryptFilterCfm cfm, CryptFilterAuthEvent authEvent, uint32_t keyLength)
    : mName(name),
    mCryptFilterDictionary(std::make_unique<CryptFilterDictionary>()){

    mCryptFilterDictionary->addKey(CryptFilterDictionaryKeyCFM, std::make_unique<ObjectName>(toString(cfm)));
    mCryptFilterDictionary->addKey(CryptFilterDictionaryKeyAuthEvent, std::make_unique<ObjectName>(toString(authEvent)));
    mCryptFilterDictionary->addKey(CryptFilterDictionaryKeyLength, std::make_unique<ObjectInteger>(keyLength));
  }

  CryptFilter::~CryptFilter(void){
  }

  std::string CryptFilter::getName(void) const{
    return mName;
  }

  std::unique_ptr<ObjectIndirectReference> CryptFilter::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
    mCryptFilterDictionary->serialize(stream, removeEncryption(params).get());
    return mCryptFilterDictionary->getReference();
  }

  //std::unique_ptr<CryptFilter> CryptFilter::makeIdentityCF(void){
  //  return std::make_unique<CryptFilter>();
  //}

  std::unique_ptr<CryptFilter> CryptFilter::makeStandardCF(CryptFilterCfm cfm, uint32_t length){
    std::string const filterName = "StdCF";

    switch (cfm){
      case CryptFilterCfmV2:{
        if (length >= 40 && length <= 128 && !(length % 8))
          return std::unique_ptr<CryptFilter>(new CryptFilter(filterName, cfm, CryptFilterAuthEventDocOpen, length));

        RUNTIME_EXCEPTION("Invalid Length value, it must be a multiple of 8 in the range of 40 to 128");
        break;
      }

      case CryptFilterCfmAESV2:
        return std::unique_ptr<CryptFilter>(new CryptFilter(filterName, cfm, CryptFilterAuthEventDocOpen, 128));

      case CryptFilterCfmAESV3:
        return std::unique_ptr<CryptFilter>(new CryptFilter(filterName, cfm, CryptFilterAuthEventDocOpen, 256));
    }
 
    RUNTIME_EXCEPTION("Invalid value for CryptFilterCfm");
    return nullptr;
  }

}}
