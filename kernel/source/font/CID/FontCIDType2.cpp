#include <font/CID/FontCIDType2.h>

#include <object/ObjectArray.h>
#include <object/ObjectStream.h>
#include <object/ObjectInteger.h>
#include <object/ObjectRectangle.h>
#include <object/ObjectByteString.h>

#include <font/ToUnicodeCMapStream.h>
#include <font/TrueType/FontTrueType.h>
#include <font/CIDSystemInfoDictionary.h>
#include <font/FontDescriptorDictionary.h>

#include <font/FontFileStream.h>

namespace kernel{ namespace font{ namespace cid{

  FontCIDType2::FontCIDType2(FontTrueType const *descendantFont, pdfout::WritingMode writingMode)
    : CompositeFont(descendantFont, writingMode){
  }

  FontCIDType2::~FontCIDType2(void) noexcept{
  }

  std::unique_ptr<kernel::ObjectIndirectReference> FontCIDType2::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
    std::unique_ptr<FontType0Dictionary> fontDictionary(makeFontDictionary());
    std::unique_ptr<FontCIDDictionary> cidFontDictionary(makeCIDFontDictionary());

    std::unique_ptr<ObjectArray> descendantArray = std::make_unique<ObjectArray>(IndirectTypeNonIndirectable);
    descendantArray->insert(cidFontDictionary->getReference());
    fontDictionary->addKey(FontType0DictionaryKeyDescendantFonts, std::move(descendantArray));

    FontTrueType const *ttf = dynamic_cast<FontTrueType const *>(getDescendantFont());
    if (!ttf)
      RUNTIME_EXCEPTION("Descendant font is not a valid TrueType font.");

    std::unique_ptr<FontDescriptorDictionary> fontDescriptor(ttf->makeFontDescriptor());
    cidFontDictionary->addKey(FontCIDDictionaryKeyFontDescriptor, fontDescriptor->getReference());

    if (params->mIsFontEmbedded){ // embed font program
      std::unique_ptr<FontFileStream> fileStream = std::make_unique<FontFileStream>();
      ttf->saveToStream(fileStream->getStream());
      fileStream->addKey(FontFileStreamKeyLength1, std::make_unique<ObjectInteger>(fileStream->getStream()->getSize()));
      fontDescriptor->addKey(FontDescriptorDictionaryKeyFontFile2, std::move(fileStream));
    }

    fontDictionary->serialize(stream, params);
    cidFontDictionary->serialize(stream, params);
    fontDescriptor->serialize(stream, params);

    return fontDictionary->getReference();
  }

  std::unique_ptr<FontType0Dictionary> FontCIDType2::makeFontDictionary(void) const{
    std::unique_ptr<FontType0Dictionary> type0Dict = std::make_unique<FontType0Dictionary>();

    FontTrueType const *ttf = dynamic_cast<FontTrueType const *>(getDescendantFont());
    if (!ttf)
      RUNTIME_EXCEPTION("Descendant font is not a valid TrueType font.");

    pdfout::UString const postScriptName = ttf->getPostScriptName();
    type0Dict->addKey(FontType0DictionaryKeyBaseFont, std::make_unique<ObjectName>(postScriptName.getData(), postScriptName.getSize()));

    std::unique_ptr<ToUnicodeCMapStream> toUnicodeStream(ttf->toUnicode());
    toUnicodeStream->setCMapInfo(getCMapName());
    toUnicodeStream->setCMapVersion(1);
    toUnicodeStream->setCMapType(2);

    type0Dict->addKey(FontType0DictionaryKeyToUnicode, std::move(toUnicodeStream));
    type0Dict->addKey(FontType0DictionaryKeyEncoding, std::make_unique<ObjectName>(cmapNameToString(getCMapName())));

    return type0Dict;
  }

  std::unique_ptr<FontCIDDictionary> FontCIDType2::makeCIDFontDictionary(void) const{
    std::unique_ptr<FontCIDDictionary> cidDictionary = std::make_unique<FontCIDDictionary>(CIDFontType2);

    FontTrueType const *ttf = dynamic_cast<FontTrueType const *>(getDescendantFont());
    if (!ttf)
      RUNTIME_EXCEPTION("Descendant font is not a valid TrueType font.");

    pdfout::UString const postScriptName = ttf->getPostScriptName();
    cidDictionary->addKey(FontCIDDictionaryKeyBaseFont, std::make_unique<ObjectName>(postScriptName.getData(), postScriptName.getSize()));

    std::unique_ptr<CIDSystemInfoDictionary> systemInfo = std::make_unique<CIDSystemInfoDictionary>(getCMapName());
    cidDictionary->addKey(FontCIDDictionaryKeyCIDSystemInfo, std::move(systemInfo));

    std::unique_ptr<ObjectStream> cid2gid(ttf->cid2gid());
    cidDictionary->addKey(FontCIDDictionaryKeyCIDToGIDMap, std::move(cid2gid));

    std::unique_ptr<ObjectArray> toW(ttf->toW());
    cidDictionary->addKey(FontCIDDictionaryKeyW, std::move(toW));

    return cidDictionary;
  }

}}}
