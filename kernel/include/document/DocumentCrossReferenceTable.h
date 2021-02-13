#pragma once

#include <map>
#include <string>
#include <cstdint>

#include <Serializable.h>

namespace kernel{

  class DocumentCrossReferenceTable : public Serializable{
  private:
    struct Record{
      uint64_t mOffset;
      uint16_t mGeneration;

      Record(uint64_t offset, uint16_t generation)
        : mOffset(offset),
        mGeneration(generation) {}
    };

  public:
    DocumentCrossReferenceTable(void) : Serializable(), mRecords(), mStartXref(0){}
    ~DocumentCrossReferenceTable(void) {}

    void addRecord(uint32_t number, uint16_t generation, uint64_t offset) {mRecords.emplace(number, Record(offset, generation));}

    uint64_t getStartXref(void) const {return mStartXref;}

    uint32_t getHighestNumber(void) const {return mRecords.rbegin()->first;}

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const;

  private:
    DocumentCrossReferenceTable(DocumentCrossReferenceTable const &) = delete;
    DocumentCrossReferenceTable &operator=(DocumentCrossReferenceTable const &) = delete;

  private:
    std::map<uint32_t, Record> mRecords;
    mutable uint64_t mStartXref;
  };
}
