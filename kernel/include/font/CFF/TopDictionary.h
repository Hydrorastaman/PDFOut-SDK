#pragma

#include <font/CFF/CffDictionary.h>

#include <Types.h>
#include <TransformationMatrix.h>

namespace kernel{ namespace font{ namespace cff{

  class TopDictionary : public Dictionary{
  public:
    TopDictionary(pdfout::InputStream *stream);
    ~TopDictionary() noexcept;
    TopDictionary(TopDictionary const &) = delete;
    TopDictionary &operator=(TopDictionary const &) = delete;

    SID getVersionSID(std::size_t index) const noexcept;
    SID getNotice(std::size_t index) const noexcept;
    SID getCopyrightSID(std::size_t index) const noexcept;
    SID getFullNameSID(std::size_t index) const noexcept;
    SID getFamilyNameSID(std::size_t index) const noexcept;
    SID getWeightSID(std::size_t index) const noexcept;
    bool isFixedPitch(std::size_t index) const noexcept;
    float getItalicAngle(std::size_t index) const noexcept;
    int32_t getUnderlinePosition(std::size_t index) const noexcept;
    int32_t getUnderlineThickness(std::size_t index) const noexcept;
    int32_t getPaintType(std::size_t index) const noexcept;
    int32_t getCharstringType(std::size_t index) const noexcept;
    pdfout::TransformationMatrix FontMatrix(std::size_t index) const noexcept;
    int32_t getUniqueID(std::size_t index) const noexcept;
    pdfout::Rectangle getFontBBox(std::size_t index) const noexcept;
    int32_t getStrokeWidth(std::size_t index) const noexcept;
    std::vector<int32_t> getXUID(std::size_t index) const noexcept;
    int32_t getCharset(std::size_t index) const noexcept;
    int32_t getEncoding(std::size_t index) const noexcept;
    int32_t getCharStrings(std::size_t index) const noexcept;
    SizeOffset getPrivate(std::size_t index) const noexcept;
    int32_t getSyntheticBase(std::size_t index) const noexcept;
    SID getPostScriptSID(std::size_t index) const noexcept;
    SID getBaseFontNameSID(std::size_t index) const noexcept;
    std::vector<int32_t> getBaseFontBlendDelta(std::size_t index) const noexcept;
  };

}}}

