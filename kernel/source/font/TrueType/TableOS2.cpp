#include <font/TrueType/TableOS2.h>

#include <memory>
#include <Types.h>

using namespace pdfout;

namespace{
  using namespace kernel::font::truetype;

  std::size_t getOS2Size(uint16_t version){
    switch (version){
      case TableOS2Version0: return TableOS2VerSize0;
      case TableOS2Version1: return TableOS2VerSize1;
      case TableOS2Version2: return TableOS2VerSize2;
      case TableOS2Version3: return TableOS2VerSize3;
      case TableOS2Version4: return TableOS2VerSize4;
      case TableOS2Version5: return TableOS2VerSize5;
    }

    return 0;
  }
}

namespace kernel{ namespace font{ namespace truetype{

  TableOS2::TableOS2(void)
    : Table(){

    memset(&mData, 0, sizeof(TableOS2Data));
  }

  TableOS2::~TableOS2(void){
  }

  void TableOS2::load(pdfout::InputStream *stream){
    Endian const endian = getEndian();
    uint16_t version = 0;
    stream->read(&version, sizeof(uint16_t), 1);
    stream->seekg(stream->tellg() - sizeof(uint16_t));

    if (endian == EndianLittle)
      swapByteOrder(version);

    std::size_t const sizeofOS2 = getOS2Size(version);

    stream->read(&mData, sizeofOS2, 1);
    if (getEndian() == EndianLittle){
      if (version >= TableOS2Version0){
        swapByteOrder(mData.mVersion);
        swapByteOrder(mData.mXAvgCharWidth);
        swapByteOrder(mData.mUsWeightClass);
        swapByteOrder(mData.mUsWidthClass);
        swapByteOrder(mData.mFsType);
        swapByteOrder(mData.mYSubscriptXSize);
        swapByteOrder(mData.mYSubscriptYSize);
        swapByteOrder(mData.mYSubscriptXOffset);
        swapByteOrder(mData.mYSubscriptYOffset);
        swapByteOrder(mData.mYSuperscriptXSize);
        swapByteOrder(mData.mYSuperscriptYSize);
        swapByteOrder(mData.mYSuperscriptXOffset);
        swapByteOrder(mData.mYSuperscriptYOffset);
        swapByteOrder(mData.mYStrikeoutSize);
        swapByteOrder(mData.mYStrikeoutPosition);
        swapByteOrder(mData.mSFamilyClass);
        swapByteOrder(mData.mUlUnicodeRange1);
        swapByteOrder(mData.mUlUnicodeRange2);
        swapByteOrder(mData.mUlUnicodeRange3);
        swapByteOrder(mData.mUlUnicodeRange4);
        swapByteOrder(mData.mFsSelection);
        swapByteOrder(mData.mUsFirstCharIndex);
        swapByteOrder(mData.mUsLastCharIndex);
        swapByteOrder(mData.mTypoAscender);
        swapByteOrder(mData.mTypoDescender);
        swapByteOrder(mData.mTypoLineGap);
        swapByteOrder(mData.mUsWinAscent);
        swapByteOrder(mData.mUsWinDescent);
      }

      //if (version >= TableOS2Version1){
      //  swapByteOrder(mData.mUlCodePageRange1);
      //  swapByteOrder(mData.mUlCodePageRange2);
      //}

      if (version >= TableOS2Version2 || version >= TableOS2Version3 || version >= TableOS2Version4){
        swapByteOrder(mData.mXHeight);
        swapByteOrder(mData.mCapHeight);
        swapByteOrder(mData.mUsDefaultChar);
        swapByteOrder(mData.mUsBreakChar);
        swapByteOrder(mData.mUsMaxContext);
      }

      if (version >= TableOS2Version5){
        swapByteOrder(mData.mUsLowerOpticalPointSize);
        swapByteOrder(mData.mUsUpperOpticalPointSize);
      }
    }
  }

  void TableOS2::save(pdfout::OutputStream *stream) const{
    TableOS2Data data;
    memcpy(&data, &mData, sizeof(TableOS2Data));

    uint16_t const version = mData.mVersion;

    if (getEndian() == EndianLittle){
      if (version >= TableOS2Version0){
        swapByteOrder(data.mVersion);
        swapByteOrder(data.mXAvgCharWidth);
        swapByteOrder(data.mUsWeightClass);
        swapByteOrder(data.mUsWidthClass);
        swapByteOrder(data.mFsType);
        swapByteOrder(data.mYSubscriptXSize);
        swapByteOrder(data.mYSubscriptYSize);
        swapByteOrder(data.mYSubscriptXOffset);
        swapByteOrder(data.mYSubscriptYOffset);
        swapByteOrder(data.mYSuperscriptXSize);
        swapByteOrder(data.mYSuperscriptYSize);
        swapByteOrder(data.mYSuperscriptXOffset);
        swapByteOrder(data.mYSuperscriptYOffset);
        swapByteOrder(data.mYStrikeoutSize);
        swapByteOrder(data.mYStrikeoutPosition);
        swapByteOrder(data.mSFamilyClass);
        swapByteOrder(data.mUlUnicodeRange1);
        swapByteOrder(data.mUlUnicodeRange2);
        swapByteOrder(data.mUlUnicodeRange3);
        swapByteOrder(data.mUlUnicodeRange4);
        swapByteOrder(data.mFsSelection);
        swapByteOrder(data.mUsFirstCharIndex);
        swapByteOrder(data.mUsLastCharIndex);
        swapByteOrder(data.mTypoAscender);
        swapByteOrder(data.mTypoDescender);
        swapByteOrder(data.mTypoLineGap);
        swapByteOrder(data.mUsWinAscent);
        swapByteOrder(data.mUsWinDescent);
      }

      //if (version >= TableOS2Version1){
      //  swapByteOrder(data.mUlCodePageRange1);
      //  swapByteOrder(data.mUlCodePageRange2);
      //}

      if (version >= TableOS2Version2 || version >= TableOS2Version3 || version >= TableOS2Version4){
        swapByteOrder(data.mXHeight);
        swapByteOrder(data.mCapHeight);
        swapByteOrder(data.mUsDefaultChar);
        swapByteOrder(data.mUsBreakChar);
        swapByteOrder(data.mUsMaxContext);
      }

      if (version >= TableOS2Version5){
        swapByteOrder(data.mUsLowerOpticalPointSize);
        swapByteOrder(data.mUsUpperOpticalPointSize);
      }
    }

    stream->write(&data, getOS2Size(version), 1);
  }

  std::string TableOS2::getWidthClass(void) const{
    switch (mData.mUsWidthClass){
      case WidthClassUltraCondensed: return "UltraCondensed";
      case WidthClassExtraCondensed: return "ExtraCondensed";
      case WidthClassCondensed: return "Condensed";
      case WidthClassSemiCondensed: return "SemiCondensed";
      case WidthClassMedium: return "Normal";
      case WidthClassSemiExpanded: return "SemiExpanded";
      case WidthClassExpanded: return "Expanded";
      case WidthClassExtraExpanded: return "ExtraExpanded";
      case WidthClassUltraExpanded: return "UltraExpanded";
    }

    return "";
  }

  uint16_t TableOS2::getWeightClass(void) const {return mData.mUsWeightClass;}
  int16_t TableOS2::getAscent(void) const {return mData.mTypoAscender;}
  int16_t TableOS2::getWinAscent(void) const {return mData.mUsWinAscent;}
  int16_t TableOS2::getDescent(void) const {return mData.mTypoDescender;}
  int16_t TableOS2::getWinDescent(void) const {return mData.mUsWinDescent;}
  int16_t TableOS2::getCapHeight(void) const {return mData.mCapHeight;}
  int16_t TableOS2::getLineGap(void) const {return mData.mTypoLineGap;}
  uint8_t TableOS2::getFontClassId(void) const {return (mData.mSFamilyClass & 0xFF00) >> 8;}
  uint8_t TableOS2::getFontSubclassId(void) const {return mData.mSFamilyClass & 0x00FF;}
  uint16_t TableOS2::getFirstCharIndex(void) const {return mData.mUsFirstCharIndex;}
  uint16_t TableOS2::getLastCharIndex(void) const {return mData.mUsLastCharIndex;}
  uint16_t TableOS2::getBreakChar(void) const {return mData.mUsBreakChar;}
  uint16_t TableOS2::getDefaultChar(void) const {return mData.mUsDefaultChar;}
  uint16_t TableOS2::getUnderlineYSize(void) const {return mData.mYStrikeoutSize;}

  bool TableOS2::isSerif(void) const{
    uint8_t fontClass = getFontClassId();
    switch (fontClass){
      case FontClassIdOldstyleSerifs:
      case FontClassIdTransitionalSerifs:
      case FontClassIdModernSerifs:
      case FontClassIdClarendonSerifs:
      case FontClassIdSlabSerifs:
      case FontClassIdFreeformSerifs:
        return true;
    }

    return false;
  }

  bool TableOS2::isScript(void) const{
    return getFontClassId() == FontClassIdScripts;
  }

  bool TableOS2::isSymbolic(void) const{
    return getFontClassId() == FontClassIdSymbolic;
  }

  std::unique_ptr<TableOS2> TableOS2::createSubset(FontInfo const &fontInfo, uint16_t defaultChar, uint16_t breakChar) const{
    std::unique_ptr<TableOS2> os2 = std::make_unique<TableOS2>(*this);
    os2->mData.mUsBreakChar = breakChar;
    os2->mData.mUsDefaultChar = defaultChar;
    return os2;
  }

}}}
