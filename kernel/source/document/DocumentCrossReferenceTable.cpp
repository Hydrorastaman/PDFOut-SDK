#include <document/DocumentCrossReferenceTable.h>

#include <stream/MemoryOStream.h>

namespace{
  uint8_t const EOL[3] = {0x0D, 0x0A, 0x00};
}

namespace kernel{

  void DocumentCrossReferenceTable::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
    char xrefBlock[21] = {0};
    char xrefRange[20] = {0};

    std::unique_ptr<pdfout::MemoryOStream> blockStream = std::make_unique<pdfout::MemoryOStream>();

    mStartXref = stream->tellp();

    sprintf(xrefRange, "xref%s", EOL);
    stream->write(xrefRange, sizeof(uint8_t), strlen(xrefRange));

    sprintf(xrefBlock, "%010i %05i f%s", 0, 65535, EOL);
    blockStream->write(xrefBlock, 20, 1);

    uint32_t from = 0;
    int count = 1;

    for (auto const &iter : mRecords){
      if (iter.first != from + count){
        sprintf(xrefRange, "%i %i%s", from, count, EOL);
        stream->write(xrefRange, sizeof(uint8_t), strlen(xrefRange));

        stream->write(blockStream->getData(), count * 20, 1);
        blockStream->seekp(0);
        from = iter.first;
        count = 0;
      }

      sprintf(xrefBlock, "%010lli %05i n%s", iter.second.mOffset, iter.second.mGeneration, EOL);
      blockStream->write(xrefBlock, 20, 1);
      ++count;
    }

    sprintf(xrefRange, "%i %i%s", from, count, EOL);

    stream->write(xrefRange, sizeof(uint8_t), strlen(xrefRange));
    stream->write(blockStream->getData(), count * 20, 1);
  }
}
