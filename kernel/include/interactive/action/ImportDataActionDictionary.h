#pragma once

#include <interactive/action/ActionDictionary.h>

namespace kernel{ namespace interactive{

  enum ImportDataActionDictionaryKey{
    ImportDataActionDictionaryKeyF,      /**< (Required) The FDF file from which to import the data. */
  };

  /**
   * @brief Annotation dictionary
   */
  class ImportDataActionDictionary : public ActionDictionary{
  public:
    ImportDataActionDictionary(void);
    ~ImportDataActionDictionary(void);

    using ActionDictionary::addKey;
    void addKey(ImportDataActionDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<ImportDataActionDictionaryKey, std::pair<std::string, uint32_t>> mImportDataActionDictionaryMap;

  private:
    ImportDataActionDictionary(ImportDataActionDictionary const &) = delete;
    ImportDataActionDictionary &operator=(ImportDataActionDictionary const &) = delete;
  };

}}
