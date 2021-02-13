#include <font/ToUnicodeCMapStream.h>

#include <iomanip>
#include <sstream>
#include <algorithm>

#include <InternalTools.h>

#undef min
#undef max

using namespace kernel;

namespace{
    using kernel::font::UnicodeInfo;

    struct Range{
      uint16_t mCharCodeBegin;
      uint16_t mCharCodeEnd;
      uint16_t mUnicodeBegin;
      
      Range(uint16_t charCodeBegin, uint16_t unicodeBegin, uint16_t count)
        : mCharCodeBegin(charCodeBegin),
        mCharCodeEnd(charCodeBegin + count - 1),
        mUnicodeBegin(unicodeBegin){
      }
    };

  void splitToRanges(std::map<uint16_t, uint16_t> const &charToUnicode,  std::deque<Range> &bfRange, std::deque<Range> &bfChar);
}

namespace kernel{ namespace font{

  ToUnicodeCMapStream::ToUnicodeCMapStream(void)
    : ObjectStream(),
    mRegistry(""),
    mOrdering(""),
    mCMapName(""),
    mSupplement(0),
    mVersion(0),
    mType(0),
    mCharToUnicode(){
  }

  ToUnicodeCMapStream::~ToUnicodeCMapStream(void) {}

  void ToUnicodeCMapStream::setRegistry(std::string const &registry) {mRegistry = registry;}

  void ToUnicodeCMapStream::setOrdering(std::string const &ordering) {mOrdering = ordering;}

  void ToUnicodeCMapStream::setSupplement(uint16_t supplement) {mSupplement = supplement;}

  void ToUnicodeCMapStream::setCMapName(std::string const &cmapName) {mCMapName = cmapName;}

  void ToUnicodeCMapStream::setCMapInfo(CMapName cmapName){
    CMapNameInfo cmapInfo = getCMapNameInfo(cmapName);
    mRegistry = cmapInfo.mRegistry;
    mOrdering = cmapInfo.mOrdering;
    mSupplement = cmapInfo.mSupplement;
    mCMapName = cmapNameToString(cmapName);
  }

  void ToUnicodeCMapStream::setCMapVersion(uint16_t version) {mVersion = version;}

  void ToUnicodeCMapStream::setCMapType(uint16_t type) {mType = type;}

  void ToUnicodeCMapStream::addBfChar(uint16_t charCode, UnicodeInfo const &uniInfo){
    if (uniInfo.mCount == 1)
      mCharToUnicode[charCode] = uniInfo.mValue[0];
    else if (uniInfo.mCount > 1)
      mCharToUnicodeMulti[charCode] = uniInfo;
  }

  void ToUnicodeCMapStream::addBfChar(uint16_t charCode, std::vector<uint32_t> const &unicode){
    UnicodeInfo uniInfo = {0};
    uniInfo.mCount = std::min((uint8_t) 4, (uint8_t) unicode.size());
    for (uint8_t i = 0; i < uniInfo.mCount; ++i)
      uniInfo.mValue[i] = (uint16_t) unicode[i];

    addBfChar(charCode, uniInfo);
  }

  void ToUnicodeCMapStream::beforeSerialize(void) const{
    Endian const endian = getEndian();
    std::stringstream sstream;

    sstream << "/CIDInit /ProcSet findresource begin\r\n"
                "12 dict begin\r\n"
                "begincmap\r\n"
                "/CIDSystemInfo <<\r\n"
                "/Registry (" << mRegistry << ")\r\n"
                "/Ordering (" << mOrdering << ")\r\n"
                "/Supplement " << mSupplement << "\r\n"
                ">> def\r\n"
                "/CMapName /" << mCMapName << " def\r\n"
                "/CMapType " << mType << " def\r\n"
                "1 begincodespacerange\r\n"
                "<0000> <FFFF>\r\n"
                "endcodespacerange\r\n";

    std::deque<Range> bfRange;
    std::deque<Range> bfChar;
    splitToRanges(mCharToUnicode, bfRange, bfChar);

    std::size_t size = 0;
    if (!bfRange.empty()){
      auto iter = std::begin(bfRange);
      auto end = std::end(bfRange);
      std::size_t count = 0;

      size = std::min(int(std::distance(iter, end)), 100);
      sstream << size << " beginbfrange\r\n";
      for (; iter != end; ++iter){
        sstream << "<" << std::setfill('0') << std::setw(4) << std::hex << iter->mCharCodeBegin << "> <" <<
                          std::setfill('0') << std::setw(4) << std::hex << iter->mCharCodeEnd << "> <" <<
                          std::setfill('0') << std::setw(4) << std::hex << iter->mUnicodeBegin << ">\r\n";
        ++count;

        if (count == 100){
          sstream << "endbfrange\r\n";
          size = std::min(int(std::distance(iter, end) - 1), 100);
          sstream << std::dec << size << " beginbfrange\r\n";
          count = 0;
        }
      }

      sstream << "endbfrange\r\n";
    }

    if (!bfChar.empty()){
      auto iter = std::begin(bfChar);
      auto end = std::end(bfChar);
      std::size_t count = 0;

      size = std::min(int(std::distance(iter, end)), 100);
      sstream << std::dec << size << " beginbfchar\r\n";

      for (; iter != end; ++iter){
        sstream << "<" << std::setfill('0') << std::setw(4) << std::hex << iter->mCharCodeBegin << "> <" <<
                          std::setfill('0') << std::setw(4) << std::hex << iter->mUnicodeBegin << ">\r\n";

        ++count;
        if (count == 100){
          sstream << "endbfchar\r\n";
          size = std::min(int(std::distance(iter, end) - 1), 100);
          sstream << std::dec << size << " beginbfchar\r\n";
          count = 0;
        }
      }

      sstream << "endbfchar\r\n";
    }

    if (!mCharToUnicodeMulti.empty()){
      auto iter = std::begin(mCharToUnicodeMulti);
      auto end = std::end(mCharToUnicodeMulti);
      std::size_t count = 0;

      size = std::min(int(std::distance(iter, end)), 100);
      sstream << std::dec << size << " beginbfchar\r\n";

      for (; iter != end; ++iter){
        sstream << "<" << std::setfill('0') << std::setw(4) << std::hex << iter->first << "> <";
        for (auto i = 0; i < iter->second.mCount; ++i)
          sstream << std::setfill('0') << std::setw(4) << std::hex << iter->second.mValue[i];
        sstream << ">\r\n";

        ++count;
        if (count == 100){
          sstream << "endbfchar\r\n";
          size = std::min(int(std::distance(iter, end) - 1), 100);
          sstream << std::dec << size << " beginbfchar\r\n";
          count = 0;
        }
      }

      sstream << "endbfchar\r\n";
    }

    sstream << "endcmap\r\n"
               "CMapName currentdict /CMap defineresource pop\r\n"
               "end\r\n"
               "end\r\n";

    writeToStream(sstream.str());
  }

  void ToUnicodeCMapStream::writeToStream(std::string const &str) const{
    mStreamData->write(str.c_str(), sizeof(uint8_t), str.length());
  }

}}

namespace{

  void splitToRanges(std::map<uint16_t, uint16_t> const &charToUnicode,  std::deque<Range> &bfRange, std::deque<Range> &bfChar){
    if (charToUnicode.empty())
      return;

    auto begin = std::begin(charToUnicode);
    uint16_t charCodeBegin = begin->first;
    uint16_t unicodeBegin = begin->second;
    uint16_t count = 1;

    auto iter = std::begin(charToUnicode);
    auto const end = std::end(charToUnicode);
    std::advance(iter, 1);
    for (; iter != end; ++iter){
      if ((iter->first == charCodeBegin + count) &&
          (iter->second == unicodeBegin + count) &&
          ((iter->first & 0xFF00) == (charCodeBegin & 0xFF00))){
        ++count;
      }
      else{
        if (count == 1)
          bfChar.push_back(Range(charCodeBegin, unicodeBegin, 1));
        else
          bfRange.push_back(Range(charCodeBegin, unicodeBegin, count));

        charCodeBegin = iter->first;
        unicodeBegin = iter->second;
        count = 1;
      }
    }

    if (count == 1)
      bfChar.push_back(Range(charCodeBegin, unicodeBegin, 1));
    else
      bfRange.push_back(Range(charCodeBegin, unicodeBegin, count));
  }

}
