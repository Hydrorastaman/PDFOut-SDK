#include <font/CFF/TopDictionary.h>
#include <stream/InputStream.h>

namespace kernel{ namespace font{ namespace cff{
  TopDictionary::TopDictionary(pdfout::InputStream *stream)
    : Dictionary(stream){
  }

  TopDictionary::~TopDictionary() noexcept {}

  SID TopDictionary::getVersionSID(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::version, InvalidSID);
  }

  SID TopDictionary::getNotice(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::Notice, InvalidSID);
  }

  SID TopDictionary::getCopyrightSID(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::Copyright, InvalidSID);
  }

  SID TopDictionary::getFullNameSID(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::FullName, InvalidSID);
  }

  SID TopDictionary::getFamilyNameSID(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::FamilyName, InvalidSID);
  }

  SID TopDictionary::getWeightSID(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::Weight, InvalidSID);
  }

  bool TopDictionary::isFixedPitch(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::isFixedPitch, false);
  }

  float TopDictionary::getItalicAngle(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::ItalicAngle, 0.f);
  }

  int32_t TopDictionary::getUnderlinePosition(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::UnderlinePosition, -100);
  }

  int32_t TopDictionary::getUnderlineThickness(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::UnderlineThickness, 50);
  }

  int32_t TopDictionary::getPaintType(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::PaintType, 0);
  }

  int32_t TopDictionary::getCharstringType(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::CharstringType, 2);
  }

  pdfout::TransformationMatrix TopDictionary::FontMatrix(std::size_t index) const noexcept{
    std::vector<float> const default = {0.001f, 0.f, 0.f, 0.001f, 0.f, 0.f};
    std::vector<float> const value = getFirstOperandOrDefault(index, Operator::FontMatrix, default);
    return pdfout::TransformationMatrix(default.data());
  }

  int32_t TopDictionary::getUniqueID(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::UniqueID, InvalidID);
  }

  pdfout::Rectangle TopDictionary::getFontBBox(std::size_t index) const noexcept{
    std::vector<float> const default = {0, 0, 0, 0};
    std::vector<float> const value = getFirstOperandOrDefault(index, Operator::FontBBox, default);
    return {value[0], value[1], value[2], value[3]};
  }

  int32_t TopDictionary::getStrokeWidth(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::StrokeWidth, 0);
  }

  std::vector<int32_t> TopDictionary::getXUID(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::XUID, std::vector<int32_t>());
  }

  int32_t TopDictionary::getCharset(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::charset, 0);
  }

  int32_t TopDictionary::getEncoding(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::Encoding, 0);
  }

  int32_t TopDictionary::getCharStrings(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::CharStrings, 0);
  }

  SizeOffset TopDictionary::getPrivate(std::size_t index) const noexcept{
    std::vector<int32_t> const default = {0, 0};
    std::vector<int32_t> const value = getFirstOperandOrDefault(index, Operator::Private, default);
    return {value[0], value[1]};
  }

  int32_t TopDictionary::getSyntheticBase(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::SyntheticBase, InvalidIndex);
  }

  SID TopDictionary::getPostScriptSID(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::PostScript, InvalidSID);
  }

  SID TopDictionary::getBaseFontNameSID(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::BaseFontName, InvalidSID);
  }

  std::vector<int32_t> TopDictionary::getBaseFontBlendDelta(std::size_t index) const noexcept{
    return getFirstOperandOrDefault(index, Operator::BaseFontBlend, std::vector<int32_t>());
  }

}}}

