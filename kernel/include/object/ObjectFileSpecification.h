#pragma once

#include <object/Object.h>
#include <object/ObjectDictionary.h>

namespace kernel{

  enum ObjectFileSpecificationKey{
    ObjectFileSpecificationKeyFS,   /**< (Optional) The name of the file system to be used to interpret this file specification. */
    ObjectFileSpecificationKeyF,    /**< (Required if the DOS, Mac, and Unix entries are all absent; amended with the UF entry for PDF 1.7) A file specification string of the form or (if the file system is URL) a uniform resource locator. */
    ObjectFileSpecificationKeyUF,   /**< (Optional, but recommended if the F entry exists in the dictionary; PDF 1.7) A Unicode text string that provides file specification of the form. */
    ObjectFileSpecificationKeyDOS,  /**< (Optional) A file specification string representing a DOS file name. */
    ObjectFileSpecificationKeyMac,  /**< (Optional) A file specification string representing a Mac OS file name. */
    ObjectFileSpecificationKeyUnix, /**< (Optional) A file specification string representing a UNIX file name. */
    ObjectFileSpecificationKeyID,   /**< (Optional) An array of two byte strings constituting a file identifier that is also included in the referenced file. */
    ObjectFileSpecificationKeyV,    /**< (Optional; PDF 1.2) A flag indicating whether the file referenced by the file specification is volatile (changes frequently with time). If the value is true, applications should never cache a copy of the file. For example, a movie annotation referencing a URL to a live video camera could set this flag to true to notify the application that it should reacquire the movie each time it is played. */
    ObjectFileSpecificationKeyEF,   /**< (Required if RF is present; PDF 1.3; amended to include the UF key in PDF 1.7) A dictionary containing a subset of the keys F, UF, DOS, Mac, and Unix, corresponding to the entries by those names in the file specification dictionary. */
    ObjectFileSpecificationKeyRF,   /**< (Optional; PDF 1.3) A dictionary with the same structure as the EF dictionary, which must also be present. Each key in the RF dictionary must also be present in the EF dictionary. */
    ObjectFileSpecificationKeyDesc, /**< (Optional; PDF 1.6) Descriptive text associated with the file specification. It is used for files in the EmbeddedFiles name tree. */
    ObjectFileSpecificationKeyCI    /**< (Optional; must be indirect reference; PDF 1.7) A collection item dictionary, which is used to create the user interface for portable collections. */
  };

  class ObjectFileSpecification : public Object{
  public:
    ObjectFileSpecification(void);
    ~ObjectFileSpecification(void);

    void serialize(pdfout::OutputStream *stream, SerializeParams *params) const;

    void addKey(ObjectFileSpecificationKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<ObjectFileSpecificationKey, std::pair<std::string, uint32_t>> mObjectFileSpecificationMap;

  private:
    ObjectFileSpecification(ObjectFileSpecification const &) = delete;
    ObjectFileSpecification &operator=(ObjectFileSpecification const &) = delete;

  private:
    std::unique_ptr<ObjectDictionary> mFileDictionary;
  };
}
