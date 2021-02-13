#include <document/DocumentHeader.h>

#include <string>
#include <ToString.h>

namespace kernel{

  void DocumentHeader::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
    std::string const docVersion = toString(mDocumentVersion);
    std::vector<uint8_t> const comment = {'%', 0xFF, 0xFF, 0xFF, 0xFF, '\r', '\n'};
    stream->write(docVersion.c_str(), sizeof(uint8_t), docVersion.length());
    stream->write(comment.data(), sizeof(uint8_t), comment.size());
  }
}
