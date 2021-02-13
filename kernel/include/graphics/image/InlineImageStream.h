#pragma once

#include <object/ObjectStream.h>

namespace kernel{ namespace graphics{ namespace image{

  enum InlineImageStreamKey{
    InlineImageStreamKeyBPC,    /**< BitsPerComponent */
    InlineImageStreamKeyCS,     /**< ColorSpace */
    InlineImageStreamKeyD,      /**< Decode */
    InlineImageStreamKeyDP,     /**< DecodeParms */
    InlineImageStreamKeyF,      /**< Filter */
    InlineImageStreamKeyH,      /**< Height */
    InlineImageStreamKeyIM,     /**< ImageMask */
    InlineImageStreamKeyIntent, /**< Intent (PDF 1.1) */
    InlineImageStreamKeyI,      /**< Interpolate */
    InlineImageStreamKeyW       /**< Width */
  };

  class InlineImageStream : public ObjectStream{
  public:
    InlineImageStream(void);
    ~InlineImageStream(void);

    using ObjectStream::addKey;
    void addKey(InlineImageStreamKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<InlineImageStreamKey, std::pair<std::string, uint32_t>> mInlineImageStreamMap;

  private:
    InlineImageStream(InlineImageStream const &) = delete;
    InlineImageStream &operator=(InlineImageStream const &) = delete;
  };

}}}
