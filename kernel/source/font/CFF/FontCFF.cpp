#include <font/CFF/FontCFF.h>

#include <font/CFF/CffIndex.h>
#include <stream/InputStream.h>
#include <font/CFF/TopDictionary.h>

namespace kernel{ namespace font{ namespace cff{

  FontCFF::FontCFF(pdfout::InputStream *stream){

    CffHeader header;
    stream->read(&header, sizeof(CffHeader), 1);

    mNameIndex = std::make_unique<Index>(stream);
    mTopDictionary = std::make_unique<TopDictionary>(stream);
    mStringIndex = std::make_unique<Index>(stream);
    mGlobalSubrIndex = std::make_unique<Index>(stream);

    std::string const str0 = mStringIndex->getString(0);
    std::string const str1 = mStringIndex->getString(1);
    std::string const str2 = mStringIndex->getString(2);

    auto version = mTopDictionary->getVersionSID(0);
    auto versionStr = mStringIndex->getString(version);

    auto notice = mTopDictionary->getNotice(0);
    auto copyright = mTopDictionary->getCopyrightSID(0);

    auto fullName = mTopDictionary->getFullNameSID(0);
    auto fullNameStr = mStringIndex->getString(fullName);

    auto familyName = mTopDictionary->getFamilyNameSID(0);
    auto familyNameStr = mStringIndex->getString(familyName);

    auto weight = mTopDictionary->getWeightSID(0);
    auto fixedPitch = mTopDictionary->isFixedPitch(0);
    auto italicAngle = mTopDictionary->getItalicAngle(0);
    auto underlinePos = mTopDictionary->getUnderlinePosition(0);
    auto underlineThick = mTopDictionary->getUnderlineThickness(0);
    auto paintType = mTopDictionary->getPaintType(0);
    auto charstringType = mTopDictionary->getCharstringType(0);
    auto fontMatrix = mTopDictionary->FontMatrix(0);
    auto uniqueId = mTopDictionary->getUniqueID(0);
    auto fontBBox = mTopDictionary->getFontBBox(0);
    auto stokeWidth = mTopDictionary->getStrokeWidth(0);
    auto xuid = mTopDictionary->getXUID(0);
    auto charset = mTopDictionary->getCharset(0);
    auto encoding = mTopDictionary->getEncoding(0);
    auto charStrings = mTopDictionary->getCharStrings(0);
    auto Private = mTopDictionary->getPrivate(0);
    auto synthetic = mTopDictionary->getSyntheticBase(0);
    auto postscript = mTopDictionary->getPostScriptSID(0);
    auto fontName = mTopDictionary->getBaseFontNameSID(0);
    auto fontBlend = mTopDictionary->getBaseFontBlendDelta(0);

  }

  FontCFF::~FontCFF() {}

}}}

