#pragma once

#include <vector>

#include "Page.h"
#include "Image.h"
#include "Types.h"
#include "UString.h"
#include "DateTime.h"
#include "PaperSize.h"
#include "Encryption.h"
#include "XMPDocument.h"
#include "PdfOutConfig.h"

namespace pdfout{

  namespace impl{
    class DocumentImpl;
  }

  class PDFOUT_EXPORTS Document{
  public:
    /**
     Creates an empty PDF document.

     @code {.cpp}
       Document document;
       doc.setCompressionFilter(FilterNameFlate);
       doc.setFontEmbedding(true);
       doc.setFontSubsetting(true);
     
       Page *page = doc.createPage(PaperSize_ISO_A4, PaperOrientationPortrait);
       doc.AddPage(page);
     
       ContentStream *cs = page->getContents();
       // make some drawings on cs
     
       doc.saveToFile("document.pdf");
     }
     @endcode
    */
    Document(void);

    ~Document(void);

    /**
     Creates the document from an existing PDF file.
     */
    //explicit Document(pdfout::UString const &filename);

    /**
     Creates document from a stream.
     */
    //explicit Document(pdfout::InputStream *stream);

    /**
     Saves the document to a stream.
     @param stream Pointer to output stream.

     @code {.cpp}
       Document document;
       MemoryOStream stream;
       // ...
       document.saveToStream(&stream);
     @endcode
     */
    void saveToStream(pdfout::OutputStream *stream) const;

    /**
     Saves the document to file.
     @param fileName Output file name.

     @code {.cpp}
       Document document;
       // ...
       document.saveToFile("document.pdf");
     @endcode
     */
    void saveToFile(pdfout::UString const &fileName) const;

    /**
     Sets the documents title.

     @param title Title of the document.

     @code {.cpp}
       Document document;
       document.setTitle("Game Engine Architecture");
     @endcode
     */
    void setTitle(pdfout::UString const &title);

    /**
     Sets the documents author.

     @param author Author of the document.

     @code {.cpp}
       Document document;
       document.setAuthor("Jason Gregory");
     @endcode
     */
    void setAuthor(pdfout::UString const &author);

    /**
     Sets the documents subject.

     @param subject Subject of the document.

     @code {.cpp}
       Document document;
       document.setSubject("game development C++");
     @endcode
     */
    void setSubject(pdfout::UString const &subject);

    /**
     Sets the documents keywords.

     @param keywords Keywords of the document.

     @code {.cpp}
       Document document;
       document.setKeywords("gamedev C++ architecture");
     @endcode
     */
    void setKeywords(pdfout::UString const &keywords);

    /**
     Sets the documents creator.

     @param creator Creator of the document.

     @code {.cpp}
       Document document;
       document.setCreator("Jason Gregory");
     @endcode
     */
    void setCreator(pdfout::UString const &creator);

    /**
     Sets the documents producer.

     @param producer Producer of the document.

     @code {.cpp}
       Document document;
       document.setProducer("CRC Press");
     @endcode
     */
    void setProducer(pdfout::UString const &producer);

    /**
     Sets the documents creation date.

     @param creationDate Creation date of the document.

     @code {.cpp}
       Document document;
       document.setCreationDate(DateTime());
     @endcode
     */
    void setCreationDate(pdfout::DateTime const &creationDate);

    /**
     Sets the documents modification date.

     @param modificationDate Modification date of the document.

     @code {.cpp}
       Document document;
       document.setModificationDate(DateTime());
     @endcode
     */
    void setModificationDate(pdfout::DateTime const &modificationDate);

    /**
     Sets the documents metadata.

     @param xmp Metadata in XMP format

     @code {.cpp}
      std::string const endl = "\r\n";
    
      std::stringstream ss;
      ss << "<?xpacket begin='' id='W5M0MpCehiHzreSzNTczkc9d'?>" << endl;
      ss << "<x:xmpmeta xmlns:x=\"adobe:ns:meta/\" x:xmptk=\"Adobe XMP Core 5.6-c015 81.157285, 2014/12/12-00:43:15\"> " << endl;
      ss << "   <rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"> " << endl;
      ss << "      <rdf:Description rdf:about=\"\"" << endl;
      ss << "            xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\"" << endl;
      ss << "            xmlns:xmpMM=\"http://ns.adobe.com/xap/1.0/mm/\"" << endl;
      ss << "            xmlns:dc=\"http://purl.org/dc/elements/1.1/\">" << endl;
      ss << "         <xmp:ModifyDate>2016-02-07T20:50:29+06:00</xmp:ModifyDate>" << endl;
      ss << "         <xmp:CreateDate>2016-02-07T20:50:29+06:00</xmp:CreateDate>" << endl;
      ss << "         <xmp:MetadataDate>2016-02-07T20:50:29+06:00</xmp:MetadataDate>" << endl;
      ss << "         <xmpMM:DocumentID>uuid:9e5da648-b072-480c-bc80-95cef49d75b9</xmpMM:DocumentID>" << endl;
      ss << "         <xmpMM:InstanceID>uuid:b44a9fdb-c194-40f6-80fe-fb686443464e</xmpMM:InstanceID>" << endl;
      ss << "         <dc:format>application/pdf</dc:format>" << endl;
      ss << "      </rdf:Description>" << endl;
      ss << "   </rdf:RDF>" << endl;
      ss << "</x:xmpmeta>" << endl;
      ss << "<?xpacket end='w'?>" << endl;
    
      pdfout::UString const metadata(ss.str().c_str(), pdfout::Encoding::Encoding_ASCII);

      Document document;
      XMPDocument const xmp(metadata);
      document.setMetadata(xmp);
     @endcode
     */
    void setMetadata(pdfout::XMPDocument const &xmp);

    /**
     Sets compression filters for the document.

     @param filter Filter name.

     @code {.cpp}
       Document document;
       document.setCompressionFilter(FilterNameFlate);
     @endcode
     */
    void setCompressionFilter(FilterName filter);

    /**
     Sets compression filters for the document, can be one or more of the ::FilterName values.

     @param filters Array of FilterName;
     @param count Count of filters in the array.

     @code {.cpp}
       Document document;
       std::vector<FilterName> filters = {FilterNameASCIIHex, FilterNameFlate};
       document.setCompressionFilters(filters.data(), filters.size());
     @endcode
     */
    void setCompressionFilters(FilterName *filters, std::size_t count);

    /**
     Sets font embedding.

     @param isEmbedded Does fonts should be embedded?

     @code {.cpp}
       Document document;
       document.setFontEmbedding(true);
     @endcode
     */
    void setFontEmbedding(bool isEmbedded);

    /**
     Sets font subsetting, only for embedded fonts.

     @param isSubsetting Does fonts should be subsetted?

     @code {.cpp}
       Document document;
       document.setFontEmbedding(true);
       document.setFontSubsetting(true);
     @endcode
     */
    void setFontSubsetting(bool isSubsetting);

    // Page interface

    /**
     Creates new documents page.
 
     @param mediaBox Page mediabox in default user space units.

     @code {.cpp}
       Document document;
       struct Rectangle mediaBox = {0, 0, 8.5 * 72, 11.0 * 72};
       Page *page = document.createPage(mediaBox);
     @endcode
     */
    Page *createPage(struct Rectangle const &mediaBox) const;

    /**
     Creates new documents page with pre-defined paper size and orientation.

     @param mediaBox Pre-defined paper size;
     @param paperOrientation Pre-defined paper orientation;

     @code {.cpp}
       Document document;
       Page *page = document.createPage(PaperSize_ISO_A4, PaperOrientationPortrait);
     @endcode
     */
    Page *createPage(PaperSize mediaBox, PaperOrientation paperOrientation = PaperOrientationPortrait) const;

    /**
     Adds a page to the document.

     @param page Pointer to Page object;

     @code {.cpp}
       Document document;
       Page *page = document.createPage(PaperSize_ISO_A4);
       document.addPage(page);
     @endcode
     */
    void addPage(Page *page);

    /**
     Inserts a page at specified position in the document.

     @param index Zero-based position in the document;
     @param page Pointer to Page object;

     @code {.cpp}
       Document document;
       Page *page = document.createPage(PaperSize_ISO_A4);
       document.insertPage(0, page);
     @endcode
     */
    void insertPage(uint32_t index, Page *page);

    /**
     Removes documents page at specified position.

     @param index Zero-based position in the document.

     @code {.cpp}
       Document document;
       // ...
       document.removePage(13);
     @endcode
     */
    void removePage(uint32_t index);

    /**
     Returns pointer to Page object at specified position.

     @param index Zero-based position in the document;
     @return Pointer to Page object.

     @code {.cpp}
       Document document;
       // ...
       Page *page = document.getPage(0);
     @endcode
     */
    Page *getPage(uint32_t index);

    /**
     Returns count of pages in the document.

     @code {.cpp}
       Document document;
       // ...
       auto const pages = document.getPageCount();
     @endcode
     */
    uint32_t getPageCount(void) const;

    // Image interface

    /**
     Creates the image from the existing file on disk.

     @param fileName Path to the image file;
     @param delayLoad Does image loading should be delayed?
     @return Pointer to Image object.

     @code {.cpp}
       Document document;
       Image *image = document.createImage("image.png", false);
     @endcode
     */
    Image *createImage(pdfout::UString const &fileName, bool delayLoad = true);

    /**
     Creates the image from memory data.

     @param imageData Pointer to image data;
     @param size Size in bytes of image data;
     @return Pointer to Image object.

     @code {.cpp}
       Document document;
       std::vector<uint8_t> source = getImageData();
       Image *image = document.createImage(source.data(), source.size());
     @endcode
     */
    Image *createImage(void *imageData, uint64_t size);

    // Controls interface
    //PushButton *createPushButton(struct Rectangle const &rect);

    // Encryption interface
    /**
     Sets documents encryption algorithm.

     @param encryptionAlgorithm Pre-defined encryption algorithm;
     @param userPassword User password;
     @param ownerPassowrd Owner password;
     @param userAccess User access mask, see ::UserAccess;
     @param encryptMetadata Does documents metadata should be encrypted?

     @code {.cpp}
       Document document;
       uint32_t const accessMask = UserAccessPrint | UserAccessCopy | UserAccessEdit;
       document.setEncryption(EncryptionAlgorithmAESv3, "test", "test", accessMask, true);
     @endcode
     */
    void setEncryption(EncryptionAlgorithm encryptionAlgorithm, pdfout::UString const &userPassword, pdfout::UString const &ownerPassword, uint32_t userAccess, bool encryptMetadata = true);

    // Produce a PDF/A compliance document
    /**
     Enable PDF/A standard compliance.

     @param isCompliance Does document should be compiliance with PDF/A standard?
     @code {.cpp}
       Document document;
       document.setPDFACompliance(true);
     @endcode
     */
    void setPDFACompliance(bool isCompliance);
  private:
    Document(Document const &);
    Document &operator=(Document const &);

  private:
    impl::DocumentImpl *mDocumentImpl;
  };

}
