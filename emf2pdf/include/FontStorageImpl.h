#pragma once

#include <map>
#include <deque>
#include <mutex>

#include <Font.h>
#include <Windows.h>
#include <FontStorage.h>

namespace emf2pdf{ namespace impl{

  bool operator==(LOGFONTW const &lhs, LOGFONTW const &rhs);

  class FontStorageImpl{
    struct EmbedFontInfo{
      bool mItalic;
      bool mBold;
      std::wstring mFamily;
      HANDLE mHandle;

      bool operator==(EmbedFontInfo const &rhs){
        return mFamily == rhs.mFamily && mItalic == rhs.mItalic && mBold == rhs.mBold;
      }
    };

    struct RegularFontInfo{
      LOGFONTW mLogfont;
      pdfout::WritingMode mWritingMode;

      bool operator==(RegularFontInfo const &rhs){
        return mLogfont == rhs.mLogfont && mWritingMode == rhs.mWritingMode;
      }
    };

  public:
    FontStorageImpl(void);
    ~FontStorageImpl(void);

    void loadEmbeddedFonts(wchar_t const *emfFile);
    void loadEmbeddedFonts(HENHMETAFILE metafile);

    pdfout::Font *getCurrentFont(HDC hdc, uint32_t ufi, TextInfo &textInfo);
    void storeEmbeddedFont(pdfout::Font *font, wchar_t const *facename, uint32_t ufi, bool isBold, bool isItalic);
    void storeEmbeddedFont(pdfout::Font *font, std::wstring const &facename, uint32_t ufi, bool isBold, bool isItalic);

    std::size_t getEmbeddedFontsCount(void) const;
    pdfout::Font const *getEmbeddedFont(std::size_t index) const;

    std::size_t getSystemFontsCount(void) const;
    pdfout::Font const *getSystemFont(std::size_t index) const;

    std::vector<pdfout::Font *> findFontByRevision(pdfout::UString const &rev) const;

  private:
    FontStorageImpl(FontStorageImpl const &) = delete;
    FontStorageImpl &operator=(FontStorageImpl const &) = delete;

  private:
    static  int CALLBACK enhMetaFileProcEmbeddedFonts(HDC hdc, HANDLETABLE *table, ENHMETARECORD const *emfr, int obj, LPARAM data);
    void onEmrGdiComment(ENHMETARECORD const *emfr);
    void addEmbeddedFont(uint8_t const *data, std::size_t size, uint32_t ufi, EmbedFontType type);

    pdfout::Font *findFont(uint32_t ufi, LOGFONTW const &logfont, pdfout::WritingMode writingMode);
    void storeFont(pdfout::Font *font, LOGFONTW const &logfont, pdfout::WritingMode writingMode);

    void installFont(pdfout::Font *font, EmbedFontInfo &fontInfo) const;
    pdfout::Font *getSubstitutionFont(LOGFONTW logfont, std::wstring const &faceName, TextInfo &textInfo);
  private:
    mutable std::mutex mAccessMutex;
    std::vector<std::pair<RegularFontInfo, pdfout::Font *>> mRegularFonts;
    std::vector<std::pair<EmbedFontInfo, pdfout::Font *>> mEmbeddedFonts;
    std::map<uint32_t, pdfout::Font *> mIndexedEmbeddedFonts;
  };

}}
