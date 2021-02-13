#include <Serializable.h>

namespace kernel{

  std::unique_ptr<SerializeParams> removeCompression(SerializeParams *params){
    std::unique_ptr<SerializeParams> result = std::make_unique<SerializeParams>(*params);
    result->mCompressionFilter = nullptr;
    result->mCompressionFilterName = {};
    return result;
  }

  std::unique_ptr<SerializeParams> removeEncryption(SerializeParams *params){
    std::unique_ptr<SerializeParams> result = std::make_unique<SerializeParams>(*params);
    result->mStmEncoder = nullptr;
    result->mStrEncoder = nullptr;
    result->mFileEncoder = nullptr;
    return result;
  }

  std::string compressionFilterToString(std::vector<std::string> const &compressionFilter){
    std::string result;
    for (auto const &elem : compressionFilter)
      result += elem + " ";

    return result;
  }

}
