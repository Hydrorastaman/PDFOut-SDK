#include <Emf2PdfEngineImpl.h>

#include <stream/MemoryOStream.h>
#include <stream/SimpleOutputFileStream.h>

#include <EmfTypes.h>
#include <Exception.h>

#include <unordered_map>

namespace{
  /**
   * values from Windows 10 HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\FontMapperFamilyFallback
   */
  static std::unordered_map<std::wstring, std::wstring> fontMapperFamilyFallback = {
    {L"Batang", L"Malgun Gothic"},
    {L"DaunPenh", L"Leelawadee UI"},
    {L"DokChampa", L"Leelawadee UI"},
    {L"Dotum", L"Malgun Gothic"},
    {L"Estrangelo Edessa", L"Segoe UI Historic"},
    {L"Euphemia", L"Gadugi"},
    {L"Gautami", L"Nirmala UI"},
    {L"Gulim", L"Malgun Gothic"},
    {L"Iskoola Pota", L"Nirmala UI"},
    {L"Kalinga", L"Nirmala UI"},
    {L"Kartika", L"Nirmala UI"},
    {L"Khmer UI", L"Leelawadee UI"},
    {L"Lao UI", L"Leelawadee UI"},
    {L"Latha", L"Nirmala UI"},
    {L"Leelawadee", L"Leelawadee UI"},
    {L"Mangal", L"Nirmala UI"},
    {L"Meiryo", L"Yu Gothic UI"},
    {L"Meiryo UI", L"Yu Gothic UI"},
    {L"MingLiU", L"Microsoft JhengHei UI"},
    {L"MingLiU_HKSCS", L"Microsoft JhengHei UI"},
    {L"MoolBoran", L"Leelawadee UI"},
    {L"MS Gothic", L"Yu Gothic UI"},
    {L"MS Mincho", L"Yu Gothic UI"},
    {L"MS PGothic", L"Yu Gothic UI"},
    {L"MS PMincho", L"Yu Gothic UI"},
    {L"MS UI Gothic", L"Yu Gothic UI"},
    {L"Nyala", L"Ebrima"},
    {L"Plantagenet Cherokee", L"Gadugi"},
    {L"PMingLiU", L"Microsoft JhengHei UI"},
    {L"Raavi", L"Nirmala UI"},
    {L"Shruti", L"Nirmala UI"},
    {L"Tunga", L"Nirmala UI"},
    {L"Vrinda", L"Nirmala UI"},
    {L"メイリオ", L"Yu Gothic UI"},
    {L"新細明體", L"Microsoft JhengHei UI"},
    {L"細明體", L"Microsoft JhengHei UI"},
    {L"細明體_HKSCS", L"Microsoft JhengHei UI"},
    {L"굴림", L"Malgun Gothic"},
    {L"돋움", L"Malgun Gothic"},
    {L"바탕", L"Malgun Gothic"},
    {L"ＭＳ ゴシック", L"Yu Gothic UI"},
    {L"ＭＳ 明朝", L"Yu Gothic UI"},
    {L"ＭＳ Ｐゴシック", L"Yu Gothic UI"},
    {L"ＭＳ Ｐ明朝", L"Yu Gothic UI"}
  };

  int CALLBACK getFontFaceNameCallback(const LOGFONTW *lpelfe, const TEXTMETRICW *lpntme, DWORD FontType, LPARAM lParam){
    WCHAR *buff = (WCHAR *) lParam;
    memcpy(buff, lpelfe->lfFaceName, LF_FACESIZE * sizeof(WCHAR));
    return 0;
  }

  std::wstring getFontFaceName(HDC hdc, LOGFONTW logfont){
    logfont.lfCharSet = DEFAULT_CHARSET;/*ANSI_CHARSET;*/
    logfont.lfPitchAndFamily = 0;

    WCHAR faceName[LF_FACESIZE] = {0};
    EnumFontFamiliesExW(hdc, &logfont, getFontFaceNameCallback, (LPARAM) faceName, 0);
    return faceName;
  }
}

using namespace pdfout;

namespace emf2pdf{ namespace impl{
  void Emf2PdfEngineImpl::onEmrReserved109(ENHMETARECORD const *emfr){
    EMR_Reserved_109 *source = (EMR_Reserved_109 *) emfr;
    mCurrentDeviceState->mUfi = source->index;
  }

  void Emf2PdfEngineImpl::onEmrExtCreateFontIndirectW(HANDLETABLE *table, ENHMETARECORD const *emfr, int obj){
    EMREXTCREATEFONTINDIRECTW *source = (EMREXTCREATEFONTINDIRECTW *) emfr;

    std::wstring const sourceFacename = source->elfw.elfLogFont.lfFaceName;
    std::wstring const facename = getFontFaceName(mHdc, source->elfw.elfLogFont);

    if (sourceFacename == facename)
      PlayEnhMetaFileRecord(mHdc, table, emfr, obj);
    else{
      auto iter = fontMapperFamilyFallback.find(sourceFacename);
      if (iter != std::end(fontMapperFamilyFallback)){
        std::vector<uint8_t> record(emfr->nSize);
        memcpy(record.data(), emfr, emfr->nSize);
        LOGFONTW *lf = &((EMREXTCREATEFONTINDIRECTW *) record.data())->elfw.elfLogFont;
        wcscpy_s(lf->lfFaceName, LF_FACESIZE, iter->second.c_str());
        PlayEnhMetaFileRecord(mHdc, table, (ENHMETARECORD const *) record.data(), obj);
      }
      else
        PlayEnhMetaFileRecord(mHdc, table, emfr, obj);
    }
  }
}}
