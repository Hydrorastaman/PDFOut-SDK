#include <FontStorageImpl.h>

#include <string>
#include <algorithm>
#include <unordered_map>

#include <Types.h>
#include <EmfTypes.h>
#include <Exception.h>
#include <GenericHandle.h>
#include <FontCollection.h>
#include <stream/MemoryOStream.h>

using namespace pdfout;

namespace{

  uint32_t const EMRI_ENGINE_FONT = 0x00000002;
  uint32_t const EMRI_SUBSET_FONT = 0x00000007;
  uint32_t const EMRI_TYPE1_FONT = 0x00000004;
  uint32_t const EMRI_DESIGNVECTOR = 0x00000006;
  uint32_t const EMRI_DELTA_FONT = 0x00000008;

  DWORD const typeTTF = 0x0;
  DWORD const typeTTCF = 0x66637474;

  int CALLBACK getFontFaceNameCallback(const LOGFONTW *lpelfe, const TEXTMETRICW *lpntme, DWORD FontType, LPARAM lParam){
    WCHAR *buff = (WCHAR *) lParam;
    memcpy(buff, lpelfe->lfFaceName, LF_FACESIZE * sizeof(WCHAR));
    return 0;
  }

  std::wstring getFontFaceName(HDC hdc, LOGFONTW logfont){
    logfont.lfCharSet = ANSI_CHARSET;
    logfont.lfPitchAndFamily = 0;

    WCHAR faceName[LF_FACESIZE] = {0};
    EnumFontFamiliesExW(hdc, &logfont, getFontFaceNameCallback, (LPARAM) faceName, 0);

    // for embedded fonts
    if (!wcslen(faceName))
      memcpy(faceName, logfont.lfFaceName, LF_FACESIZE * sizeof(WCHAR));

    return faceName;
  }
}

namespace emf2pdf{ namespace impl{
  bool operator==(LOGFONTW const &lhs, LOGFONTW const &rhs){
    return lhs.lfWeight == rhs.lfWeight
            && lhs.lfItalic == rhs.lfItalic
            && lhs.lfUnderline == rhs.lfUnderline
            && lhs.lfStrikeOut == rhs.lfStrikeOut
            && lhs.lfCharSet == rhs.lfCharSet
            && lhs.lfOutPrecision == rhs.lfOutPrecision
            && lhs.lfClipPrecision == rhs.lfClipPrecision
            && lhs.lfQuality == rhs.lfQuality
            && lhs.lfPitchAndFamily == rhs.lfPitchAndFamily
            && std::wstring(lhs.lfFaceName, LF_FACESIZE) == std::wstring(rhs.lfFaceName, LF_FACESIZE);
  }

  FontStorageImpl::FontStorageImpl(void)
    : mAccessMutex(),
    mRegularFonts(){
  }

  FontStorageImpl::~FontStorageImpl(void){
  }

  void FontStorageImpl::loadEmbeddedFonts(wchar_t const *emfFile){
    HEnhMetafile emf(GetEnhMetaFileW(emfFile));
    if (!emf)
      RUNTIME_EXCEPTION("Failed to GetEnhMetaFile(), errcode: " + std::to_string(GetLastError()));

    return loadEmbeddedFonts(emf);
  }

  void FontStorageImpl::loadEmbeddedFonts(HENHMETAFILE metafile){
    ENHMETAHEADER hdr = {0};
    GetEnhMetaFileHeader(metafile, sizeof(hdr), &hdr);

    uint16_t const emfDpiX = (uint16_t) std::lround(25.4f * hdr.szlDevice.cx / hdr.szlMillimeters.cx);
    uint16_t const emfDpiY = (uint16_t) std::lround(25.4f * hdr.szlDevice.cy / hdr.szlMillimeters.cy);
    float const frameWidth = (hdr.rclFrame.right - hdr.rclFrame.left) / 100.f;
    float const frameHeight = (hdr.rclFrame.bottom - hdr.rclFrame.top) / 100.f;
    RECT rect = {0, 0, std::round(emfDpiX * frameWidth / 25.4f), std::round(emfDpiY * frameHeight / 25.4f)};

    Hdc screenDC(GetDC(nullptr));
    Hdc hdc(CreateCompatibleDC(screenDC));

    EnumEnhMetaFile(hdc, metafile, enhMetaFileProcEmbeddedFonts, this, &rect);
  }

  Font *FontStorageImpl::getCurrentFont(HDC hdc, uint32_t ufi, TextInfo &textInfo){
    HFONT currFont = (HFONT) GetCurrentObject(hdc, OBJ_FONT);
    LOGFONTW logfont = {0};
    GetObjectW(currFont, sizeof(LOGFONTW), &logfont);
    std::wstring faceName = getFontFaceName(hdc, logfont);
    memcpy(logfont.lfFaceName, faceName.data(), (faceName.length() + 1) * sizeof(WCHAR));

    textInfo.mIsBold = logfont.lfWeight >= FW_BOLD;
    textInfo.mIsItalic = logfont.lfItalic ? true : false;
    textInfo.mEscapement = logfont.lfEscapement;
    textInfo.mOrientation = logfont.lfOrientation;
    textInfo.mIsUnderline = logfont.lfUnderline;
    textInfo.mWritingMode = (faceName[0] == '@' && logfont.lfEscapement == 2700 && logfont.lfOrientation == 2700) ? pdfout::WritingModeVertical : pdfout::WritingModeHorizontal;

    TEXTMETRIC tm = {0};
    GetTextMetrics(hdc, &tm);
    textInfo.mSize = tm.tmAscent + tm.tmDescent - tm.tmInternalLeading;

    Font *storedFont = findFont(ufi, logfont, textInfo.mWritingMode);
    if (storedFont){
      return storedFont;
    }

    DWORD type = typeTTCF;
    DWORD dataSize = GetFontData(hdc, type, 0, nullptr, 0);
    if (dataSize == GDI_ERROR){
      type = typeTTF;
      dataSize = GetFontData(hdc, type, 0, nullptr, 0);
      if (dataSize == GDI_ERROR)
        return nullptr;
    }

    std::vector<uint8_t> fontData(dataSize);
    GetFontData(hdc, type, 0, &fontData[0], dataSize);

    Font *font = nullptr;
    if (type == typeTTF){
      font = Font::createFont(fontData.data(), dataSize, textInfo.mWritingMode);
      if (!font)
        return getSubstitutionFont(logfont, L"Times New Roman", textInfo);

      storeFont(font, logfont, textInfo.mWritingMode);
    }
    else if (type == typeTTCF){
      FontCollection *collection = Font::createFontCollection(fontData.data(), dataSize, textInfo.mWritingMode);
      for (std::size_t i = 0, size = collection->getCount(); i < size; ++i){
        UString tmp = collection->getFont(i)->getPostScriptName().encode(Encoding_WCHAR_T);
        std::wstring psName((wchar_t const *) tmp.getData(), tmp.getSize() / sizeof(wchar_t));

        if (psName == faceName){
          font = collection->getFont(i);
          if (!font)
            return nullptr;

          storeFont(font, logfont, textInfo.mWritingMode);
          return font;
        }
      }

      font = collection->getFont(0);
      if (!font)
        return nullptr;

      storeFont(font, logfont, textInfo.mWritingMode);
    }

    return font;
  }

  void FontStorageImpl::storeEmbeddedFont(pdfout::Font *font, wchar_t const *facename, uint32_t ufi, bool isBold, bool isItalic){
    std::lock_guard<std::mutex> lock(mAccessMutex);

    EmbedFontInfo embedInfo;
    embedInfo.mBold = isBold;
    embedInfo.mItalic = isItalic;
    embedInfo.mFamily = facename;
    embedInfo.mHandle = nullptr;

    auto iter = std::find_if(std::begin(mEmbeddedFonts), std::end(mEmbeddedFonts), [&embedInfo](std::pair<EmbedFontInfo, Font *> &value)->bool {return value.first == embedInfo;});
    if (iter == std::end(mEmbeddedFonts)){
      installFont(font, embedInfo);
      mEmbeddedFonts.push_back(std::make_pair(embedInfo, font));
    }
    else{
      if (font->getVersion() > iter->second->getVersion()){
        installFont(font, iter->first);
        iter->second = font;
      }
    }

    if (ufi != FontStorage::unknownUfi)
      mIndexedEmbeddedFonts[ufi] = font;
  }

  void FontStorageImpl::storeEmbeddedFont(pdfout::Font *font, std::wstring const &facename, uint32_t ufi, bool isBold, bool isItalic){
    return storeEmbeddedFont(font, facename.c_str(), ufi, isBold, isItalic);
  }

  std::size_t FontStorageImpl::getEmbeddedFontsCount(void) const{
    std::lock_guard<std::mutex> lock(mAccessMutex);
    return mEmbeddedFonts.size();
  }

  pdfout::Font const *FontStorageImpl::getEmbeddedFont(std::size_t index) const{
    std::lock_guard<std::mutex> lock(mAccessMutex);
    return mEmbeddedFonts[index].second;
  }

  std::size_t FontStorageImpl::getSystemFontsCount(void) const{
    std::lock_guard<std::mutex> lock(mAccessMutex);
    return mRegularFonts.size();
  }

  pdfout::Font const *FontStorageImpl::getSystemFont(std::size_t index) const{
    std::lock_guard<std::mutex> lock(mAccessMutex);
    return mRegularFonts[index].second;
  }

  std::vector<pdfout::Font *> FontStorageImpl::findFontByRevision(pdfout::UString const &rev) const{
    std::vector<pdfout::Font *> result;

    if (!rev.getSize())
      return result;

    std::lock_guard<std::mutex> lock(mAccessMutex);
    for (auto const &elem : mEmbeddedFonts){
      if (elem.second->getRevision() == rev)
        result.push_back(elem.second);
    }

    return result;
  }

  Font *FontStorageImpl::findFont(uint32_t ufi, LOGFONTW const &logfont, pdfout::WritingMode writingMode){
    std::lock_guard<std::mutex> lock(mAccessMutex);

    if (ufi != FontStorage::unknownUfi){
      auto iter3 = mIndexedEmbeddedFonts.find(ufi);
      if (iter3 != std::end(mIndexedEmbeddedFonts))
        return iter3->second;
    }

    bool const isItalic = logfont.lfItalic != 0;
    bool const isBold = logfont.lfWeight >= FW_BOLD;

    EmbedFontInfo const embedInfo = {isBold, isItalic, logfont.lfFaceName, nullptr};
    auto iter = std::find_if(std::begin(mEmbeddedFonts), std::end(mEmbeddedFonts), [&embedInfo](std::pair<EmbedFontInfo, Font *> &value)->bool {return value.first == embedInfo;});
    if (iter != std::end(mEmbeddedFonts))
      return iter->second;

    RegularFontInfo const regularInfo = {logfont, writingMode};
    auto iter2 = std::find_if(std::begin(mRegularFonts), std::end(mRegularFonts), [&regularInfo](std::pair<RegularFontInfo, Font *> &value)->bool {return value.first == regularInfo;});
    if (iter2 != std::end(mRegularFonts))
      return iter2->second;

    return nullptr;
  }

  void FontStorageImpl::storeFont(Font *font, LOGFONTW const &logfont, pdfout::WritingMode writingMode){
    std::lock_guard<std::mutex> lock(mAccessMutex);
    RegularFontInfo const regularInfo = {logfont, writingMode};
    mRegularFonts.push_back(std::make_pair(regularInfo, font));
  }

  void FontStorageImpl::installFont(pdfout::Font *font, EmbedFontInfo &fontInfo) const{
    if (fontInfo.mHandle)
      RemoveFontMemResourceEx(fontInfo.mHandle);

    pdfout::MemoryOStream ostream;
    font->saveToStream(&ostream);
    DWORD fontNum;
    fontInfo.mHandle = AddFontMemResourceEx(ostream.getData(), ostream.getSize(), 0, &fontNum);
    if (!fontInfo.mHandle)
      RUNTIME_EXCEPTION("Failed to AddFontMemResourceEx(), errcode: " + std::to_string(GetLastError()));
  }

  pdfout::Font *FontStorageImpl::getSubstitutionFont(LOGFONTW logfont, std::wstring const &faceName, TextInfo &textInfo) {
    Hdc hdc(::GetDC(NULL));
    memcpy(logfont.lfFaceName, faceName.data(), (faceName.length() + 1) * sizeof(WCHAR));
    HFONT font = CreateFontIndirectW(&logfont);
    SelectObject(hdc, font);
    pdfout::Font *substFont = getCurrentFont(hdc, FontStorage::unknownUfi, textInfo);

    return substFont;
  }

  int CALLBACK FontStorageImpl::enhMetaFileProcEmbeddedFonts(HDC hdc, HANDLETABLE *table, ENHMETARECORD const *emfr, int obj, LPARAM data){
    FontStorageImpl *fontStorage = (FontStorageImpl *) data;
    switch (emfr->iType){
      case EMR_GDICOMMENT:
        fontStorage->onEmrGdiComment(emfr);
        break;
    }

    return 1;
  }

  void FontStorageImpl::onEmrGdiComment(ENHMETARECORD const *emfr){
    EMR_CommentEmfSpool *record = (EMR_CommentEmfSpool *) emfr;
    if (record->cbCommentId == 0x0 && record->cbEmfSpoolRecordId == 0x544F4E46){

      DWORD offset = 0;

      while (offset < record->cbData){
        EMRI_GenericRecord *subsetRecord = (EMRI_GenericRecord *) &record->Data[offset];

        switch (subsetRecord->ulID){
          case EMRI_ENGINE_FONT:{
            EMRI_EngineFontRecord *fontRecord = (EMRI_EngineFontRecord *) &record->Data[offset];
            if (fontRecord->Type1ID != 0x0)
              continue;

            BYTE *ptr = fontRecord->data;
            DWORD count = fontRecord->NumFiles;
            std::deque<DWORD> fileSizes;
            for (DWORD i = 0; i < count; ++i){
              fileSizes.push_back(((DWORD *) fontRecord->data)[i]);
              ptr += sizeof(DWORD);
            }

            ptr += ((DWORD) ptr - (DWORD) subsetRecord) % 8;

            for (DWORD i = 0; i < count; ++i){
              ptr += ((DWORD) ptr - (DWORD) subsetRecord) % 4;
              addEmbeddedFont(ptr, fileSizes[i], FontStorage::unknownUfi, EmbedFontTypeEngine);
              ptr += fileSizes[i];
            }
            break;
          }

          case EMRI_SUBSET_FONT:{
            EMRI_SubsetFontRecord *fontRecord = (EMRI_SubsetFontRecord *) &record->Data[offset];
            uint32_t const ufi = ((EMR_Reserved_109 *) fontRecord->UniversalFontId)->index;
            addEmbeddedFont(fontRecord->data, fontRecord->generic.cjSize, ufi, EmbedFontTypeSubset);
            break;
          }

          case EMRI_TYPE1_FONT:{
            break;
          }

          case EMRI_DESIGNVECTOR:{
            break;
          }

          case EMRI_DELTA_FONT:{
            EMRI_DeltaFontRecord *fontRecord = (EMRI_DeltaFontRecord *) &record->Data[offset];
            uint32_t const ufi = ((EMR_Reserved_109 *) fontRecord->UniversalFontId)->index;
            addEmbeddedFont(fontRecord->data, fontRecord->generic.cjSize, ufi, EmbedFontTypeDelta);
            break;
          }
        }

        offset += sizeof(EMRI_GenericRecord) + subsetRecord->cjSize;
      }
    }
  }

  void FontStorageImpl::addEmbeddedFont(uint8_t const *data, std::size_t size, uint32_t ufi, EmbedFontType type){
    if (!data || !size)
      return;

    Font *ttf = Font::createFont(data, size);
    if (!ttf)
      return;

    std::deque<Font *> embedFonts;

    if (type == EmbedFontTypeDelta){
      std::vector<Font *> fonts = findFontByRevision(ttf->getRevision());
      if (fonts.empty())
        RUNTIME_EXCEPTION("Failed to find a base TrueType font for a partial TrueType font");

      for (auto &elem : fonts){
        try{
          Font *merged = elem->merge(ttf);
          embedFonts.push_back(merged);
        }
        catch (pdfout::Exception &){
        }
      }
    }
    else
      embedFonts.push_back(ttf);

    for (auto const &elem : embedFonts){
      UString fontFamilyName = elem->getFamilyName().encode(Encoding_WCHAR_T);
      std::wstring familyName((WCHAR *) fontFamilyName.getData(), (WCHAR *) fontFamilyName.getData() + fontFamilyName.getSize() / sizeof(WCHAR));
      storeEmbeddedFont(elem, familyName, ufi, elem->isBold(), elem->isItalic());
    }
  }

}}
