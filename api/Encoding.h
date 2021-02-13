#pragma once

#include "PdfOutConfig.h"

namespace pdfout{

  /**
   Pre-defined encodings.
   */
  enum Encoding{
    Encoding_US_ASCII = 0,
    Encoding_ASCII,
    Encoding_ISO646_US,
    Encoding_ISO_646_IRV_1991,
    Encoding_ISO_IR_6,
    Encoding_ANSI_X3_4_1968,
    Encoding_ANSI_X3_4_1986,
    Encoding_CP367,
    Encoding_IBM367,
    Encoding_US,
    Encoding_CSASCII,
    Encoding_UTF8,
    Encoding_UCS2,
    Encoding_ISO_10646_UCS_2,
    Encoding_CSUNICODE,
    Encoding_UCS2BE,
    Encoding_UNICODEBIG,
    Encoding_UNICODE_1_1,
    Encoding_CSUNICODE11,
    Encoding_UCS2LE,
    Encoding_UNICODELITTLE,
    Encoding_UCS4,
    Encoding_ISO_10646_UCS_4,
    Encoding_CSUCS4,
    Encoding_UCS4BE,
    Encoding_UCS4LE,
    Encoding_UTF16,
    Encoding_UTF16BE,
    Encoding_UTF16LE,
    Encoding_UTF32,
    Encoding_UTF32BE,
    Encoding_UTF32LE,
    Encoding_UTF7,
    Encoding_UNICODE_1_1_UTF7,
    Encoding_CSUNICODE11UTF7,
    Encoding_UCS_2_INTERNAL,
    Encoding_UCS_2_SWAPPED,
    Encoding_UCS_4_INTERNAL,
    Encoding_UCS_4_SWAPPED,
    Encoding_C99,
    Encoding_JAVA,
    Encoding_ISO_8859_1,
    Encoding_ISO_8859_1_1987,
    Encoding_ISO_IR_100,
    Encoding_CP819,
    Encoding_IBM819,
    Encoding_LATIN1,
    Encoding_L1,
    Encoding_CSISOLATIN1,
    Encoding_ISO8859_1,
    Encoding_ISO_8859_2,
    Encoding_ISO_8859_2_1987,
    Encoding_ISO_IR_101,
    Encoding_LATIN2,
    Encoding_L2,
    Encoding_CSISOLATIN2,
    Encoding_ISO8859_2,
    Encoding_ISO_8859_3,
    Encoding_ISO_8859_3_1988,
    Encoding_ISO_IR_109,
    Encoding_LATIN3,
    Encoding_L3,
    Encoding_CSISOLATIN3,
    Encoding_ISO8859_3,
    Encoding_ISO_8859_4,
    Encoding_ISO_8859_4_1988,
    Encoding_ISO_IR_110,
    Encoding_LATIN4,
    Encoding_L4,
    Encoding_CSISOLATIN4,
    Encoding_ISO8859_4,
    Encoding_ISO_8859_5,
    Encoding_ISO_8859_5_1988,
    Encoding_ISO_IR_144,
    Encoding_CYRILLIC,
    Encoding_CSISOLATINCYRILLIC,
    Encoding_ISO8859_5,
    Encoding_ISO_8859_6,
    Encoding_ISO_8859_6_1987,
    Encoding_ISO_IR_127,
    Encoding_ECMA_114,
    Encoding_ASMO_708,
    Encoding_ARABIC,
    Encoding_CSISOLATINARABIC,
    Encoding_ISO8859_6,
    Encoding_ISO_8859_7,
    Encoding_ISO_8859_7_1987,
    Encoding_ISO_8859_7_2003,
    Encoding_ISO_IR_126,
    Encoding_ECMA_118,
    Encoding_ELOT_928,
    Encoding_GREEK8,
    Encoding_GREEK,
    Encoding_CSISOLATINGREEK,
    Encoding_ISO8859_7,
    Encoding_ISO_8859_8,
    Encoding_ISO_8859_8_1988,
    Encoding_ISO_IR_138,
    Encoding_HEBREW,
    Encoding_CSISOLATINHEBREW,
    Encoding_ISO8859_8,
    Encoding_ISO_8859_9,
    Encoding_ISO_8859_9_1989,
    Encoding_ISO_IR_148,
    Encoding_LATIN5,
    Encoding_L5,
    Encoding_CSISOLATIN5,
    Encoding_ISO8859_9,
    Encoding_ISO_8859_10,
    Encoding_ISO_8859_10_1992,
    Encoding_ISO_IR_157,
    Encoding_LATIN6,
    Encoding_L6,
    Encoding_CSISOLATIN6,
    Encoding_ISO8859_10,
    Encoding_ISO_8859_11,
    Encoding_ISO8859_11,
    Encoding_ISO_8859_13,
    Encoding_ISO_IR_179,
    Encoding_LATIN7,
    Encoding_L7,
    Encoding_ISO8859_13,
    Encoding_ISO_8859_14,
    Encoding_ISO_8859_14_1998,
    Encoding_ISO_IR_199,
    Encoding_LATIN8,
    Encoding_L8,
    Encoding_ISO_CELTIC,
    Encoding_ISO8859_14,
    Encoding_ISO_8859_15,
    Encoding_ISO_8859_15_1998,
    Encoding_ISO_IR_203,
    Encoding_LATIN9,
    Encoding_ISO8859_15,
    Encoding_ISO_8859_16,
    Encoding_ISO_8859_16_2001,
    Encoding_ISO_IR_226,
    Encoding_LATIN10,
    Encoding_L10,
    Encoding_ISO8859_16,
    Encoding_KOI8R,
    Encoding_CSKOI8R,
    Encoding_KOI8U,
    Encoding_KOI8RU,
    Encoding_CP1250,
    Encoding_WINDOWS_1250,
    Encoding_MS_EE,
    Encoding_CP1251,
    Encoding_WINDOWS_1251,
    Encoding_MS_CYRL,
    Encoding_CP1252,
    Encoding_WINDOWS_1252,
    Encoding_MS_ANSI,
    Encoding_CP1253,
    Encoding_WINDOWS_1253,
    Encoding_MS_GREEK,
    Encoding_CP1254,
    Encoding_WINDOWS_1254,
    Encoding_MS_TURK,
    Encoding_CP1255,
    Encoding_WINDOWS_1255,
    Encoding_MS_HEBR,
    Encoding_CP1256,
    Encoding_WINDOWS_1256,
    Encoding_MS_ARAB,
    Encoding_CP1257,
    Encoding_WINDOWS_1257,
    Encoding_WINBALTRIM,
    Encoding_CP1258,
    Encoding_WINDOWS_1258,
    Encoding_CP850,
    Encoding_IBM850,
    Encoding_850,
    Encoding_CSPC850MULTILINGUAL,
    Encoding_CP862,
    Encoding_IBM862,
    Encoding_862,
    Encoding_CSPC862LATINHEBREW,
    Encoding_CP866,
    Encoding_IBM866,
    Encoding_866,
    Encoding_CSIBM866,
    Encoding_CP1131,
    Encoding_MACROMAN,
    Encoding_MACINTOSH,
    Encoding_MAC,
    Encoding_CSMACINTOSH,
    Encoding_MACCENTRALEUROPE,
    Encoding_MACICELAND,
    Encoding_MACCROATIAN,
    Encoding_MACROMANIA,
    Encoding_MACCYRILLIC,
    Encoding_MACUKRAINE,
    Encoding_MACGREEK,
    Encoding_MACTURKISH,
    Encoding_MACHEBREW,
    Encoding_MACARABIC,
    Encoding_MACTHAI,
    Encoding_HP_ROMAN8,
    Encoding_ROMAN8,
    Encoding_R8,
    Encoding_CSHPROMAN8,
    Encoding_NEXTSTEP,
    Encoding_ARMSCII_8,
    Encoding_GEORGIAN_ACADEMY,
    Encoding_GEORGIAN_PS,
    Encoding_KOI8_T,
    Encoding_PT154,
    Encoding_PTCP154,
    Encoding_CP154,
    Encoding_CYRILLIC_ASIAN,
    Encoding_CSPTCP154,
    Encoding_RK1048,
    Encoding_STRK1048_2002,
    Encoding_KZ_1048,
    Encoding_CSKZ1048,
    Encoding_MULELAO_1,
    Encoding_CP1133,
    Encoding_IBM_CP1133,
    Encoding_TIS_620,
    Encoding_TIS620,
    Encoding_TIS620_0,
    Encoding_TIS620_2529_1,
    Encoding_TIS620_2533_0,
    Encoding_TIS620_2533_1,
    Encoding_ISO_IR_166,
    Encoding_CP874,
    Encoding_WINDOWS_874,
    Encoding_VISCII,
    Encoding_VISCII1_1_1,
    Encoding_CSVISCII,
    Encoding_TCVN,
    Encoding_TCVN_5712,
    Encoding_TCVN5712_1,
    Encoding_TCVN5712_1_1993,
    Encoding_JIS_C6220_1969_RO,
    Encoding_ISO646_JP,
    Encoding_ISO_IR_14,
    Encoding_JP,
    Encoding_CSISO14JISC6220RO,
    Encoding_JIS_X0201,
    Encoding_JISX0201_1976,
    Encoding_X0201,
    Encoding_CSHALFWIDTHKATAKANA,
    Encoding_JIS_X0208,
    Encoding_JIS_X0208_1983,
    Encoding_JIS_X0208_1990,
    Encoding_JIS0208,
    Encoding_X0208,
    Encoding_ISO_IR_87,
    Encoding_JIS_C6226_1983,
    Encoding_CSISO87JISX0208,
    Encoding_JIS_X0212,
    Encoding_JIS_X0212_1990_0,
    Encoding_JIS_X0212_1990,
    Encoding_X0212,
    Encoding_ISO_IR_159,
    Encoding_CSISO159JISX02121990,
    Encoding_GB_1988_80,
    Encoding_ISO646_CN,
    Encoding_ISO_IR_57,
    Encoding_CN,
    Encoding_CSISO57GB1988,
    Encoding_GB_2312_80,
    Encoding_ISO_IR_58,
    Encoding_CSISO58GB231280,
    Encoding_CHINESE,
    Encoding_ISO_IR_165,
    Encoding_CN_GB_ISOIR165,
    Encoding_KSC_5601,
    Encoding_KS_C_5601_1987,
    Encoding_KS_C_5601_1989,
    Encoding_ISO_IR_149,
    Encoding_CSKSC56011987,
    Encoding_KOREAN,
    Encoding_EUC_JP,
    Encoding_EUCJP,
    Encoding_EXTENDED_UNIX_CODE_PACKED_FORMAT_FOR_JAPANESE,
    Encoding_CSEUCPKDFMTJAPANESE,
    Encoding_SHIFT_JIS,
    Encoding_SJIS,
    Encoding_MS_KANJI,
    Encoding_CSSHIFTJIS,
    Encoding_CP932,
    Encoding_ISO_2022_JP,
    Encoding_CSISO2022JP,
    Encoding_ISO_2022_JP_1,
    Encoding_ISO_2022_JP_2,
    Encoding_CSISO2022JP2,
    Encoding_EUC_CN,
    Encoding_EUCCN,
    Encoding_GB2312,
    Encoding_CN_GB,
    Encoding_CSGB2312,
    Encoding_GBK,
    Encoding_CP936,
    Encoding_MS936,
    Encoding_WINDOWS_936,
    Encoding_GB18030,
    Encoding_ISO_2022_CN,
    Encoding_CSISO2022CN,
    Encoding_ISO_2022_CN_EXT,
    Encoding_HZ,
    Encoding_HZ_GB_2312,
    Encoding_EUC_TW,
    Encoding_EUCTW,
    Encoding_CSEUCTW,
    Encoding_BIG5,
    Encoding_BIG_5,
    Encoding_BIG_FIVE,
    Encoding_BIGFIVE,
    Encoding_CN_BIG5,
    Encoding_CSBIG5,
    Encoding_CP950,
    Encoding_BIG5_HKSCS_1999,
    Encoding_BIG5_HKSCS_2001,
    Encoding_BIG5_HKSCS_2004,
    Encoding_BIG5_HKSCS,
    Encoding_BIG5HKSCS,
    Encoding_BIG5_HKSCS_2008,
    Encoding_EUC_KR,
    Encoding_EUCKR,
    Encoding_CSEUCKR,
    Encoding_CP949,
    Encoding_UHC,
    Encoding_JOHAB,
    Encoding_CP1361,
    Encoding_ISO_2022_KR,
    Encoding_CSISO2022KR,
    Encoding_CHAR,
    Encoding_WCHAR_T,
    Encoding_CP437,
    Encoding_CP737,
    Encoding_CP775,
    Encoding_CP852,
    Encoding_CP853,
    Encoding_CP855,
    Encoding_CP857,
    Encoding_CP858,
    Encoding_CP860,
    Encoding_CP861,
    Encoding_CP863,
    Encoding_CP864,
    Encoding_CP865,
    Encoding_CP869,
    Encoding_CP1125,
    Encoding_UNKNOWN
  };

  /**
   Gets Encoding with specified name.

   @param name Encoding name;
   @return Pre-defined encoding value corresponding to its name;

   @code {.cpp}
     Encoding const cp865 = GetEncodingByName("CP865");
   @endcod
   */
  PDFOUT_EXPORTS Encoding GetEncodingByName(char const *name);
}