#pragma once

#include <object/ObjectStream.h>

namespace kernel{ namespace graphics{ namespace color{

  enum ICCStreamDictionaryKey{
    ICCStreamDictionaryKeyN,         // Required
    ICCStreamDictionaryKeyAlternate, // Optional
    ICCStreamDictionaryKeyRange,     // Optional
    ICCStreamDictionaryKeyMetadata   // Optional; PDF 1.4
  };

  class ColorICCStream : public ObjectStream{
  public:
    ColorICCStream(void);
    ~ColorICCStream(void);

    using ObjectStream::addKey;
    void addKey(ICCStreamDictionaryKey key, std::unique_ptr<Object> value);

    ColorICCStream *clone(void) const;

  private:
    static std::unordered_map<ICCStreamDictionaryKey, std::pair<std::string, uint32_t>> mColorICCStreamMap;

  private:
    ColorICCStream(ColorICCStream const &obj);
    ColorICCStream &operator=(ColorICCStream const &) = delete;
  };

}}}
