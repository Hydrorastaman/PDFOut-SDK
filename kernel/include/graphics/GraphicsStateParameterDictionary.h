#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{

  enum GraphicsStateParameterDictionaryKey{
    GraphicsStateParameterDictionaryKeyLW,    // Optional; PDF 1.3
    GraphicsStateParameterDictionaryKeyLC,    // Optional; PDF 1.3
    GraphicsStateParameterDictionaryKeyLJ,    // Optional; PDF 1.3
    GraphicsStateParameterDictionaryKeyML,    // Optional; PDF 1.3
    GraphicsStateParameterDictionaryKeyD,     // Optional; PDF 1.3
    GraphicsStateParameterDictionaryKeyRI,    // Optional; PDF 1.3
    GraphicsStateParameterDictionaryKeyOP,    // Optional
    GraphicsStateParameterDictionaryKeyop,    // Optional; PDF 1.3
    GraphicsStateParameterDictionaryKeyOPM,   // Optional; PDF 1.3
    GraphicsStateParameterDictionaryKeyFont,  // Optional; PDF 1.3
    GraphicsStateParameterDictionaryKeyBG,    // Optional
    GraphicsStateParameterDictionaryKeyBG2,   // Optional; PDF 1.3
    GraphicsStateParameterDictionaryKeyUCR,   // Optional
    GraphicsStateParameterDictionaryKeyUCR2,  // Optional; PDF 1.3
    GraphicsStateParameterDictionaryKeyTR,    // Optional
    GraphicsStateParameterDictionaryKeyTR2,   // Optional; PDF 1.3
    GraphicsStateParameterDictionaryKeyHT,    // Optional
    GraphicsStateParameterDictionaryKeyFL,    // Optional; PDF 1.3
    GraphicsStateParameterDictionaryKeySM,    // Optional; PDF 1.3
    GraphicsStateParameterDictionaryKeySA,    // Optional
    GraphicsStateParameterDictionaryKeyBM,    // Optional; PDF 1.4
    GraphicsStateParameterDictionaryKeySMask, // Optional; PDF 1.4
    GraphicsStateParameterDictionaryKeyCA,    // Optional; PDF 1.4
    GraphicsStateParameterDictionaryKeyca,    // Optional; PDF 1.4
    GraphicsStateParameterDictionaryKeyAIS,   // Optional; PDF 1.4
    GraphicsStateParameterDictionaryKeyTK     // Optional; PDF 1.4
  };

  class GraphicsStateParameterDictionary : public ObjectDictionary{
  public:
    GraphicsStateParameterDictionary(void);
    ~GraphicsStateParameterDictionary(void) {}

    void addKey(GraphicsStateParameterDictionaryKey key, std::unique_ptr<Object> value);
    void removeKey(GraphicsStateParameterDictionaryKey key);
    Object const *findKey(GraphicsStateParameterDictionaryKey key) const;

  private:
    static std::unordered_map<GraphicsStateParameterDictionaryKey, std::pair<std::string, uint32_t>> mGraphicsStateParameterDictionaryMap;

  private:
    GraphicsStateParameterDictionary(GraphicsStateParameterDictionary const &) = delete;
    GraphicsStateParameterDictionary &operator=(GraphicsStateParameterDictionary const &) = delete;
  };
}
