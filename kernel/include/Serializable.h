#pragma once

#include <memory>
#include <string>
#include <vector>

#include <stream/OutputStream.h>

namespace kernel{

  class DocumentCrossReferenceTable;
  class Object;

  namespace encryption{
    class Encoder;
  };

  namespace filter{
    class Filter;
  }

  struct SerializeParams{
    DocumentCrossReferenceTable *mXref;
    filter::Filter *mCompressionFilter;
    std::vector<std::string> mCompressionFilterName;
    Object const *mRootObj;
    encryption::Encoder *mStmEncoder;  // Encoder for stream object
    encryption::Encoder *mStrEncoder;  // Encoder for string object
    encryption::Encoder *mFileEncoder; // Encoder for embedded file object
    bool mIsFontEmbedded;              // Does font programs embedded?
    bool mIsFontSubsetting;            // Does font subsetting enabled?
  };

  std::unique_ptr<SerializeParams> removeCompression(SerializeParams *params);
  std::unique_ptr<SerializeParams> removeEncryption(SerializeParams *params);
  std::string compressionFilterToString(std::vector<std::string> const &compressionFilter);

  class Serializable{
  public:
    virtual ~Serializable(void) {}

    virtual void serialize(pdfout::OutputStream *stream, SerializeParams *params) const = 0;

  protected:
    Serializable(void) {}
    Serializable(Serializable const &) {}
    Serializable &operator=(Serializable const &) {return *this;}
  };
}
