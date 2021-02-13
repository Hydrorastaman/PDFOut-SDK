#pragma once

#include <deque>
#include <memory>
#include <string>

#include <Font.h>
#include <FontCollectionImpl.h>

#include <font/CompositeFont.h>

#include <stream/InputStream.h>
#include <stream/OutputStream.h>

namespace kernel{
  class ObjectIndirectReference;
  struct SerializeParams;

  template<typename T>
  class UserObjectHash;
}

namespace pdfout{ namespace impl{

  class FontImpl : public Font{
  protected:
    FontImpl(std::unique_ptr<kernel::font::CompositeFont> compositeFont, uint16_t version);

  public:
    ~FontImpl(void) noexcept;

    /**
     * Font interface implementation
     */
    static void addFontPath(pdfout::UString const &fontPath);
    static void setFontPath(pdfout::UString const &fontPath);
    static FontImpl *createFont(pdfout::UString const &fileName, WritingMode writingMode);
    static FontImpl *createFont(void const *fontData, uint64_t size, WritingMode writingMode);
    static FontImpl *createFont(InputStream *istream, WritingMode writingMode);

    static FontCollectionImpl *createFontCollection(pdfout::UString const &fileName, WritingMode writingMode);
    static FontCollectionImpl *createFontCollection(void const *fontData, uint64_t size, WritingMode writingMode);
    static FontCollectionImpl *createFontCollection(InputStream *istream, WritingMode writingMode);

    uint16_t getVersion(void) const;
    pdfout::WritingMode getWritingMode(void) const;

    UString getRevision(void) const;

    UString getPostScriptName(void) const;
    UString getFamilyName(void) const;
    UString getSubFamilyName(void) const;

    void saveToStream(pdfout::OutputStream *stream) const;

    void toCharCodes(void const *glyphIndices, uint16_t elemSize, uint64_t count, pdfout::UString &charCodes, uint32_t defaultValue = 0) const;
    void toText(void const *glyphIndices, uint16_t elemSize, uint64_t count, pdfout::UString &text, uint32_t defaultValue = 0) const;
    int32_t getTextWidth(pdfout::UString const &text) const;
    int32_t getCharWidth(uint16_t cid) const;
    void getCIDMetrics(uint16_t cid, CIDMetrics &metrics) const;
    int32_t getKerningWidth(uint16_t cidLhs, uint16_t cidRhs) const;

    int32_t getLineGap(void) const;
    int32_t getAscent(void) const;
    int32_t getDescent(void) const;
    int32_t getCapHeight(void) const;
    int32_t getUnitsPerEm(void) const;

    int32_t getWinAscent(void) const;
    int32_t getWinDescent(void) const;
    int32_t getWinInternalLeading(void) const;
    int32_t getWinExternalLeading(void) const;
    int32_t getBreakChar(void) const;
    int32_t getDefaultChar(void) const;
    int32_t getUnderlineYSize(void) const;

    bool isBold(void) const;
    bool isItalic(void) const;
    bool isUnderline(void) const;
    bool isShadow(void) const;
    bool isCondensed(void) const;
    bool isExtended(void) const;

    Font *merge(Font const *delta) const;

    /**
     * Internal API
     */

    /**
     * Gets font resource unique ID.
     */
    std::string const &getUID(void) const;

    int32_t getTextWidth(pdfout::UString const &text, std::vector<int32_t> &widths) const;

    /**
     * Serializes object to stream as a PDF object.
     */
    std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

    /**
     * Creates font subset with specified CIDs.
     */
    std::unique_ptr<FontImpl> createSubset(std::vector<uint32_t> const &cids) const;

    /**
     * Creates font superset with specified font.
     */
    std::unique_ptr<FontImpl> createSuperset(FontImpl const *font) const;

  private:
    FontImpl(FontImpl const &) = delete;
    FontImpl &operator=(FontImpl const &) = delete;

    static std::string getSHA256Ex(std::string const &sha256, WritingMode writingMode);
    static std::string getSHA256Ex(std::string const &sha256, uint32_t index);

    static std::unique_ptr<InputStream> makeInputStream(pdfout::UString const &fileName);

  private:
    std::string mUID;
    uint16_t mVersion;
    std::unique_ptr<kernel::font::CompositeFont> mCompositeFont;

    static std::deque<UString> mFontPath; /**< UTF_8 encoded font path */
    static std::unique_ptr<kernel::UserObjectHash<FontImpl>> mFonts;
    static std::unique_ptr<kernel::UserObjectHash<kernel::font::SimpleFont>> mSimpleFonts;
  };

}}
