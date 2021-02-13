#include <PaperSize.h>

#include <algorithm>
#include <Exception.h>

namespace {

  using namespace pdfout;

  PaperSizeInfo paperSizeInfo[] = {
    {PaperSize_ISO_A0,  33.1f, 46.8f},
    {PaperSize_ISO_A1,  23.4f, 33.1f},
    {PaperSize_ISO_A2,  16.5f, 23.4f},
    {PaperSize_ISO_A3,  11.7f, 16.5f},
    {PaperSize_ISO_A4,  8.27f, 11.7f},
    {PaperSize_ISO_A5,  5.83f, 8.27f},
    {PaperSize_ISO_A6,  4.13f, 5.83f},
    {PaperSize_ISO_A7,  2.91f, 4.13f},
    {PaperSize_ISO_A8,  2.05f, 2.91f},
    {PaperSize_ISO_A9,  1.46f, 2.05f},
    {PaperSize_ISO_A10, 1.02f, 1.46f},
    {PaperSize_ISO_B0,  39.4f, 55.7f},
    {PaperSize_ISO_B1,  27.8f, 39.4f},
    {PaperSize_ISO_B2,  19.7f, 27.8f},
    {PaperSize_ISO_B3,  13.9f, 19.7f},
    {PaperSize_ISO_B4,  9.84f, 13.9f},
    {PaperSize_ISO_B5,  6.93f, 9.84f},
    {PaperSize_ISO_B6,  4.92f, 6.93f},
    {PaperSize_ISO_B7,  3.46f, 4.92f},
    {PaperSize_ISO_B8,  2.44f, 3.46f},
    {PaperSize_ISO_B9,  1.73f, 2.44f},
    {PaperSize_ISO_B10, 1.22f, 1.73f},
    {PaperSize_ISO_C0,  36.1f, 51.1f},
    {PaperSize_ISO_C1,  25.5f, 36.1f},
    {PaperSize_ISO_C2,  18.0f, 25.5f},
    {PaperSize_ISO_C3,  12.8f, 18.0f},
    {PaperSize_ISO_C4,  9.02f, 12.8f},
    {PaperSize_ISO_C5,  6.38f, 9.02f},
    {PaperSize_ISO_C6,  4.49f, 6.38f},
    {PaperSize_ISO_C7,  3.19f, 4.49f},
    {PaperSize_ISO_C8,  2.24f, 3.19f},
    {PaperSize_ISO_C9,  1.57f, 2.24f},
    {PaperSize_ISO_C10, 1.10f, 1.57f},
    {PaperSize_DIN476_4A0, 66.22f, 93.62f},
    {PaperSize_DIN476_2A0, 46.81f, 66.22f},
    {PaperSize_JIS_B0,  40.55f, 57.32f},
    {PaperSize_JIS_B1,  28.66f, 40.55f},
    {PaperSize_JIS_B2,  20.28f, 28.66f},
    {PaperSize_JIS_B3,  14.33f, 20.28f},
    {PaperSize_JIS_B4,  10.12f, 14.33f},
    {PaperSize_JIS_B5,  7.17f, 10.12f},
    {PaperSize_JIS_B6,  5.04f, 7.17f},
    {PaperSize_JIS_B7,  3.58f, 5.04f},
    {PaperSize_JIS_B8,  2.52f, 3.58f},
    {PaperSize_JIS_B9,  1.77f, 2.52f},
    {PaperSize_JIS_B10, 1.26f, 1.77f},
    {PaperSize_JIS_B11, 0.87f, 1.26f},
    {PaperSize_JIS_B12, 0.63f, 0.87f},
    {PaperSize_JIS_ShirokuBan4, 10.39f, 14.92f},
    {PaperSize_JIS_ShirokuBan5, 7.44f, 10.31f},
    {PaperSize_JIS_ShirokuBan6, 5.00f, 7.40f},
    {PaperSize_JIS_Kiku4, 8.94f, 12.05f},
    {PaperSize_JIS_Kiku5, 5.94f, 8.94f},
    {PaperSize_NA_Letter, 8.5f, 11.0f},
    {PaperSize_NA_GovermentLetter, 8.0f, 10.5f},
    {PaperSize_NA_Legal, 8.5f, 14.0f},
    {PaperSize_NA_JuniorLegal, 8.0f, 5.0f},
    {PaperSize_NA_Ledger,  17.0f, 11.0f},
    {PaperSize_NA_Tabloid, 11.0f, 17.0f},
    {PaperSize_ArchA,  9.0f, 12.0f},
    {PaperSize_ArchB,  12.0f, 18.0f},
    {PaperSize_ArchC,  18.0f, 24.0f},
    {PaperSize_ArchD,  24.0f, 36.0f},
    {PaperSize_ArchE,  36.0f, 48.0f},
    {PaperSize_ArchE1, 30.0f, 42.0f},
    {PaperSize_ArchE2, 26.0f, 38.0f},
    {PaperSize_ArchE3, 27.0f, 39.0f},
    {PaperSize_UNKNOWN, -1.0f, -1.0f}};
}

namespace pdfout{

  PaperSizeInfo getPaperSizeInfo(PaperSize paperSize, PaperOrientation paperOrientation){
    auto elem = std::find_if(paperSizeInfo, paperSizeInfo + sizeof(paperSizeInfo) / sizeof(PaperSizeInfo) - 1, [paperSize](PaperSizeInfo const &value) -> bool {return value.mPaperSize == paperSize;});
    switch (paperOrientation){
      case PaperOrientationPortrait:
        return *elem;

      case PaperOrientationLandscape:
        return {elem->mPaperSize, elem->mHeight, elem->mWidth};
    }

    RUNTIME_EXCEPTION("Invalid PaperSize or PaperOrientation values");
  }

  float getPaperWidth(PaperSize paperSize, PaperOrientation paperOrientation){
    return getPaperSizeInfo(paperSize, paperOrientation).mWidth;
  }

  float getPaperHeight(PaperSize paperSize, PaperOrientation paperOrientation){
    return getPaperSizeInfo(paperSize, paperOrientation).mHeight;
  }

  struct Rectangle getPaperBoundaries(PaperSize paperSize, PaperOrientation paperOrientation){
    auto elem = std::find_if(paperSizeInfo, paperSizeInfo + sizeof(paperSizeInfo) / sizeof(PaperSizeInfo) - 1, [paperSize](PaperSizeInfo const &value) -> bool {return value.mPaperSize == paperSize;});
    switch (paperOrientation){
      case PaperOrientationPortrait:
        return {0.0f, 0.0f, elem->mWidth * 72.f, elem->mHeight * 72.f};

      case PaperOrientationLandscape:
        return {0.0f, 0.0f, elem->mHeight * 72.f, elem->mWidth * 72.f};
    }

    RUNTIME_EXCEPTION("Invalid PaperSize or PaperOrientation values");
  }
}
