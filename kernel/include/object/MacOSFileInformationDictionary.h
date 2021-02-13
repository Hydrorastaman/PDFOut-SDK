#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{

  enum MacOSFileInformationDictionaryKey{
    MacOSFileInformationDictionaryKeySubtype, /**< (Optional) The embedded file's file type. It is encoded as an integer according to Mac OS conventions: a 4-character ASCII text literal, converted to a 32-bit integer, with the high-order byte first. */
    MacOSFileInformationDictionaryKeyCreator, /**< (Optional) The embedded file's creator signature, encoded in the same way as Subtype. */
    MacOSFileInformationDictionaryKeyResFork  /**< (Optional) The binary contents of the embedded file's resource fork. */
  };

  class MacOSFileInformationDictionary : public ObjectDictionary{
  public:
    MacOSFileInformationDictionary(void);
    ~MacOSFileInformationDictionary(void);

    void addKey(MacOSFileInformationDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<MacOSFileInformationDictionaryKey, std::pair<std::string, uint32_t>> mMacOSFileInformationDictionaryMap;

  private:
    MacOSFileInformationDictionary(MacOSFileInformationDictionary const &) = delete;
    MacOSFileInformationDictionary &operator=(MacOSFileInformationDictionary const &) = delete;
  };

}
