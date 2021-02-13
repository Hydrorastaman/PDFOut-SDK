#pragma once

#include "Types.h"
#include "PdfOutConfig.h"

namespace pdfout{

  /**
   * @brief Pre-defined paper sizes
   */
  enum PaperSize{
    PaperSize_ISO_A0 = 0,         /**< 33.1 x 46.8 in inches */
    PaperSize_ISO_A1,             /**< 23.4 x 33.1 in inches */
    PaperSize_ISO_A2,             /**< 16.5 x 23.4 in inches */
    PaperSize_ISO_A3,             /**< 11.7 x 16.5 in inches */
    PaperSize_ISO_A4,             /**< 8.27 x 11.7 in inches */
    PaperSize_ISO_A5,             /**< 5.83 x 8.27 in inches */
    PaperSize_ISO_A6,             /**< 4.13 x 5.83 in inches */
    PaperSize_ISO_A7,             /**< 2.91 x 4.13 in inches */
    PaperSize_ISO_A8,             /**< 2.05 x 2.91 in inches */
    PaperSize_ISO_A9,             /**< 1.46 x 2.05 in inches */
    PaperSize_ISO_A10,            /**< 1.02 x 1.46 in inches */
    PaperSize_ISO_B0,             /**< 39.4 x 55.7 in inches */
    PaperSize_ISO_B1,             /**< 27.8 x 39.4 in inches */
    PaperSize_ISO_B2,             /**< 19.7 x 27.8 in inches */
    PaperSize_ISO_B3,             /**< 13.9 x 19.7 in inches */
    PaperSize_ISO_B4,             /**< 9.84 x 13.9 in inches */
    PaperSize_ISO_B5,             /**< 6.93 x 9.84 in inches */
    PaperSize_ISO_B6,             /**< 4.92 x 6.93 in inches */
    PaperSize_ISO_B7,             /**< 3.46 x 4.92 in inches */
    PaperSize_ISO_B8,             /**< 2.44 x 3.46 in inches */
    PaperSize_ISO_B9,             /**< 1.73 x 2.44 in inches */
    PaperSize_ISO_B10,            /**< 1.22 x 1.73 in inches */
    PaperSize_ISO_C0,             /**< 36.1 x 51.1 in inches */
    PaperSize_ISO_C1,             /**< 25.5 x 36.1 in inches */
    PaperSize_ISO_C2,             /**< 18.0 x 25.5 in inches */
    PaperSize_ISO_C3,             /**< 12.8 x 18.0 in inches */
    PaperSize_ISO_C4,             /**< 9.02 x 12.8 in inches */
    PaperSize_ISO_C5,             /**< 6.38 x 9.02 in inches */
    PaperSize_ISO_C6,             /**< 4.49 x 6.38 in inches */
    PaperSize_ISO_C7,             /**< 3.19 x 4.49 in inches */
    PaperSize_ISO_C8,             /**< 2.24 x 3.19 in inches */
    PaperSize_ISO_C9,             /**< 1.57 x 2.24 in inches */
    PaperSize_ISO_C10,            /**< 1.10 x 1.57 in inches */
    PaperSize_DIN476_4A0,         /**< 66.22 x 93.62 in inches */
    PaperSize_DIN476_2A0,         /**< 46.81 x 66.22 in inches */
    PaperSize_JIS_B0,             /**< 40.55 x 57.32 in inches */
    PaperSize_JIS_B1,             /**< 28.66 x 40.55 in inches */
    PaperSize_JIS_B2,             /**< 20.28 x 28.66 in inches */
    PaperSize_JIS_B3,             /**< 14.33 x 20.28 in inches */
    PaperSize_JIS_B4,             /**< 10.12 x 14.33 in inches */
    PaperSize_JIS_B5,             /**< 7.17 x 10.12 in inches */
    PaperSize_JIS_B6,             /**< 5.04 x 7.17 in inches */
    PaperSize_JIS_B7,             /**< 3.58 x 5.04 in inches */
    PaperSize_JIS_B8,             /**< 2.52 x 3.58 in inches */
    PaperSize_JIS_B9,             /**< 1.77 x 2.52 in inches */
    PaperSize_JIS_B10,            /**< 1.26 x 1.77 in inches */
    PaperSize_JIS_B11,            /**< 0.87 x 1.26 in inches */
    PaperSize_JIS_B12,            /**< 0.63 x 0.87 in inches */
    PaperSize_JIS_ShirokuBan4,    /**< 10.39 x 14.92 in inches */
    PaperSize_JIS_ShirokuBan5,    /**< 7.44 x 10.31 in inches */
    PaperSize_JIS_ShirokuBan6,    /**< 5.00 x 7.40 in inches */
    PaperSize_JIS_Kiku4,          /**< 8.94 x 12.05 in inches */
    PaperSize_JIS_Kiku5,          /**< 5.94 x 8.94  in inches */
    PaperSize_NA_Letter,          /**< 8.5 x 11 in inches */
    PaperSize_NA_GovermentLetter, /**< 8 x 10.5 in inches */
    PaperSize_NA_Legal,           /**< 8.5 x 14 in inches */
    PaperSize_NA_JuniorLegal,     /**<  8 x 5 in inches */
    PaperSize_NA_Ledger,          /**< 17 x 11 in inches */
    PaperSize_NA_Tabloid,         /**< 11 x 17 in inches */
    PaperSize_ArchA,              /**<  9 x 12 in inches*/
    PaperSize_ArchB,              /**< 12 x 18 in inches*/
    PaperSize_ArchC,              /**< 18 x 24 in inches*/
    PaperSize_ArchD,              /**< 24 x 36 in inches*/
    PaperSize_ArchE,              /**< 36 x 48 in inches*/
    PaperSize_ArchE1,             /**< 30 x 42 in inches*/
    PaperSize_ArchE2,             /**< 26 x 38 in inches*/
    PaperSize_ArchE3,             /**< 27 x 39 in inches*/
    PaperSize_UNKNOWN             /**< unknown paper size */
  };

  /**
   * @brief Paper orientation
   */
  enum PaperOrientation{
    PaperOrientationPortrait = 0, /**< Portrait paper orientation */
    PaperOrientationLandscape     /**< Landscape paper orientation */
  };

  /**
   * @brief Paper size in inches
   */
  struct PDFOUT_EXPORTS PaperSizeInfo{
    PaperSize mPaperSize; /**< Pre-defined paper size name */
    float mWidth;        /**< Width in inches */
    float mHeight;       /**< Height in inches */
  };

  /**
   * @brief Gets info for pre-defined paper sizes
   */
  PDFOUT_EXPORTS PaperSizeInfo getPaperSizeInfo(PaperSize paperSize, PaperOrientation paperOrientation = PaperOrientationPortrait);

  /**
   * @brief Gets paper width, in inches
   */
  PDFOUT_EXPORTS float getPaperWidth(PaperSize paperSize, PaperOrientation paperOrientation = PaperOrientationPortrait);

  /**
   * @brief Gets paper height, in inches
   */
  PDFOUT_EXPORTS float getPaperHeight(PaperSize paperSize, PaperOrientation paperOrientation = PaperOrientationPortrait);

  /**
   * @brief Gets paper boundaries, can be used directly to set MediaBox, CropBox and others.
   */
  PDFOUT_EXPORTS struct Rectangle getPaperBoundaries(PaperSize paperSize, PaperOrientation paperOrientation = PaperOrientationPortrait);
}
