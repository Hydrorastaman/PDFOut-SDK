#pragma once

#include <vector>
#include <string>
#include <memory>
#include <cstdint>

#include <UString.h>
#include <font/TrueType/Table.h>

namespace pdfout{

  class InputStream;
  class OutputStream;

}

namespace kernel{ namespace font{ namespace truetype{

  /**
   * @brief Records in the names table
   */
  struct NameRecord{
    uint16_t mPlatformID;       /**< Platform ID, see Types.h */
    uint16_t mEncodingID;       /**< Platform-specific encoding ID, see Types.h */
    uint16_t mLanguageID;       /**< Language ID */
    uint16_t mNameID;           /**< Name ID */
    std::vector<uint8_t> mName; /**< Name */
  };

  /**
   * @brief Name IDs description
   */
  enum NameId{
    NameIdCopyright = 0,           /**< Copyright notice. */
    NameIdFontFamilyName = 1,      /**< Font Family name. Up to four fonts can share the Font Family name, forming a font style linking group (regular, italic, bold, bold italic - as defined by OS/2.fsSelection bit settings). */
    NameIdFontSubfamilyName = 2,   /**< Font Subfamily name. The Font Subfamily name distiguishes the font in a group with the same Font Family name (name ID 1). This is assumed to address style (italic, oblique) and weight (light, bold, black, etc.).
                                        A font with no particular differences in weight or style (e.g. medium weight, not italic and fsSelection bit 6 set) should have the string “Regular” stored in this position. */
    NameIdUniqueFontId = 3,        /**< Unique font identifier */
    NameIdFullFontName = 4,        /**< Full font name; a combination of strings 1 and 2, or a similar human-readable variant. If string 2 is "Regular", it is sometimes omitted from name ID 4. */
    NameIdVersionString = 5,       /**< Version string. Should begin with the syntax 'Version <number>.<number>' (upper case, lower case, or mixed, with a space between “Version” and the number).
                                        The string must contain a version number of the following form: one or more digits (0-9) of value less than 65,535, followed by a period,
                                        followed by one or more digits of value less than 65,535. Any character other than a digit will terminate the minor number.
                                        A character such as “;” is helpful to separate different pieces of version information.
                                        The first such match in the string can be used by installation software to compare font versions. Note that some installers may require the string to start with “Version ”, followed by a version number as above. */
    NameIdPostScriptName = 6,      /**< Postscript name for the font; Name ID 6 specifies a string which is used to invoke a PostScript language font that corresponds to this OFF font.
                                        When translated to ASCII, the name string must be no longer than 63 characters and restricted to the printable ASCII subset, codes 33-126,
                                        except for the 10 characters '[', ']', '(', ')', '{', '}', '<', '>', '/', '%'. See 'name' table section of clause 7 "Recommendations for OFF fonts" for additional information. */
    NameIdTrademark = 7,           /**< Trademark; this is used to save any trademark notice/information for this font. Such information should be based on legal advice. This is distinctly separate from the copyright. */
    NameIdManufacturerName = 8,    /**< Manufacturer Name. */
    NameIdDesigner = 9,            /**< Designer; name of the designer of the typeface. */
    NameIdDescription = 10,        /**< Description; description of the typeface. Can contain revision information, usage recommendations, history, features, etc. */
    NameIdVendorURL = 11,          /**< URL Vendor; URL of font vendor (with protocol, e.g., http://, ftp://). If a unique serial number is embedded in the URL, it can be used to register the font. */
    NameIdDesignerURL = 12,        /**< URL Designer; URL of typeface designer (with protocol, e.g., http://, ftp://). */
    NameIdLicenseDescription = 13, /**< License Description; description of how the font may be legally used, or different example scenarios for licensed use. This field should be written in plain language, not legalese. */
    NameIdLicenseInfoURL = 14,     /**< License Info URL; URL where additional licensing information can be found. */
    NameIdTypographicFamilyName = 16,    /**< Typographic Family name: The typographic family grouping doesn't impose any constraints on the number of faces within it, in contrast with the 4-style family grouping (ID 1),
                                              which is present both for historical reasons and to express style linking groups. If name ID 16 is absent, then name ID 1 is considered to be the typographic family name.
                                              (In earlier versions of the specification, name ID 16 was known as "Preferred Family".) */
    NameIdTypographicSubfamilyName = 17, /**< Typographic Subfamily name: This allows font designers to specify a subfamily name within the typographic family grouping.
                                              This string must be unique within a particular typographic family. If it is absent, then name ID 2 is considered to be the typographic subfamily name.
                                              (In earlier versions of the specification, name ID 17 was known as "Preferred Subfamily".) */
    NameIdCompatibleFull = 18,      /**< Compatible Full (Macintosh only); On the Macintosh, the menu name is constructed using the FOND resource. This usually matches the Full Name. If you want the name of the font to appear differently than the Full Name, you can insert the Compatible Full Name in ID 18. */
    NameIdSampleText = 19,          /**< Sample text; This can be the font name, or any other text that the designer thinks is the best sample to display the font in. */
    NameIdPostScriptCID = 20,       /**< PostScript CID findfont name; Its presence in a font means that the nameID 6 holds a PostScript font name that is meant to be used with the “composefont” invocation in order to invoke the font in a PostScript interpreter. See the definition of name ID 6.
                                         The value held in the name ID 20 string is interpreted as a PostScript font name that is meant to be used with the “findfont” invocation, in order to invoke the font in a PostScript interpreter.
                                         When translated to ASCII, this name string must be restricted to the printable ASCII subset, codes 33 through 126, except for the 10 characters: '[', ']', '(', ')', '{', '}', '<', '>', '/', '%'.
                                         See "Recommendations for OTF fonts" for additional information */
    NameIdWWSFamilyName = 21,       /**< WWS Family Name. Used to provide a WWS-conformant family name in case the entries for IDs 16 and 17 do not conform to the WWS model. (That is, in case the entry for ID 17 includes qualifiers for some attribute other than weight, width or slope.).
                                        If bit 8 of the fsSelection field is set, a WWS Family Name entry should not be needed and should not be included. Conversely, if an entry for this ID is include, bit 8 should not be set. (See OS/2 'fsSelection' field for details.).
                                        Examples of name ID 21: “Minion Pro Caption” and “Minion Pro Display”. (Name ID 16 would be “Minion Pro” for these examples.) */
    NameIdWWSSubfamilyName = 22,    /**< WWS Subfamily Name. Used in conjunction with ID 21, this ID provides a WWS-conformant subfamily name (reflecting only weight, width and slope attributes) in case the entries for IDs 16 and 17 do not conform to the WWS model.
                                         As in the case of ID 21, use of this ID should correlate inversely with bit 8 of the fsSelection field being set. Examples of name ID 22: “Semibold Italic”, “Bold Condensed”. (Name ID 17 could be “Semibold Italic Caption”, or “Bold Condensed Display”, for example.) */
    NameIdLightBackgroundPalette = 23, /**< Light Backgound Palette. This ID, if used in the CPAL table's Palette Labels Array, specifies that the corresponding color palette in the CPAL table is appropriate to use with the font when displaying it on a light background such as white. Name table strings for this ID specify the user interface strings associated with this palette. */
    NameIdDarkBackgroundPalette = 24   /**< Dark Backgound Palette. This ID, if used in the CPAL table's Palette Labels Array, specifies that the corresponding color palette in the CPAL table is appropriate to use with the font when displaying it on a dark background such as black. Name table strings for this ID specify the user interface strings associated with this palette */
  };


  class TableName : public Table{
  public:
    TableName(void);
    ~TableName(void);

    void load(pdfout::InputStream *stream);
    void save(pdfout::OutputStream *stream) const;

    NameRecord const *getNameRecord(NameId nameId) const;
    pdfout::UString getName(NameId nameId) const;
    pdfout::UString getPostScriptName(void) const;
    pdfout::UString getFamilyName(void) const;
    pdfout::UString getSubFamilyName(void) const;

    std::unique_ptr<TableName> createSubset(void) const;

  private:
    pdfout::Encoding getEncoding(uint16_t platformId, uint16_t encodingId) const;

  private:
    //TableName(TableName const &) = delete;
    TableName &operator=(TableName const &) = delete;

  public:
    uint16_t mFormat;                        /**< Format selector, 0 or 1 */
    std::vector<NameRecord> mNameRecord;     /**< The name records */
    std::vector<std::vector<uint8_t>> mLangTagRecord; /**< The language-tag records for mFormat == 1 */
  };
}}}
