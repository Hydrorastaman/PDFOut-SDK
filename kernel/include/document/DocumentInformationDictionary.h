#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{

  enum InformationDictionaryKey{
    InformationDictionaryKeyTitle,        // (Optional; PDF 1.1) The document's title.
    InformationDictionaryKeyAuthor,       // (Optional) The name of the person who created the document.
    InformationDictionaryKeySubject,      // (Optional; PDF 1.1) The subject of the document.
    InformationDictionaryKeyKeywords,     // (Optional; PDF 1.1) Keywords associated with the document.
    InformationDictionaryKeyCreator,      // (Optional) If the document was converted to PDF from another format, the name of the application (for example, Adobe FrameMaker) that created the original document from which it was converted.
    InformationDictionaryKeyProducer,     // (Optional) If the document was converted to PDF from another format, the name of the application (for example, Acrobat Distiller) that converted it to PDF.
    InformationDictionaryKeyCreationDate, // (Optional) The date and time the document was created, in human-readable form.
    InformationDictionaryKeyModDate,      // (Required if PieceInfo is present in the document catalog; otherwise optional; PDF 1.1) The date and time the document was most recently modified, in human-readable form.
    InformationDictionaryKeyTrapped       // (Optional; PDF 1.3) A name object indicating whether the document has been modified to include trapping information.
  };

  class DocumentInformationDictionary : public ObjectDictionary{
  public:
    DocumentInformationDictionary(void) : ObjectDictionary() {}
    ~DocumentInformationDictionary(void) {}

    void addKey(InformationDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<InformationDictionaryKey, std::pair<std::string, uint32_t>> mDocumentInformationDictionaryMap;

  private:
    DocumentInformationDictionary(DocumentInformationDictionary const &) = delete;
    DocumentInformationDictionary &operator=(DocumentInformationDictionary const &) = delete;
  };
}
