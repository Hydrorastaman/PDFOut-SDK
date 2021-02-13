#pragma once

#include "PdfOutConfig.h"

namespace pdfout{

  /**
   * @brief Document version
   */
  enum DocumentVersion{
    DocumentVersion10, /**< Creates PDF v1.0 */
    DocumentVersion11, /**< Creates PDF v1.1 */
    DocumentVersion12, /**< Creates PDF v1.2 */
    DocumentVersion13, /**< Creates PDF v1.3 */
    DocumentVersion14, /**< Creates PDF v1.4 */
    DocumentVersion15, /**< Creates PDF v1.5 */
    DocumentVersion16, /**< Creates PDF v1.6 */
    DocumentVersion17, /**< Creates PDF v1.7 */
    DocumentVersion20  /**< Creates PDF v2.0 */
  };

  /**
   * @brief Platform ID names
   */
  enum PlatformId{
    PlatformIdUnicode = 0,   /**< Unicode */
    PlatformIdMacintosh = 1, /**< Macintosh */
    PlatformIdISO = 2,       /**< ISO/IEC 10646 [deprecated] */
    PlatformIdWindows = 3,   /**< Windows */
    PlatformIdCustom = 4     /**< Custom */
  };

  /**
    * @brief When the Platform ID is 0 (Unicode), the Encoding ID is interpreted as follows:
    */
  enum UnicodeEncoding{
    UnicodeEncodingDefault = 0,       /**< Default semantics */
    UnicodeEncodingVersion = 1,       /**< Version 1.1 semantics */
    UnicodeEncodingISO = 2,           /**< ISO 10646 1993 semantics (deprecated) */
    UnicodeEncodingUnicodeBmp = 3,    /**< Unicode 2.0 or later semantics (BMP only) */
    UnicodeEncodingUnicodeNonBmp = 4, /**< Unicode 2.0 or later semantics (non-BMP characters allowed) */
    UnicodeEncodingUVS = 5,           /**< Unicode Variation Sequences */
    UnicodeEncodingFullUnicode = 6    /**< Full Unicode coverage (used with type 13.0 cmaps by OpenType) */
  };

  /**
    * @brief When the Platform ID is 1 (Macintosh), the Encoding ID is interpreted as follows:
    */
  enum MacintoshEncoding{
    MacintoshEncodingRoman = 0,               /**< Roman */
    MacintoshEncodingJapanese = 1,            /**< Japanese */
    MacintoshEncodingTraditionalChinese = 2,  /**< Traditional Chinese */
    MacintoshEncodingKorean = 3,              /**< Korean */
    MacintoshEncodingArabic = 4,              /**< Arabic */
    MacintoshEncodingHebrew = 5,              /**< Hebrew */
    MacintoshEncodingGreek = 6,               /**< Greek */
    MacintoshEncodingRussian = 7,             /**< Russian */
    MacintoshEncodingRSymbol = 8,             /**< RSymbol */
    MacintoshEncodingDevanagari = 9,          /**< Devanagari */
    MacintoshEncodingGurmukhi = 10,           /**< Gurmukhi */
    MacintoshEncodingGujarati = 11,           /**< Gujarati */
    MacintoshEncodingOriya = 12,              /**< Oriya */
    MacintoshEncodingBengali = 13,            /**< Bengali */
    MacintoshEncodingTamil = 14,              /**< Tamil */
    MacintoshEncodingTelugu = 15,             /**< Telugu */
    MacintoshEncodingKannada = 16,            /**< Kannada */
    MacintoshEncodingMalayalam = 17,          /**< Malayalam */
    MacintoshEncodingSinhalese = 18,          /**< Sinhalese */
    MacintoshEncodingBurmese = 19,            /**< Burmese */
    MacintoshEncodingKhmer = 20,              /**< Khmer */
    MacintoshEncodingThai = 21,               /**< Thai */
    MacintoshEncodingLaotian = 22,            /**< Laotian */
    MacintoshEncodingGeorgian = 23,           /**< Georgian */
    MacintoshEncodingArmenian = 24,           /**< Armenian */
    MacintoshEncodingSimplifiedChinese = 25,  /**< Simplified Chinese */
    MacintoshEncodingTibetan = 26,            /**< Tibetan */
    MacintoshEncodingMongolian = 27,          /**< Mongolian */
    MacintoshEncodingGeez = 28,               /**< Geez */
    MacintoshEncodingSlavic = 29,             /**< Slavic */
    MacintoshEncodingVietnamese = 30,         /**< Vietnamese */
    MacintoshEncodingSindhi = 31,             /**< Sindhi */
    MacintoshEncodingUninterpreted = 32       /**< (Uninterpreted) */
  };

  /**
    * @brief When the Platform ID is 3 (Windows), the Encoding ID is interpreted as follows:
    */
  enum MicrosoftEncoding{
    MicrosoftEncodingSymbol = 0,   /**< Symbol */
    MicrosoftEncodingUSC2 = 1,     /**< Unicode BMP-only (UCS-2) */
    MicrosoftEncodingShiftJIS = 2, /**< Shift-JIS */
    MicrosoftEncodingPRC = 3,      /**< PRC */
    MicrosoftEncodingBigFive = 4,  /**< BigFive */
    MicrosoftEncodingJohab = 5,    /**< Johab */
    MicrosoftEncodingUSC4 = 10     /**< Unicode UCS-4 */
  };

  /**
   * @brief When the Platform ID is 1 (Macintosh), the Language ID is interpreted as follows (Language):
   */
  enum MacintoshLanguage{
    MacintoshLanguageEnglish = 0,
    MacintoshLanguageFrench = 1,
    MacintoshLanguageGerman = 2,
    MacintoshLanguageItalian = 3,
    MacintoshLanguageDutch = 4,
    MacintoshLanguageSwedish = 5,
    MacintoshLanguageSpanish = 6,
    MacintoshLanguageDanish = 7,
    MacintoshLanguagePortuguese = 8,
    MacintoshLanguageNorwegian = 9,
    MacintoshLanguageHebrew = 10,
    MacintoshLanguageJapanese = 11,
    MacintoshLanguageArabic = 12,
    MacintoshLanguageFinnish = 13,
    MacintoshLanguageGreek = 14,
    MacintoshLanguageIcelandic = 15,
    MacintoshLanguageMaltese = 16,
    MacintoshLanguageTurkish = 17,
    MacintoshLanguageCroatian = 18,
    MacintoshLanguageChineseTraditional = 19,
    MacintoshLanguageUrdu = 20,
    MacintoshLanguageHindi = 21,
    MacintoshLanguageThai = 22,
    MacintoshLanguageKorean = 23,
    MacintoshLanguageLithuanian = 24,
    MacintoshLanguagePolish = 25,
    MacintoshLanguageHungarian = 26,
    MacintoshLanguageEstonian = 27,
    MacintoshLanguageLatvian = 28,
    MacintoshLanguageSami = 29,
    MacintoshLanguageFaroese = 30,
    MacintoshLanguageFarsiPersian = 31,
    MacintoshLanguageRussian = 32,
    MacintoshLanguageChineseSimplified = 33,
    MacintoshLanguageFlemish = 34,
    MacintoshLanguageIrishGaelic = 35,
    MacintoshLanguageAlbanian = 36,
    MacintoshLanguageRomanian = 37,
    MacintoshLanguageCzech = 38,
    MacintoshLanguageSlovak = 39,
    MacintoshLanguageSlovenian = 40,
    MacintoshLanguageYiddish = 41,
    MacintoshLanguageSerbian = 42,
    MacintoshLanguageMacedonian = 43,
    MacintoshLanguageBulgarian = 44,
    MacintoshLanguageUkrainian = 45,
    MacintoshLanguageByelorussian = 46,
    MacintoshLanguageUzbek = 47,
    MacintoshLanguageKazakh = 48,
    MacintoshLanguageAzerbaijaniCyrillic = 49,
    MacintoshLanguageAzerbaijaniArabic = 50,
    MacintoshLanguageArmenian = 51,
    MacintoshLanguageGeorgian = 52,
    MacintoshLanguageMoldavian = 53,
    MacintoshLanguageKirghiz = 54,
    MacintoshLanguageTajiki = 55,
    MacintoshLanguageTurkmen = 56,
    MacintoshLanguageMongolianMongolian = 57,
    MacintoshLanguageMongolianCyrillic = 58,
    MacintoshLanguagePashto = 59,
    MacintoshLanguageKurdish = 60,
    MacintoshLanguageKashmiri = 61,
    MacintoshLanguageSindhi = 62,
    MacintoshLanguageTibetan = 63,
    MacintoshLanguageNepali = 64,
    MacintoshLanguageSanskrit = 65,
    MacintoshLanguageMarathi = 66,
    MacintoshLanguageBengali = 67,
    MacintoshLanguageAssamese = 68,
    MacintoshLanguageGujarati = 69,
    MacintoshLanguagePunjabi = 70,
    MacintoshLanguageOriya = 71,
    MacintoshLanguageMalayalam = 72,
    MacintoshLanguageKannada = 73,
    MacintoshLanguageTamil = 74,
    MacintoshLanguageTelugu = 75,
    MacintoshLanguageSinhalese = 76,
    MacintoshLanguageBurmese = 77,
    MacintoshLanguageKhmer = 78,
    MacintoshLanguageLao = 79,
    MacintoshLanguageVietnamese = 80,
    MacintoshLanguageIndonesian = 81,
    MacintoshLanguageTagalog = 82,
    MacintoshLanguageMalayRoman = 83,
    MacintoshLanguageMalayArabic = 84,
    MacintoshLanguageAmharic = 85,
    MacintoshLanguageTigrinya = 86,
    MacintoshLanguageGalla = 87,
    MacintoshLanguageSomali = 88,
    MacintoshLanguageSwahili = 89,
    MacintoshLanguageKinyarwandaRuanda = 90,
    MacintoshLanguageRundi = 91,
    MacintoshLanguageNyanjaChewa = 92,
    MacintoshLanguageMalagasy = 93,
    MacintoshLanguageEsperanto = 94,
    MacintoshLanguageWelsh = 128,
    MacintoshLanguageBasque = 129,
    MacintoshLanguageCatalan = 130,
    MacintoshLanguageLatin = 131,
    MacintoshLanguageQuechua = 132,
    MacintoshLanguageGuarani = 133,
    MacintoshLanguageAymara = 134,
    MacintoshLanguageTatar = 135,
    MacintoshLanguageUighur = 136,
    MacintoshLanguageDzongkha = 137,
    MacintoshLanguageJavaneseRoman = 138,
    MacintoshLanguageSundaneseRoman = 139,
    MacintoshLanguageGalician = 140,
    MacintoshLanguageAfrikaans = 141,
    MacintoshLanguageBreton = 142,
    MacintoshLanguageInuktitut = 143,
    MacintoshLanguageScottishGaelic = 144,
    MacintoshLanguageManxGaelic = 145,
    MacintoshLanguageIrishGaelicWithDotAbove = 146,
    MacintoshLanguageTongan = 147,
    MacintoshLanguageGreekPolytonic = 148,
    MacintoshLanguageGreenlandic = 149,
    MacintoshLanguageAzerbaijaniRoman = 150
  };

  /**
   * @brief When the Platform ID is 3 (Windows), the Language ID is interpreted as follows (Primary Language +	Region):
   */
  enum WindowsLanguage{
    WindowsLanguageAfrikaansSouthAfrica = 0x0436,
    WindowsLanguageAlbanianAlbania = 0x041C,
    WindowsLanguageAlsatianFrance = 0x0484,
    WindowsLanguageAmharicEthiopia = 0x045E,
    WindowsLanguageArabicAlgeria = 0x1401,
    WindowsLanguageArabicBahrain = 0x3C01,
    WindowsLanguageArabicEgypt = 0x0C01,
    WindowsLanguageArabicIraq = 0x0801,
    WindowsLanguageArabicJordan = 0x2C01,
    WindowsLanguageArabicKuwait = 0x3401,
    WindowsLanguageArabicLebanon = 0x3001,
    WindowsLanguageArabicLibya = 0x1001,
    WindowsLanguageArabicMorocco = 0x1801,
    WindowsLanguageArabicOman = 0x2001,
    WindowsLanguageArabicQatar = 0x4001,
    WindowsLanguageArabicSaudiArabia = 0x0401,
    WindowsLanguageArabicSyria = 0x2801,
    WindowsLanguageArabicTunisia = 0x1C01,
    WindowsLanguageArabicUAE = 0x3801,
    WindowsLanguageArabicYemen = 0x2401,
    WindowsLanguageArmenianArmenia = 0x042B,
    WindowsLanguageAssameseIndia = 0x044D,
    WindowsLanguageAzeriCyrillicAzerbaijan = 0x082C,
    WindowsLanguageAzeriLatinAzerbaijan = 0x042C,
    WindowsLanguageBashkirRussia = 0x046D,
    WindowsLanguageBasqueBasque = 0x042D,
    WindowsLanguageBelarusianBelarus = 0x0423,
    WindowsLanguageBengaliBangladesh = 0x0845,
    WindowsLanguageBengaliIndia = 0x0445,
    WindowsLanguageBosnianCyrillicBosniaAndHerzegovina = 0x201A,
    WindowsLanguageBosnianLatinBosniaAndHerzegovina = 0x141A,
    WindowsLanguageBretonFrance = 0x047E,
    WindowsLanguageBulgarianBulgaria = 0x0402,
    WindowsLanguageCatalanCatalan = 0x0403,
    WindowsLanguageChineseHongKongSAR = 0x0C04,
    WindowsLanguageChineseMacaoSAR = 0x1404,
    WindowsLanguageChinesePRC = 0x0804,
    WindowsLanguageChineseSingapore = 0x1004,
    WindowsLanguageChineseTaiwan = 0x0404,
    WindowsLanguageCorsicanFrance = 0x0483,
    WindowsLanguageCroatianCroatia = 0x041A,
    WindowsLanguageCroatianLatinBosniaAndHerzegovina = 0x101A,
    WindowsLanguageCzechCzechRepublic = 0x0405,
    WindowsLanguageDanishDenmark = 0x0406,
    WindowsLanguageDariAfghanistan = 0x048C,
    WindowsLanguageDivehiMaldives = 0x0465,
    WindowsLanguageDutchBelgium = 0x0813,
    WindowsLanguageDutchNetherlands = 0x0413,
    WindowsLanguageEnglishAustralia = 0x0C09,
    WindowsLanguageEnglishBelize = 0x2809,
    WindowsLanguageEnglishCanada = 0x1009,
    WindowsLanguageEnglishCaribbean = 0x2409,
    WindowsLanguageEnglishIndia = 0x4009,
    WindowsLanguageEnglishIreland = 0x1809,
    WindowsLanguageEnglishJamaica = 0x2009,
    WindowsLanguageEnglishMalaysia = 0x4409,
    WindowsLanguageEnglishNewZealand = 0x1409,
    WindowsLanguageEnglishRepublicOfThePhilippines = 0x3409,
    WindowsLanguageEnglishSingapore = 0x4809,
    WindowsLanguageEnglishSouthAfrica = 0x1C09,
    WindowsLanguageEnglishTrinidadAndTobago = 0x2C09,
    WindowsLanguageEnglishUnitedKingdom = 0x0809,
    WindowsLanguageEnglishUnitedStates = 0x0409,
    WindowsLanguageEnglishZimbabwe = 0x3009,
    WindowsLanguageEstonianEstonia = 0x0425,
    WindowsLanguageFaroeseFaroeIslands = 0x0438,
    WindowsLanguageFilipinoPhilippines = 0x0464,
    WindowsLanguageFinnishFinland = 0x040B,
    WindowsLanguageFrenchBelgium = 0x080C,
    WindowsLanguageFrenchCanada = 0x0C0C,
    WindowsLanguageFrenchFrance = 0x040C,
    WindowsLanguageFrenchLuxembourg = 0x140C,
    WindowsLanguageFrenchPrincipalityOfMonaco = 0x180C,
    WindowsLanguageFrenchSwitzerland = 0x100C,
    WindowsLanguageFrisianNetherlands = 0x0462,
    WindowsLanguageGalicianGalician = 0x0456,
    WindowsLanguageGeorgianGeorgia = 0x0437,
    WindowsLanguageGermanAustria = 0x0C07,
    WindowsLanguageGermanGermany = 0x0407,
    WindowsLanguageGermanLiechtenstein = 0x1407,
    WindowsLanguageGermanLuxembourg = 0x1007,
    WindowsLanguageGermanSwitzerland = 0x807,
    WindowsLanguageGreekGreece = 0x0408,
    WindowsLanguageGreenlandicGreenland = 0x046F,
    WindowsLanguageGujaratiIndia = 0x0447,
    WindowsLanguageHausaLatinÒigeria = 0x0468,
    WindowsLanguageHebrewIsrael = 0x040D,
    WindowsLanguageHindiIndia = 0x0439,
    WindowsLanguageHungarianHungary = 0x040E,
    WindowsLanguageIcelandicIceland = 0x040F,
    WindowsLanguageIgboNigeria = 0x0470,
    WindowsLanguageIndonesianIndonesia = 0x0421,
    WindowsLanguageInuktitutCanada = 0x045D,
    WindowsLanguageInuktitutLatinCanada = 0x085D,
    WindowsLanguageIrishIreland = 0x083C,
    WindowsLanguageisiXhosaSouthAfrica = 0x0434,
    WindowsLanguageisiZuluSouthAfrica = 0x0435,
    WindowsLanguageItalianItaly = 0x0410,
    WindowsLanguageItalianSwitzerland = 0x0810,
    WindowsLanguageJapaneseJapan = 0x0411,
    WindowsLanguageKannadaIndia = 0x044B,
    WindowsLanguageKazakhKazakhstan = 0x043F,
    WindowsLanguageKhmerCambodia = 0x0453,
    WindowsLanguageKicheGuatemala = 0x0486,
    WindowsLanguageKinyarwandaRwanda = 0x0487,
    WindowsLanguageKiswahiliKenya = 0x0441,
    WindowsLanguageKonkaniIndia = 0x0457,
    WindowsLanguageKoreanKorea = 0x0412,
    WindowsLanguageKyrgyzKyrgyzstan = 0x0440,
    WindowsLanguageLaoLaoPDR = 0x0454,
    WindowsLanguageLatvianLatvia = 0x0426,
    WindowsLanguageLithuanianLithuania = 0x0427,
    WindowsLanguageLowerSorbianGermany = 0x082E,
    WindowsLanguageLuxembourgishLuxembourg = 0x046E,
    WindowsLanguageMacedonianFYROM = 0x042F,
    WindowsLanguageMalayBruneiDarussalam = 0x083E,
    WindowsLanguageMalayMalaysia = 0x043E,
    WindowsLanguageMalayalamIndia = 0x044C,
    WindowsLanguageMalteseMalta = 0x043A,
    WindowsLanguageMaoriNewZealand = 0x481,
    WindowsLanguageMapudungunChile = 0x047A,
    WindowsLanguageMarathiIndia = 0x044E,
    WindowsLanguageMohawkMohawk = 0x047C,
    WindowsLanguageMongolianCyrillicMongolia = 0x0450,
    WindowsLanguageMongolianTraditionalPRC = 0x0850,
    WindowsLanguageNepaliNepal = 0x0461,
    WindowsLanguageNorwegianBokmalNorway = 0x0414,
    WindowsLanguageNorwegianNynorskNorway = 0x0814,
    WindowsLanguageOccitanFrance = 0x0482,
    WindowsLanguageOdiaOriyaIndia = 0x0448,
    WindowsLanguagePashtoAfghanistan = 0x0463,
    WindowsLanguagePolishPoland = 0x0415,
    WindowsLanguagePortugueseBrazil = 0x0416,
    WindowsLanguagePortuguesePortugal = 0x0816,
    WindowsLanguagePunjabiIndia = 0x0446,
    WindowsLanguageQuechuaBolivia = 0x046B,
    WindowsLanguageQuechuaEcuador = 0x086B,
    WindowsLanguageQuechuaPeru = 0x0C6B,
    WindowsLanguageRomanianRomania = 0x0418,
    WindowsLanguageRomanshSwitzerland = 0x0417,
    WindowsLanguageRussianRussia = 0x0419,
    WindowsLanguageSamiInariFinland = 0x243B,
    WindowsLanguageSamiLuleNorway = 0x103B,
    WindowsLanguageSamiLuleSweden = 0x143B,
    WindowsLanguageSamiNorthernFinland = 0x0C3B,
    WindowsLanguageSamiNorthernNorway = 0x043B,
    WindowsLanguageSamiNorthernSweden = 0x083B,
    WindowsLanguageSamiSkoltFinland = 0x203B,
    WindowsLanguageSamiSouthernNorway = 0x183B,
    WindowsLanguageSamiSouthernSweden = 0x1C3B,
    WindowsLanguageSanskritIndia = 0x044F,
    WindowsLanguageSerbianCyrillicBosniaAndHerzegovina = 0x1C1A,
    WindowsLanguageSerbianCyrillicSerbia = 0x0C1A,
    WindowsLanguageSerbianLatinBosniaAndHerzegovina = 0x181A,
    WindowsLanguageSerbianLatinSerbia = 0x081A,
    WindowsLanguageSesothoSaLeboaSouthAfrica = 0x046C,
    WindowsLanguageSetswanaSouthAfrica = 0x432,
    WindowsLanguageSinhalaSriLanka = 0x045B,
    WindowsLanguageSlovakSlovakia = 0x041B,
    WindowsLanguageSlovenianSlovenia = 0x424,
    WindowsLanguageSpanishArgentina = 0x2C0A,
    WindowsLanguageSpanishBolivia = 0x400A,
    WindowsLanguageSpanishChile = 0x340A,
    WindowsLanguageSpanishColombia = 0x240A,
    WindowsLanguageSpanishCostaRica = 0x140A,
    WindowsLanguageSpanishDominicanRepublic = 0x1C0A,
    WindowsLanguageSpanishEcuador = 0x300A,
    WindowsLanguageSpanishElSalvador = 0x440A,
    WindowsLanguageSpanishGuatemala = 0x100A,
    WindowsLanguageSpanishHonduras = 0x480A,
    WindowsLanguageSpanishMexico = 0x080A,
    WindowsLanguageSpanishNicaragua = 0x4C0A,
    WindowsLanguageSpanishPanama = 0x180A,
    WindowsLanguageSpanishParaguay = 0x3C0A,
    WindowsLanguageSpanishPeru = 0x280A,
    WindowsLanguageSpanishPuertoRico = 0x500A,
    WindowsLanguageSpanishModernSortSpain = 0x0C0A,
    WindowsLanguageSpanishTraditionalSortSpain = 0x040A,
    WindowsLanguageSpanishUnitedStates = 0x540A,
    WindowsLanguageSpanishUruguay = 0x380A,
    WindowsLanguageSpanishVenezuela = 0x200A,
    WindowsLanguageSwedenFinland = 0x081D,
    WindowsLanguageSwedishSweden = 0x041D,
    WindowsLanguageSyriacSyria = 0x045A,
    WindowsLanguageTajikCyrillicTajikistan = 0x0428,
    WindowsLanguageTamazightLatinAlgeria = 0x085F,
    WindowsLanguageTamilIndia = 0x0449,
    WindowsLanguageTatarRussia = 0x0444,
    WindowsLanguageTeluguIndia = 0x044A,
    WindowsLanguageThaiThailand = 0x041E,
    WindowsLanguageTibetanPRC = 0x0451,
    WindowsLanguageTurkishTurkey = 0x041F,
    WindowsLanguageTurkmenTurkmenistan = 0x0442,
    WindowsLanguageUighurPRC = 0x0480,
    WindowsLanguageUkrainianUkraine = 0x0422,
    WindowsLanguageUpperSorbianGermany = 0x042E,
    WindowsLanguageUrduIslamicRepublicOfPakistan = 0x0420,
    WindowsLanguageUzbekCyrillicUzbekistan = 0x0843,
    WindowsLanguageUzbekLatinUzbekistan = 0x0443,
    WindowsLanguageVietnameseVietnam = 0x042A,
    WindowsLanguageWelshUnitedKingdom = 0x0452,
    WindowsLanguageWolofSenegal = 0x0488,
    WindowsLanguageYakutRussia = 0x0485,
    WindowsLanguageYiPRC = 0x0478,
    WindowsLanguageYorubaNigeria = 0x046A
  };

  /**
   * @brief IBM font class ID, for more info see https://www.microsoft.com/typography/otspec/ibmfc.htm
   */
  enum FontClassId{
    FontClassIdNoClassification = 0,   /**< This class ID is used to indicate that the associated font has no design classification or that the design classification is not of significance to the creator or user of the font resource. */
    FontClassIdOldstyleSerifs = 1,     /**< This style is generally based upon the Latin printing style of the 15th to 17th century, with a mild diagonal contrast in stroke emphasis (lighter in upper left to lower right, heavier in upper right to lower left) and bracketed serifs. */
    FontClassIdTransitionalSerifs = 2, /**< This style is generally based upon the Latin printing style of the 18th to 19th century, with a pronounced vertical contrast in stroke emphasis (vertical strokes being heavier than the horizontal strokes) and bracketed serifs.  */
    FontClassIdModernSerifs = 3,       /**< This style is generally based upon the Latin printing style of the 20th century, with an extreme contrast between the thick and thin portion of the strokes. */
    FontClassIdClarendonSerifs = 4,    /**< This style is a variation of the Oldstyle Serifs and the Transitional Serifs, with a mild vertical stroke contrast and bracketed serifs. */
    FontClassIdSlabSerifs = 5,         /**< This style is characterized by serifs with a square transition between the strokes and the serifs (no brackets). */
    FontClassIdFreeformSerifs = 7,     /**< This style includes serifs, but which expresses a design freedom that does not generally fit within the other serif design classifications. */
    FontClassIdSansSerif = 8,          /**< This style includes most basic letter forms (excluding Scripts and Ornamentals) that do not have serifs on the strokes. */
    FontClassIdOrnamentals = 9,        /**< This style includes highly decorated or stylized character shapes that are typically used in headlines. */
    FontClassIdScripts = 10,           /**< This style includes those typefaces that are designed to simulate handwriting. */
    FontClassIdSymbolic = 12           /**< This style is generally design independent, making it suitable for Pi and special characters (icons, dingbats, technical symbols, etc.) that may be used equally well with any font. */
  };

  /**
   * @brief Text decoration properties
   */
  enum TextDecorationProperty{
    TextDecorationPropertyUnderline = 0x1
  };

  /**
   * @brief Defines the coordinates of the lower-left and upper-right corners of a rectangle
   * PDF uses Decartes coordinate system
   */
  struct Rectangle{
    float mLowerLeftX;
    float mLowerLeftY;
    float mUpperRightX;
    float mUpperRightY;
  };

  struct Point{
    float mX;
    float mY;
  };

  enum LineCap{
    LineCapButt = 0,             /**< The stroke is squared off at the endpoint of the path. There is no projection beyond the end of the path. */
    LineCapRound = 1,            /**< A semicircular arc with a diameter equal to the line width is drawn around the endpoint and filled in. */
    LineCapProjectingSquare = 2  /**< The stroke continues beyond the endpoint of the path for a distance equal to half the line width and is squared off. */
  };

  enum LineJoin{
    LineJoinMiter = 0, /**< The outer edges of the strokes for the two segments are extended until they meet at an angle. */
    LineJoinRound = 1, /**< An arc of a circle with a diameter equal to the line width is drawn around the point where the two segments meet, connecting the outer edges of the strokes for the two segments. */
    LineJoinBevel = 2  /**< The two segments are finished with butt caps and the resulting notch beyond the ends of the segments is filled with a triangle. */
  };

  /**
   * @brief Rendering intent
   */
  enum RenderingIntent{
    RenderingIntentAbsoluteColorimetric, /**< Colors are represented solely with respect to the light source; no correction is made for the output medium's white point. */
    RenderingIntentRelativeColorimetric, /**< Colors are represented with respect to the combination of the light source and the output medium's white point. */
    RenderingIntentSaturation,           /**< Colors are represented in a manner that preserves or emphasizes saturation. Reproduction of in-gamut colors may or may not be colorimetrically accurate. */
    RenderingIntentPerceptual            /**< Colors are represented in a manner that provides a pleasing perceptual appearance. To preserve color relationships, both in-gamut and out-of-gamut colors are generally modified from their precise colorimetric values. */
  };

  /**
   * @brief Text rendering mode
   */
  enum TextRenderingMode{
    TextRenderingModeFill = 0,            /**< Fill text. */
    TextRenderingModeStroke = 1,          /**< Stroke text. */
    TextRenderingModeFillStroke = 2,      /**< Fill, then stroke text. */
    TextRenderingModeInvisible = 3,       /**< Neither fill nor stroke text (invisible). */
    TextRenderingModeFillClipp = 4,       /**< Fill text and add to path for clipping. */
    TextRenderingModeStrokeClipp = 5,     /**< Stroke text and add to path for clipping. */
    TextRenderingModeFillStrokeClipp = 6, /**< Fill, then stroke text and add to path for clipping. */
    TextRenderingModeClipp = 7            /**< Add text to path for clipping. */
  };

  /**
   * @brief Path-painting operators
   */
  enum PathPaintingOp{
    PathPaintingOpNope = 0,          /**< End the path object without filling or stroking it. */
    PathPaintingOpStroke,            /**< Stroke the path. */
    PathPaintingOpCloseAndStroke,    /**< Close and stroke the path. */
    PathPaintingOpFillNW,            /**< Fill the path, using the nonzero winding number rule to determine the region to fill. */
    PathPaintingOpFillEO,            /**< Fill the path, using the even-odd rule to determine the region to fill. */
    PathPaintingOpFillAndStrokeNW,   /**< Fill and then stroke the path, using the nonzero winding number rule to determine the region to fill. */
    PathPaintingOpFillAndStrokeEO,   /**< Fill and then stroke the path, using the even-odd rule to determine the region to fill. */
    PathPaintingOpCloseFillStrokeNW, /**< Close, fill, and then stroke the path, using the nonzero winding number rule to determine the region to fill. */
    PathPaintingOpCloseFillStrokeEO, /**< Close, fill, and then stroke the path, using the even-odd rule to determine the region to fill. */
    PathPaintingOpClippingNW,        /**< Modify the current clipping path by intersecting it with the current path, using the nonzero winding number rule to determine which regions lie inside the clipping path. */
    PathPaintingOpClippingEO         /**< Modify the current clipping path by intersecting it with the current path, using the even-odd rule to determine which regions lie inside the clipping path. */
  };

  /**
   * @brief Content alignment
   */
  enum Alignment{
    AlignmentLeft     = 0x01, /**< Aligns to the left */
    AlignmentRight    = 0x02, /**< Aligns to the right */
    AlignmentHCenter  = 0x04, /**< Centers horizontaly */
    AlignmentVCenter  = 0x08, /**< Centers verticaly */
    AlignmentTop      = 0x10, /**< Aligns to the top */
    AlignmentBottom   = 0x20, /**< Aligns to the bottom */
    AlignmentBaseline = 0x40 /**< Aligns to the base line */
  };

  /**
   * @brief Standard blend modes
   */
  enum BlendMode{
    /**
     * Standard separable blend modes
     */
    BlendModeNormal,     /**< Selects the source color, ignoring the backdrop */
    BlendModeMultiply,   /**< Multiplies the backdrop and source color values. The result color is always at least as dark as either of the two constituent colors. Multiplying any color with black produces black; multiplying with white leaves the original color unchanged. Painting successive overlapping objects with a color other than black or white produces progressively darker colors. */
    BlendModeScreen,     /**< Multiplies the complements of the backdrop and source color values, then complements the result. The result color is always at least as light as either of the two constituent colors. Screening any color with white produces white; screening with black leaves the original color unchanged. The effect is similar to projecting multiple photographic slides simultaneously onto a single screen. */
    BlendModeOverlay,    /**< Multiplies or screens the colors, depending on the backdrop color value. Source colors overlay the backdrop while preserving its highlights and shadows. The backdrop color is not replaced but is mixed with the source color to reflect the lightness or darkness of the backdrop. */
    BlendModeDarken,     /**< Selects the darker of the backdrop and source colors. The backdrop is replaced with the source where the source is darker; otherwise, it is left unchanged. */
    BlendModeLighten,    /**< Selects the lighter of the backdrop and source colors. The backdrop is replaced with the source where the source is lighter; otherwise, it is left unchanged. */
    BlendModeColorDodge, /**< Brightens the backdrop color to reflect the source color. Painting with black produces no changes. */
    BlendModeColorBurn,  /**< Darkens the backdrop color to reflect the source color. Painting with white produces no change. */
    BlendModeHardLight,  /**< Multiplies or screens the colors, depending on the source color value. The effect is similar to shining a harsh spotlight on the backdrop. */
    BlendModeSoftLight,  /**< Darkens or lightens the colors, depending on the source color value. The effect is similar to shining a diffused spotlight on the backdrop. */
    BlendModeDifference, /**< Subtracts the darker of the two constituent colors from the lighter color. Painting with white inverts the backdrop color; painting with black produces no change. */
    BlendModeExclusion,  /**< Produces an effect similar to that of the ::Difference mode but lower in contrast. Painting with white inverts the backdrop color; painting with black produces no change. */

    /**
     * Standard nonseparable blend modes
     */
    BlendModeHue,        /**< Creates a color with the hue of the source color and the saturation and luminosity of the backdrop color. */
    BlendModeSaturation, /**< Creates a color with the saturation of the source color and the hue and luminosity of the backdrop color.
                             Painting with this mode in an area of the backdrop that is a pure gray (no saturation) produces no change. */
    BlendModeColor,      /**< Creates a color with the hue and saturation of the source color and the luminosity of the backdrop color. */
    BlendModeLuminosity  /**< Creates a color with the luminosity of the source color and the hue and saturation of the backdrop color.
                             This produces an inverse effect to that of the Color mode. */
  };

  enum OverprintMode{
    OverprintModeZero = 0,
    OverprintModeNonZero = 1
  };

  /**
   * @brief Predefined filter names
   */
  enum FilterName{
    FilterNameASCIIHex,  /**< Decodes data that has been encoded in ASCII hexadecimal form */
    FilterNameASCII85,   /**< Decodes data that has been encoded in ASCII base-85 encoding and produces binary data */
    FilterNameLZW,       /**< Variable-length, adaptive compression method that has been adopted as one of the standard compression methods in the Tag Image File Format (TIFF) standard */
    FilterNameFlate,     /**< Based on the public-domain zlib/deflate compression method, which is a variable-length Lempel-Ziv adaptive compression method cascaded with adaptive Huffman coding */
    FilterNameRunLength, /**< Decodes data that has been encoded in a simple byte-oriented format based on run length */
    FilterNameCCITTFax,  /**< Decodes image data that has been encoded using either Group 3 or Group 4 CCITT facsimile (fax) encoding */
    FilterNameJBIG2,     /**< Decodes monochrome (1 bit per pixel) image data that has been encoded using JBIG2 encoding */
    FilterNameDCT,       /**< Decodes grayscale or color image data that has been encoded in the JPEG baseline format */
    FilterNameJPX        /**< Decodes data that has been encoded using the JPEG2000 compression method, an international standard for the compression and packaging of image data */
  };

  /**
   * @brief Writing mode
   */
  enum WritingMode{
    WritingModeHorizontal, /**< Horizontal writing mode */
    WritingModeVertical    /**< Vertical writing mode */
  };

  struct CIDMetrics{
    int32_t mLSB;     /**< Left side bearing */
    int32_t mWidth;   /**< Width of glyph, advance width == mLSB + mRSB + mWidth */
    int32_t mRSB;     /**< Right side bearing */
  };

}
