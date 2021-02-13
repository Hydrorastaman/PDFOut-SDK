#pragma once

#include <vector>

#include "Encoding.h"
#include "PdfOutConfig.h"

namespace pdfout{

  /**
   * @brief Universal string class
   */
  class PDFOUT_EXPORTS UString {
  public:
    /**
     * Creates null-terminated UString from null-terminated string with using default encoding (see setDefaultEncoding method).
     * @param source [in, required] null-terminated string
     */
    UString(char const *source);

    /**
     * Creates null-terminated UString from null-terminated string with specified encoding
     * @param source [in, required] Null-terminated string
     * @param encoding [in, required] The incoming string encoding
     */
    UString(char const *source, Encoding encoding);

    /**
     * Creates null-terminated UString from null-terminated wide string with specified encoding
     * @param source [in, required] Null-terminated wide string
     * @param encoding [in, optional] The incoming string encoding. Default value for Windows platform is UTF16-LE.
     */
    UString(wchar_t const *source, Encoding encoding = Encoding_WCHAR_T);

    /**
     * Creates UString from incoming string with specified length and encoding
     * @param source [in, required] The incoming string
     * @param length [in, required] The length in bytes for incoming string
     * @param encoding [in, required] The incoming string encoding
     */
    UString(uint8_t const *source, std::size_t length, Encoding encoding);

    ~UString(void);

    UString(UString const &rhs);
    UString &operator=(UString const &rhs);

#if defined __GNUC__ || _MSC_VER >= 1600
    UString(UString &&rhs);
    UString &operator=(UString &&rhs);
#endif

    /**
     * Concatenates with null-terminated string
     * @param rhs [in, required] The incoming null-terminated string
     */
    UString &operator+=(UString const &rhs);

    /**
     * Returns UString encoding
     */
    Encoding getEncoding(void) const;

    /**
     * Returns UString source data
     */
    uint8_t const *getData(void) const;

    /**
     * Returns UString source data length in bytes
     */
    std::size_t getSize(void) const;

    /**
     * Creates new UString object with specified encoding
     * @param encoding [in, required] An encoding for new UString object
     */
    UString encode(Encoding encoding) const;

    /**
     * Normalize by applying SASLPrep profile (see RFC 4013)
     * @return UTF-8 encoded string
     */
    UString saslPrepProfile(void) const;

    /**
     * Apply Unicode Bidirectional Algorithm v12.0.0 (https://unicode.org/reports/tr9/)
     * for resolving right-to-left text flow
     */
    UString applyUnicodeBidiAlgorithm(std::vector<uint32_t> &transpositions) const;

    /**
     * Specifies default An encoding for UString objects
     * @param encoding [in, required] The default encoding for UString objects. Default encoding is ASCII
     */
    static void setDefaultEncoding(Encoding encoding);

    /**
     * Returns default encoding for UString objects
     */
    static Encoding getDefaultEncoding(void);

  private:
    void initialize(uint8_t const *source, std::size_t lenght);
    void release(void);

    /**
     * Converts source data from one encoding to other
     * @param [in, required] data source data
     * @param [in, required] size size of data
     * @param [in, required] from source encoding
     * @param [in, required] to required encoding
     * @param [out] result source data in required encoding
     */
    bool convertEncoding(void const *data, std::size_t size, Encoding from, Encoding to, std::vector<uint8_t> &result) const;

  private:
    Encoding mEncoding;
    uint8_t *mData;
    std::size_t mSize;

  private:
    static Encoding gDefaultEncoding;
  };

  /**
   * Concatenates two null-terminated UStrings
   */
  PDFOUT_EXPORTS UString operator+(pdfout::UString const &lhs, UString const &rhs);

  /**
   * Compares two UString objects as binary data, performs encoding conversion if required
   */
  PDFOUT_EXPORTS bool operator==(pdfout::UString const &lhs, UString const &rhs);
  PDFOUT_EXPORTS bool operator!=(pdfout::UString const &lhs, UString const &rhs);
}
