#pragma once

#include <memory>
#include <unordered_map>
#include <object/ObjectStream.h>

namespace kernel{ namespace font{

  enum FontFileStreamKey{
    FontFileStreamKeyLength1, /**< (Required for Type 1 and TrueType fonts) The length in bytes of the clear_text portion of the Type 1 font program,
                                   or the entire TrueType font program, after it has been decoded using the filters specified by the stream's Filter entry, if any. */
    FontFileStreamKeyLength2, /**< (Required for Type 1 fonts) The length in bytes of the encrypted portion of the Type 1 font program
                                   after it has been decoded using the filters specified by the stream's Filter entry. */
    FontFileStreamKeyLength3, /**< (Required for Type 1 fonts) The length in bytes of the fixed_content portion of the Type 1 font program after it has been decoded
                                   using the filters specified by the stream's Filter entry. If Length3 is 0, it indicates that the 512 zeros and
                                   cleartomark have not been included in the FontFile font program and must be added. */
    FontFileStreamKeySubtype, /**< (Required if referenced from FontFile3; PDF 1.2) A name specifying the format of the embedded font program.
                                   The name must be Type1C for Type 1 compact fonts, CIDFontType0C for Type 0 compact CIDFonts, or OpenType for OpenType fonts.
                                   When additional font formats are added to PDF, more values will be defined for Subtype. */
    FontFileStreamKeyMetadata /**< (Optional; PDF 1.4) A metadata stream containing metadata for the embedded font program (see Section 10.2.2, “Metadata Streams”). */
  };

  class FontFileStream : public ObjectStream{
  public:
    FontFileStream(void);
    ~FontFileStream(void) {}

    using ObjectStream::addKey;
    void addKey(FontFileStreamKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<FontFileStreamKey, std::pair<std::string, uint32_t>> mFontFileStreamMap;

  private:
    FontFileStream(FontFileStream const &) = delete;
    FontFileStream &operator=(FontFileStream const &) = delete;
  };
}}
