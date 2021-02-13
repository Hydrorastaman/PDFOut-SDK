#include <filter/Filter.h>

#include <stream/MemoryOStream.h>
#include <stream/MemoryIStream.h>
#include <stream/MemoryIOStream.h>

using namespace pdfout;

namespace kernel{ namespace filter{
  Filter::Filter(void)
    : mAttachment(nullptr){
  }

  Filter::Filter(std::unique_ptr<Filter> attachment)
    : mAttachment(std::move(attachment)){
  }

  Filter::Filter(Filter *attachment)
    : mAttachment(attachment->clone()){
  }

  bool Filter::encode(pdfout::InputStream *istream, pdfout::OutputStream *ostream){
    if (mAttachment){
      std::unique_ptr<pdfout::MemoryIOStream<>> memoryStream = std::make_unique<pdfout::MemoryIOStream<>>();
      if (!encode2(istream, memoryStream.get()))
        return false;

      return mAttachment->encode(memoryStream.get(), ostream);
    }
    else
      return encode2(istream, ostream);
  }

  bool Filter::decode(pdfout::InputStream *istream, pdfout::OutputStream *ostream){
    if (mAttachment){
      std::unique_ptr<pdfout::MemoryIOStream<>> memoryStream = std::make_unique<pdfout::MemoryIOStream<>>();
      if (!decode2(istream, memoryStream.get()))
        return false;

      return mAttachment->decode(memoryStream.get(), ostream);
    }
    else
      return decode2(istream, ostream);
  }

  void Filter::attach(std::unique_ptr<Filter> attachment){
    mAttachment = std::move(attachment);
  }
}}
