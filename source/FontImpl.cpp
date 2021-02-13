#include <FontImpl.h>

#include <ResourceUid.h>
#include <UserObjectHash.h>
#include <UserObjectManager.h>
#include <FontCollectionImpl.h>
#include <object/ObjectDictionary.h>
#include <stream/SimpleInputFileStream.h>

#include <stream/InputStream.h>
#include <stream/MemoryIStream.h>
#include <stream/MemoryOStream.h>

#include <font/SimpleFont.h>
#include <font/TrueType/FontTrueType.h>
#include <font/TrueType/FontTrueTypeCollection.h>

namespace{
  using namespace pdfout;

  bool isFileExists(pdfout::UString const &fileName){
    #ifdef _WIN32
      struct _stat64 info;
      UString const tmp = fileName.encode(Encoding_WCHAR_T);
      return (_wstat64((wchar_t *) tmp.getData(), &info) == 0);
    #else
      struct stat info;   
      UString const tmp = fileName.encode(Encoding_UTF8);
      return (stat(tmp.getData(), &info) == 0); 
    #endif
  }
}

namespace kernel{
  typedef UserObjectManager<pdfout::impl::FontImpl> FontManager;
  typedef UserObjectManager<kernel::font::SimpleFont> SimpleFontManager;
  typedef UserObjectManager<pdfout::impl::FontCollectionImpl> FontCollectionManager;
}

using namespace kernel;
using namespace kernel::font;

using namespace pdfout;
using namespace pdfout::impl;

std::deque<UString> FontImpl::mFontPath;
std::unique_ptr<kernel::UserObjectHash<FontImpl>> FontImpl::mFonts(std::make_unique<kernel::UserObjectHash<FontImpl>>());
std::unique_ptr<kernel::UserObjectHash<kernel::font::SimpleFont>> FontImpl::mSimpleFonts(std::make_unique<kernel::UserObjectHash<kernel::font::SimpleFont>>());

namespace pdfout{ namespace impl{

  FontImpl::FontImpl(std::unique_ptr<CompositeFont> compositeFont, uint16_t version)
    : Font(),
    mUID(ResourceUid::getResourceUid(ResourceTypeFont)),
    mVersion(version),
    mCompositeFont(std::move(compositeFont)){
  }

  FontImpl::~FontImpl(void) {}

  void FontImpl::addFontPath(pdfout::UString const &fontPath){
    mFontPath.push_back(fontPath);
  }

  void FontImpl::setFontPath(pdfout::UString const &fontPath){
    mFontPath.clear();
    mFontPath.push_back(fontPath);
  }

  FontImpl *FontImpl::createFont(pdfout::UString const &fileName, WritingMode writingMode){
    std::unique_ptr<InputStream> fstream = makeInputStream(fileName);

    if (!fstream)
      return nullptr;

    return createFont(fstream.get(), writingMode);
  }

  FontImpl *FontImpl::createFont(void const *fontData, uint64_t size, WritingMode writingMode){
    std::unique_ptr<MemoryIStream> fstream = std::make_unique<MemoryIStream>(fontData, size);
    return createFont(fstream.get(), writingMode);
  }

  FontImpl *FontImpl::createFont(InputStream *istream, WritingMode writingMode){
    using kernel::font::truetype::FontTrueType;

    std::string const sha256 = getSHA256(istream);
    std::string const sha256ex = getSHA256Ex(sha256, writingMode);

    FontImpl *implPtr = mFonts->find(sha256ex);
    if (implPtr)
      return implPtr;

    SimpleFont *simpleFont = mSimpleFonts->find(sha256);
    if (!simpleFont){
      simpleFont = SimpleFontManager::getInstance().insert(SimpleFont::createSimpleFont(istream));
      if (!simpleFont)
        return nullptr;

      mSimpleFonts->store(simpleFont, sha256);
    }

    std::unique_ptr<FontImpl> impl(new FontImpl(CompositeFont::createCompositeFont(simpleFont, writingMode), 1));
    implPtr = FontManager::getInstance().insert(std::move(impl));
    mFonts->store(implPtr, sha256ex);

    return implPtr;
  }

  FontCollectionImpl *FontImpl::createFontCollection(pdfout::UString const &fileName, WritingMode writingMode){
    std::unique_ptr<InputStream> fstream = makeInputStream(fileName);

    if (!fstream)
      return nullptr;

    return createFontCollection(fstream.get(), writingMode);
  }

  FontCollectionImpl *FontImpl::createFontCollection(void const *fontData, uint64_t size, WritingMode writingMode){
    std::unique_ptr<MemoryIStream> fstream = std::make_unique<MemoryIStream>(fontData, size);
    return createFontCollection(fstream.get(), writingMode);
  }

  FontCollectionImpl *FontImpl::createFontCollection(InputStream *istream, WritingMode writingMode){
    using kernel::font::truetype::FontTrueTypeCollection;

    std::unique_ptr<FontTrueTypeCollection> collection = std::make_unique<FontTrueTypeCollection>(istream);
    std::size_t const count = collection->getNumFonts();
    std::string const sha256 = getSHA256(istream); // SHA256 for FontTrueTypeCollection object

    std::vector<Font *> fonts;

    for (std::size_t i = 0; i < count; ++i){
      std::string const sha256Ex = getSHA256Ex(sha256, i); // SHA256 for SimpleFont object in the collection
      std::string const sha256ExEx = getSHA256Ex(sha256Ex, writingMode); // SHA256 for FontImpl object

      FontImpl *implPtr = mFonts->find(sha256ExEx);
      if (implPtr){
        fonts.push_back(implPtr);
        continue;
      }

      SimpleFont *simpleFont = mSimpleFonts->find(sha256Ex);
      if (!simpleFont){
        simpleFont = SimpleFontManager::getInstance().insert(collection->createFont(i));
        mSimpleFonts->store(simpleFont, sha256Ex);
      }

      std::unique_ptr<FontImpl> impl(new FontImpl(CompositeFont::createCompositeFont(simpleFont, writingMode), 1));
      implPtr = FontManager::getInstance().insert(std::move(impl));
      mFonts->store(implPtr, sha256ExEx);

      fonts.push_back(implPtr);
    }

    FontCollectionImpl *result = FontCollectionManager::getInstance().insert(std::make_unique<FontCollectionImpl>(fonts));
    return result;
  }

  uint16_t FontImpl::getVersion(void) const{
    return mVersion;
  }

  pdfout::WritingMode FontImpl::getWritingMode(void) const{
    return mCompositeFont->getWritingMode();
  }

  UString FontImpl::getRevision(void) const {return mCompositeFont->getDescendantFont()->getRevision();}
  UString FontImpl::getPostScriptName(void) const {return mCompositeFont->getDescendantFont()->getPostScriptName();}
  UString FontImpl::getFamilyName(void) const     {return mCompositeFont->getDescendantFont()->getFamilyName();}
  UString FontImpl::getSubFamilyName(void) const  {return mCompositeFont->getDescendantFont()->getSubFamilyName();}

  void FontImpl::saveToStream(pdfout::OutputStream *stream) const{
    return mCompositeFont->getDescendantFont()->saveToStream(stream);
  }

  void FontImpl::toCharCodes(void const *glyphIndices, uint16_t elemSize, uint64_t count, pdfout::UString &charCodes, uint32_t defaultValue) const{
    return mCompositeFont->getDescendantFont()->toCharCodes(glyphIndices, elemSize, count, charCodes, defaultValue);
  }

  void FontImpl::toText(void const *glyphIndices, uint16_t elemSize, uint64_t count, pdfout::UString &text, uint32_t defaultValue) const{
    return mCompositeFont->getDescendantFont()->toText(glyphIndices, elemSize, count, text, defaultValue);
  }

  int32_t FontImpl::getTextWidth(pdfout::UString const &text) const {return mCompositeFont->getDescendantFont()->getTextWidth(text);}
  int32_t FontImpl::getCharWidth(uint16_t cid) const {return mCompositeFont->getDescendantFont()->getCharWidth(cid);}
  void FontImpl::getCIDMetrics(uint16_t cid, CIDMetrics &metrics) const {return mCompositeFont->getDescendantFont()->getCIDMetrics(cid, metrics);}
  int32_t FontImpl::getKerningWidth(uint16_t cidLhs, uint16_t cidRhs) const {return mCompositeFont->getDescendantFont()->getKerningWidth(cidLhs, cidRhs);}

  int32_t FontImpl::getLineGap(void) const    {return mCompositeFont->getDescendantFont()->getLineGap();}
  int32_t FontImpl::getAscent(void) const     {return mCompositeFont->getDescendantFont()->getAscent();}
  int32_t FontImpl::getDescent(void) const    {return mCompositeFont->getDescendantFont()->getDescent();}
  int32_t FontImpl::getCapHeight(void) const  {return mCompositeFont->getDescendantFont()->getCapHeight();}
  int32_t FontImpl::getUnitsPerEm(void) const {return mCompositeFont->getDescendantFont()->getUnitsPerEm();}

  int32_t FontImpl::getWinAscent(void) const          {return mCompositeFont->getDescendantFont()->getWinAscent();}
  int32_t FontImpl::getWinDescent(void) const         {return mCompositeFont->getDescendantFont()->getWinDescent();}
  int32_t FontImpl::getWinInternalLeading(void) const {return mCompositeFont->getDescendantFont()->getWinInternalLeading();}
  int32_t FontImpl::getWinExternalLeading(void) const {return mCompositeFont->getDescendantFont()->getWinExternalLeading();}
  int32_t FontImpl::getBreakChar(void) const          {return mCompositeFont->getDescendantFont()->getBreakChar();}
  int32_t FontImpl::getDefaultChar(void) const        {return mCompositeFont->getDescendantFont()->getDefaultChar();}
  int32_t FontImpl::getUnderlineYSize(void) const     {return mCompositeFont->getDescendantFont()->getUnderlineYSize();}

  bool FontImpl::isBold(void) const      {return mCompositeFont->getDescendantFont()->isBold();}
  bool FontImpl::isItalic(void) const    {return mCompositeFont->getDescendantFont()->isItalic();}
  bool FontImpl::isUnderline(void) const {return mCompositeFont->getDescendantFont()->isUnderline();}
  bool FontImpl::isShadow(void) const    {return mCompositeFont->getDescendantFont()->isShadow();}
  bool FontImpl::isCondensed(void) const {return mCompositeFont->getDescendantFont()->isCondensed();}
  bool FontImpl::isExtended(void) const  {return mCompositeFont->getDescendantFont()->isExtended();}

  Font *FontImpl::merge(Font const *delta) const{
    std::unique_ptr<FontImpl> superset = createSuperset((FontImpl *) delta);
    FontImpl *loadedFont = FontManager::getInstance().insert(std::move(superset));
    return loadedFont;
  }

  std::string const &FontImpl::getUID(void) const{
    return mUID;
  }

  int32_t FontImpl::getTextWidth(pdfout::UString const &text, std::vector<int32_t> &widths) const{
    return mCompositeFont->getDescendantFont()->getTextWidth(text, widths);
  }

  std::string FontImpl::getSHA256Ex(std::string const &sha256, WritingMode writingMode){
    std::string const sha256ex = sha256 + ((writingMode == WritingModeHorizontal) ? "H" : "V");
    return sha256ex;
  }

  std::string FontImpl::getSHA256Ex(std::string const &sha256, uint32_t index){
    std::string const sha256ex = sha256 + std::to_string(index);
    return sha256ex;
  }

  std::unique_ptr<kernel::ObjectIndirectReference> FontImpl::serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const{
    return mCompositeFont->serialize(stream, params);
  }

  std::unique_ptr<FontImpl> FontImpl::createSubset(std::vector<uint32_t> const &cids) const{
    std::unique_ptr<SimpleFont> subset = mCompositeFont->getDescendantFont()->createSubset(cids);
    SimpleFont *subsetPtr = SimpleFontManager::getInstance().insert(std::move(subset));

    std::unique_ptr<FontImpl> impl(new FontImpl(CompositeFont::createCompositeFont(subsetPtr, getWritingMode()), 1));
    return impl;
  }

  std::unique_ptr<FontImpl> FontImpl::createSuperset(FontImpl const *font) const{
    if (!font)
      RUNTIME_EXCEPTION("Invalid FontImpl pointer");

    std::unique_ptr<SimpleFont> superset = mCompositeFont->getDescendantFont()->createSuperset(font->mCompositeFont->getDescendantFont());
    SimpleFont *supersetPtr = SimpleFontManager::getInstance().insert(std::move(superset));

    std::unique_ptr<FontImpl> impl(new FontImpl(CompositeFont::createCompositeFont(supersetPtr, getWritingMode()), getVersion() + 1));
    impl->mUID = mUID;

    return impl;
  }

  std::unique_ptr<InputStream> FontImpl::makeInputStream(pdfout::UString const &fileName){
    std::unique_ptr<InputStream> fstream(nullptr);

    if (isFileExists(fileName))
      fstream = std::make_unique<SimpleInputFileStream>(fileName);
    else{
      for (auto const &elem : mFontPath){
        UString const temp = elem + fileName;
        if (isFileExists(temp)){
          fstream = std::make_unique<SimpleInputFileStream>(temp);
          break;
        }
      }
    }

    return fstream;
  }

}}
