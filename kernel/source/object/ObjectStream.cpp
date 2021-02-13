#include <object/ObjectStream.h>

#include <filter/Filter.h>
#include <encryption/Encoder.h>

#include <object/ObjectArray.h>
#include <object/ObjectInteger.h>

#include <stream/InputStream.h>
#include <stream/OutputStream.h>
#include <stream/MemoryIStream.h>
#include <stream/MemoryOStream.h>

#include <InternalTools.h>

namespace{
  std::string const keyLength = "Length";
  std::string const keyFilter = "Filter";
  std::string const keyDecodeParms = "DecodeParms";
  std::string const keyF = "F";
  std::string const keyFFilter = "FFilter";
  std::string const keyFDecodeParms = "FDecodeParms";
  std::string const keyDL = "DL";

  std::string const streamBegin = "stream";
  std::string const streamEnd = "endstream";
}

using namespace pdfout;

namespace kernel{

  std::unordered_map<StreamDictionaryKey, std::pair<std::string, uint32_t>> ObjectStream::mObjectStreamMap{
    {StreamDictionaryKeyFilter,       {keyFilter,       ObjectTypeName | ObjectTypeArray}},
    {StreamDictionaryKeyDecodeParms,  {keyDecodeParms,  ObjectTypeDictionary | ObjectTypeArray}},
    {StreamDictionaryKeyF,            {keyF,            ObjectTypeFileSpecification}},
    {StreamDictionaryKeyFFilter,      {keyFFilter,      ObjectTypeName | ObjectTypeArray}},
    {StreamDictionaryKeyFDecodeParms, {keyFDecodeParms, ObjectTypeDictionary | ObjectTypeArray}},
    {StreamDictionaryKeyDL,           {keyDL,           ObjectTypeInteger}},
  };

  ObjectStream::ObjectStream(IndirectType indirectable)
    : Object(ObjectTypeStream, indirectable),
    mStreamDictionary(std::make_unique<ObjectDictionary>(IndirectTypeNonIndirectable)),
    mStreamData(std::make_unique<MemoryOStream>()){

    mStreamDictionary->insert(keyLength, std::make_unique<ObjectInteger>(0));

    //FilterName filters[1] = {FilterNameFlate};
    //setFilter(filters, 1);
  }

  ObjectStream::ObjectStream(std::unique_ptr<MemoryOStream> stream, IndirectType indirectable)
    : Object(ObjectTypeStream, indirectable),
    mStreamDictionary(std::make_unique<ObjectDictionary>(IndirectTypeNonIndirectable)),
    mStreamData(std::move(stream)){
  }

  ObjectStream::ObjectStream(pdfout::InputStream const *stream, IndirectType indirectable)
    : Object(ObjectTypeStream, indirectable),
    mStreamDictionary(std::make_unique<ObjectDictionary>(IndirectTypeNonIndirectable)),
    mStreamData(std::make_unique<MemoryOStream>()){

    mStreamDictionary->insert(keyLength, std::make_unique<ObjectInteger>(0));

    if (stream){
      uint64_t read = 0;
      unsigned char buff[chunkSize] = {0};
      while (read = stream->read(buff, sizeof(uint8_t), chunkSize))
        mStreamData->write(buff, sizeof(uint8_t), read);
    }
  }

  ObjectStream::ObjectStream(char const *stream, IndirectType indirectable)
    : Object(ObjectTypeStream, indirectable),
    mStreamDictionary(std::make_unique<ObjectDictionary>(IndirectTypeNonIndirectable)),
    mStreamData(std::make_unique<MemoryOStream>()){

    mStreamDictionary->insert(keyLength, std::make_unique<ObjectInteger>(0));

    if (stream)
      mStreamData->write(stream, sizeof(uint8_t), strlen(stream));
  }

  ObjectStream::ObjectStream(ObjectStream const &obj)
    : Object(obj),
    mStreamDictionary(obj.mStreamDictionary->clone()),
    mStreamData(std::make_unique<MemoryOStream>()){
      mStreamData->write(obj.mStreamData->getData(), sizeof(uint8_t), obj.mStreamData->getSize());
  }

  uint64_t ObjectStream::write(void const *data, uint32_t sizeOfElements, uint64_t numOfElements){
    return mStreamData->write(data, sizeOfElements, numOfElements);
  }

  uint64_t ObjectStream::write(char const *str){
    return mStreamData->write(str, sizeof(uint8_t), strlen(str));
  }

  void ObjectStream::addKey(StreamDictionaryKey key, std::unique_ptr<Object> value){
    addKey(mObjectStreamMap, key, std::move(value));
  }

  void ObjectStream::removeKey(StreamDictionaryKey key){
    auto iter = mObjectStreamMap.find(key);
    if (iter != std::end(mObjectStreamMap))
      mStreamDictionary->remove(iter->second.first);
  }

  void ObjectStream::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
    std::unique_ptr<ObjectIndirectReference> ref = getReference();

    if (isIndirectable()){
      params->mXref->addRecord(ref->getNumber(), ref->getGeneration(), stream->tellp());
      writeHeader(stream, ref->getNumber(), ref->getGeneration());
      params->mRootObj = this;
    }

    beforeSerialize();

    std::unique_ptr<pdfout::MemoryOStream> ostream = std::make_unique<pdfout::MemoryOStream>();
    pdfout::MemoryOStream *sourceOStream = mStreamData.get();
    std::unique_ptr<ObjectArray> filter(nullptr);

    if (params && params->mCompressionFilter){
      std::unique_ptr<pdfout::MemoryIStream> istream = std::make_unique<pdfout::MemoryIStream>(mStreamData->getData(), mStreamData->getSize());
      params->mCompressionFilter->encode(istream.get(), ostream.get());
      sourceOStream = ostream.get();

      if (!params->mCompressionFilterName.empty()){
        filter = std::make_unique<ObjectArray>(IndirectTypeNonIndirectable);
        for (auto const &elem : params->mCompressionFilterName)
          filter->insert(std::make_unique<ObjectName>(elem));
      }
    }

    if (params && params->mStmEncoder){
      auto encoder = params->mStmEncoder->makeEncoder(this);
      std::unique_ptr<pdfout::MemoryIStream> istream = std::make_unique<pdfout::MemoryIStream>(sourceOStream->getData(), sourceOStream->getSize());
      std::unique_ptr<pdfout::MemoryOStream> encryptedOStream = std::make_unique<pdfout::MemoryOStream>();
      encoder->encode(istream.get(), encryptedOStream.get());
      ostream = std::move(encryptedOStream);
      sourceOStream = ostream.get();
    }

    mStreamDictionary->insert(keyLength, std::make_unique<ObjectInteger>(sourceOStream->getSize()));
    if (filter)
      mStreamDictionary->insert(keyFilter, std::move(filter));

    std::deque<Object *> indirectObjects;
    mStreamDictionary->serializeBody(stream, params, indirectObjects);
    stream->write(streamBegin.c_str(), sizeof(uint8_t), streamBegin.length());
    kernel::writeEOL(stream);
    stream->write(sourceOStream->getData(), sizeof(uint8_t), sourceOStream->getSize());

    kernel::writeEOL(stream);
    stream->write(streamEnd.c_str(), sizeof(uint8_t), streamEnd.length());

    if (isIndirectable()){
      kernel::writeEOL(stream);
      writeFooter(stream);
    }

    mStreamDictionary->serializeIndirectObjects(stream, params, indirectObjects);
  }

  ObjectStream *ObjectStream::clone(void) const{
    return new ObjectStream(*this);
  }

  MemoryOStream *ObjectStream::getStream(void){
    return mStreamData.get();
  }

  void ObjectStream::writeEOL(void){
    kernel::writeEOL(mStreamData.get());
  }
}
