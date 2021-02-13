#pragma once

#include <object/ObjectStream.h>

namespace kernel{

  enum EmbeddedFileStreamKey{
    EmbeddedFileStreamKeyParams,      /**< (Optional) An embedded file parameter dictionary containing additional, file-specific information. */
    EmbeddedFileStreamKeySize,        /**< (Optional) The size of the embedded file, in bytes. */  
    EmbeddedFileStreamKeyCreationDate,/**< (Optional) The date and time when the embedded file was created. */
    EmbeddedFileStreamKeyModDate,     /**< (Optional) The date and time when the embedded file was last modified. */
    EmbeddedFileStreamKeyMac,         /**< (Optional) A subdictionary containing additional information specific to Mac OS files. */
    EmbeddedFileStreamKeyCheckSum     /**< (Optional) A 16-byte string that is the checksum of the bytes of the uncompressed embedded file. The checksum is calculated by applying the standard MD5 message-digest algorithm to the bytes of the embedded file stream. */
  };

  class EmbeddedFileStream : public ObjectStream{
  public:
    EmbeddedFileStream(std::string const &subtype);
    ~EmbeddedFileStream(void);

    using ObjectStream::addKey;
    void addKey(EmbeddedFileStreamKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<EmbeddedFileStreamKey, std::pair<std::string, uint32_t>> mObjectStreamMap;

  private:
    // deny
    EmbeddedFileStream(EmbeddedFileStream const &) = delete;
    EmbeddedFileStream &operator=(EmbeddedFileStream const &) = delete;
  };

}
