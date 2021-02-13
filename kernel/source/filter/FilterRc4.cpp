#include <filter/FilterRc4.h>

#include <stream/OutputStream.h>
#include <stream/InputStream.h>

using namespace CryptoPP;

namespace kernel{ namespace filter{

  FilterRc4::FilterRc4(uint8_t const *key, std::size_t length)
    : Filter(),
    mKey(key, key + length){
  }

  FilterRc4::FilterRc4(std::vector<uint8_t> const &key, std::unique_ptr<Filter> attachment)
    : Filter(std::move(attachment)),
    mKey(key){
  }

  FilterRc4::FilterRc4(std::vector<uint8_t> const &key, Filter *attachment)
    : Filter(attachment),
    mKey(key){
  }

  FilterRc4::~FilterRc4(void) {}

  FilterRc4 *FilterRc4::clone(void) const{
    return nullptr;
  }

  bool FilterRc4::encode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream){
    return processing(istream, ostream);
  }

  bool FilterRc4::decode2(pdfout::InputStream *istream, pdfout::OutputStream *ostream){
    return processing(istream, ostream);
  }

  bool FilterRc4::processing(pdfout::InputStream *istream, pdfout::OutputStream *ostream){
    if (!istream || !ostream)
      return false;

    reset();

    uint32_t const buffSize = 4096;
    uint8_t buff[buffSize] = {0};
    uint64_t read = 0;

    while (read = istream->read(&buff[0], sizeof(uint8_t), buffSize)){
      mEncryptor.ProcessData(buff, buff, read);
      ostream->write(buff, sizeof(uint8_t), read);
    }

    return true;
  }

  void FilterRc4::reset(void){
    mEncryptor.SetKey(mKey.data(), mKey.size());
  }

}}
