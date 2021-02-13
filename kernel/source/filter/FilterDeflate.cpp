#include <filter/FilterDeflate.h>

#include <Exception.h>
#include <InternalTools.h>
#include <stream/InputStream.h>
#include <stream/OutputStream.h>

#include <zlib.h>

namespace kernel{ namespace filter{
  FilterDeflate *FilterDeflate::clone(void) const{
    return new FilterDeflate();
  }

  bool FilterDeflate::encode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream){
    if (!istream || !ostream)
      return false;

    CryptoPP::ZlibCompressor encoder;

    std::size_t const size = 4096;
    uint8_t inBuff[size] = {0};
    uint64_t read = 0;
    std::size_t available = 0;
    std::vector<uint8_t> outBuff(size);
    while (read = istream->read(inBuff, sizeof(uint8_t), size)){
      encoder.Put(inBuff, read);
      encoder.Flush(false);
      available = encoder.Get(outBuff.data(), encoder.MaxRetrievable());
      ostream->write(outBuff.data(), sizeof(uint8_t), available);
    }

    encoder.MessageEnd();
    available = encoder.Get(outBuff.data(), encoder.MaxRetrievable());
    ostream->write(outBuff.data(), sizeof(uint8_t), available);
  
    return true;
  }

  bool FilterDeflate::decode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream){
    if (!istream || !ostream)
      return false;

    NOT_IMPLEMENTED();

    return true;
  }
}}
