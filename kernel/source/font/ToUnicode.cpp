#include <font/ToUnicode.h>

#include <regex>
#include <font/AdobeGlyphList.h>

namespace{
  bool extractUnicode4(std::string const &glyphName, std::vector<uint32_t> &unicode);
  bool extractUnicode6(std::string const &glyphName, std::vector<uint32_t> &unicode);
  bool extractAGL(std::string const &glyphName, std::vector<uint32_t> &unicode);
}

namespace kernel{ namespace font{
  void toUnicode(uint16_t glyphId, std::string const &glyphName, std::vector<uint32_t> &unicode){

    if (extractAGL(glyphName, unicode))
      return;

    std::regex separator("[^_|\\.]+", std::regex_constants::ECMAScript);

    std::regex uniRegex("uni([0-9A-F]{4})+", std::regex_constants::ECMAScript);
    std::regex uRegex("u([0-9A-F]{4,6})+", std::regex_constants::ECMAScript);

    std::smatch match;
    auto begin = std::sregex_iterator(std::begin(glyphName), std::end(glyphName), separator);
    auto end = std::sregex_iterator();

    std::string subRegex;
    bool result = false;
    for (std::sregex_iterator i = begin; i != end; ++i){
      subRegex = i->str();

      if (extractAGL(subRegex, unicode))
        continue;

      auto sbegin = std::sregex_iterator(std::begin(subRegex), std::end(subRegex), uniRegex);
      auto send = std::sregex_iterator();

      for (std::sregex_iterator sub = sbegin; sub != send; ++sub){
        match = *sub;
        result = extractUnicode4(match.str(), unicode);
      }

      if (result)
        continue;

      sbegin = std::sregex_iterator(std::begin(subRegex), std::end(subRegex), uRegex);
      send = std::sregex_iterator();

      for (std::sregex_iterator sub = sbegin; sub != send; ++sub){
        match = *sub;
        result = extractUnicode6(match.str(), unicode);
      }
    }
  }

}}

namespace{

  bool extractUnicode4(std::string const &glyphName, std::vector<uint32_t> &unicode){
    std::regex unicode4("([0-9A-F]{4})", std::regex_constants::ECMAScript);
    auto begin = std::sregex_iterator(std::begin(glyphName), std::end(glyphName), unicode4);
    auto end = std::sregex_iterator();

    std::smatch match;
    uint32_t value = 0;
    bool result = false;
    for (std::sregex_iterator iter = begin; iter != end; ++iter){
      match = *iter;
      value = std::stoul(match.str().c_str(), nullptr, 16);
      if (value <= 0xD7FF || (value >= 0xE000 && value <= 0xFFFF)){
        unicode.push_back(value);
        result = true;
      }
    }

    return result;
  }

  bool extractUnicode6(std::string const &glyphName, std::vector<uint32_t> &unicode){
    std::regex unicode6("([0-9A-F]{4,6})", std::regex_constants::ECMAScript);
    auto begin = std::sregex_iterator(std::begin(glyphName), std::end(glyphName), unicode6);
    auto end = std::sregex_iterator();

    std::smatch match;
    uint32_t value = 0;
    bool result = false;
    for (std::sregex_iterator iter = begin; iter != end; ++iter){
      match = *iter;
      value = std::strtol(match.str().c_str(), nullptr, 16);
      if (value <= 0xD7FF || (value >= 0xE000 && value <= 0x10FFFF)){
        unicode.push_back(value);
        result = true;
      }
    }

    return result;
  }

  bool extractAGL(std::string const &glyphName, std::vector<uint32_t> &unicode){
    kernel::font::UnicodeInfo uniInfo = {0};
    kernel::font::getUnicodeByName(glyphName, uniInfo);
    for (std::size_t i = 0; i < uniInfo.mCount; ++i)
      unicode.push_back(uniInfo.mValue[i]);

    return uniInfo.mCount > 0;
  }
}
