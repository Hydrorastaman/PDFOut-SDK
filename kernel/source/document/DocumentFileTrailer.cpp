#include <document/DocumentFileTrailer.h>

#include <string>

namespace{
  std::string const keySize = "Size";
  std::string const keyPrev = "Prev";
  std::string const keyRoot = "Root";
  std::string const keyEncrypt = "Encrypt";
  std::string const keyInfo = "Info";
  std::string const keyID = "ID";

  std::string const trailer = "trailer\r\n";
}

namespace kernel{

  std::unordered_map<DocumentFileTrailerKey, std::pair<std::string, uint32_t>> DocumentFileTrailer::mDocumentFileTrailerMap{
    {DocumentFileTrailerKeySize,    {keySize,    ObjectTypeInteger}},
    {DocumentFileTrailerKeyPrev,    {keyPrev,    ObjectTypeInteger}},
    {DocumentFileTrailerKeyRoot,    {keyRoot,    ObjectTypeDictionary}},
    {DocumentFileTrailerKeyEncrypt, {keyEncrypt, ObjectTypeDictionary}},
    {DocumentFileTrailerKeyInfo,    {keyInfo,    ObjectTypeDictionary}},
    {DocumentFileTrailerKeyID,      {keyID,      ObjectTypeArray}} // non indirectable
  };

  void DocumentFileTrailer::addKey(DocumentFileTrailerKey key, std::unique_ptr<Object> value){
    ObjectDictionary::addKey(mDocumentFileTrailerMap, key, std::move(value));
  }

  void DocumentFileTrailer::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
    stream->write(trailer.c_str(), sizeof(uint8_t), trailer.length());
    ObjectDictionary::serialize(stream, removeEncryption(params).get());
  }
}
