#pragma once

#include <memory>

#include <object/Object.h>
#include <stream/MemoryOStream.h>
#include <object/ObjectDictionary.h>

namespace pdfout{
  class InputStream;
  class OutputStream;
  class MemoryIStream;
}

namespace kernel{

  enum StreamDictionaryKey{
    StreamDictionaryKeyFilter,       // Optional
    StreamDictionaryKeyDecodeParms,  // Optional
    StreamDictionaryKeyF,            // Optional; PDF 1.2
    StreamDictionaryKeyFFilter,      // Optional; PDF 1.2
    StreamDictionaryKeyFDecodeParms, // Optional; PDF 1.2
    StreamDictionaryKeyDL            // Optional; PDF 1.5
  };


  class ObjectStream : public Object{
  private:
    static std::size_t const chunkSize = 4096;

  public:
    explicit ObjectStream(IndirectType indirectable = IndirectTypeIndirectable);
    explicit ObjectStream(std::unique_ptr<pdfout::MemoryOStream> stream, IndirectType indirectable = IndirectTypeIndirectable);
    explicit ObjectStream(pdfout::InputStream const *stream, IndirectType indirectable = IndirectTypeIndirectable);
    explicit ObjectStream(char const *stream, IndirectType indirectable = IndirectTypeIndirectable);

    ~ObjectStream(void) {}

    void insert(std::string const &key, std::unique_ptr<Object> value) {mStreamDictionary->insert(key, std::move(value));}

    void insert(ObjectName *key, std::unique_ptr<Object> value) {mStreamDictionary->insert(key, std::move(value));}

    uint64_t write(void const *data, uint32_t sizeOfElements, uint64_t numOfElements);

    uint64_t write(char const *str);

    void addKey(StreamDictionaryKey key, std::unique_ptr<Object> value);

    void removeKey(StreamDictionaryKey key);

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const;

    ObjectStream *clone(void) const;

    pdfout::MemoryOStream *getStream(void);

    void writeEOL(void);

    template <typename T>
    void addKey(std::unordered_map<T, std::pair<std::string, uint32_t>> const &map, T key, std::unique_ptr<Object> value){
      auto iter = map.find(key);
      if (iter != std::end(map) && (value->getType() & (iter->second.second | ObjectTypeIndirectReference))){
        mStreamDictionary->insert(iter->second.first, std::move(value));
        return;
      }

      RUNTIME_EXCEPTION("Unsupported object type");
    }

  protected:
    ObjectStream(ObjectStream const &obj);
    virtual void beforeSerialize(void) const {}

  private:
    static std::unordered_map<StreamDictionaryKey, std::pair<std::string, uint32_t>> mObjectStreamMap;

  private:
    ObjectStream &operator=(ObjectStream const &) = delete;

  protected:
    std::unique_ptr<ObjectDictionary> mStreamDictionary;
    mutable std::unique_ptr<pdfout::MemoryOStream> mStreamData; /**< for beforeSerialize() method in derived classes */
  };
}
