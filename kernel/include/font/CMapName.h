#pragma once

#include <string>

namespace kernel{ namespace font{

  /**
    * @brief Predefined CMap names
    */
  enum CMapName{
    /**< Chinese (Simplified) */
    CMapName_GB_EUC_H = 0,  /**< Microsoft Code Page 936 (lfCharSet 0x86), GB 2312_80 character set, EUC_CN encoding */
    CMapName_GB_EUC_V,      /**< Vertical version of GB_EUC_H */
    CMapName_GBpc_EUC_H,    /**< Mac OS, GB 2312_80 character set, EUC_CN encoding, Script Manager code 19 */
    CMapName_GBpc_EUC_V,    /**< Vertical version of GBpc_EUC_H */
    CMapName_GBK_EUC_H,     /**< Microsoft Code Page 936 (lfCharSet 0x86), GBK character set, GBK encoding */
    CMapName_GBK_EUC_V,     /**< Vertical version of GBK_EUC_H */
    CMapName_GBKp_EUC_H,    /**< Same as GBK_EUC_H but replaces half_width Latin characters with proportional forms and maps character code 0x24 to a dollar sign ($) instead of a yuan symbol (¥) */
    CMapName_GBKp_EUC_V,    /**< Vertical version of GBKp_EUC_H */
    CMapName_GBK2K_H,       /**< GB 18030_2000 character set, mixed 1_, 2_, and 4_byte encoding */
    CMapName_GBK2K_V,       /**< Vertical version of GBK2K_H */
    CMapName_UniGB_UCS2_H,  /**< Unicode (UCS-2) encoding for the Adobe_GB1 character collection */
    CMapName_UniGB_UCS2_V,  /**< Vertical version of UniGB_UCS2_H */
    CMapName_UniGB_UTF16_H, /**< Unicode (UTF-16BE) encoding for the Adobe_GB1 character collection; contains mappings for all characters in the GB18030_2000 character set */
    CMapName_UniGB_UTF16_V, /**< Vertical version of UniGB_UTF16_H */

    /**< Chinese (Traditional) */
    CMapName_B5pc_H,         /**< Mac OS, Big Five character set, Big Five encoding, Script Manager code 2 */
    CMapName_B5pc_V,         /**< Vertical version of B5pc_H */
    CMapName_HKscs_B5_H,     /**< Hong Kong SCS, an extension to the Big Five character set and encoding */
    CMapName_HKscs_B5_V,     /**< Vertical version of HKscs_B5_H */
    CMapName_ETen_B5_H,      /**< Microsoft Code Page 950 (lfCharSet 0x88), Big Five character set with ETen extensions */
    CMapName_ETen_B5_V,      /**< Vertical version of ETen_B5_H */
    CMapName_ETenms_B5_H,    /**< Same as ETen_B5_H but replaces half_width Latin characters with proportional forms */
    CMapName_ETenms_B5_V,    /**< Vertical version of ETenms_B5_H */
    CMapName_CNS_EUC_H,      /**< CNS 11643_1992 character set, EUC_TW encoding */
    CMapName_CNS_EUC_V,      /**< Vertical version of CNS_EUC_H */
    CMapName_UniCNS_UCS2_H,  /**< Unicode (UCS-2) encoding for the Adobe_CNS1 character collection */
    CMapName_UniCNS_UCS2_V,  /**< Vertical version of UniCNS_UCS2_H */
    CMapName_UniCNS_UTF16_H, /**< Unicode (UTF-16BE) encoding for the Adobe_CNS1 character collection; contains mappings for all the characters in the HKSCS_2001 character set and contains both 2_ and 4_byte character codes */
    CMapName_UniCNS_UTF16_V, /**< Vertical version of UniCNS_UTF16_H NAME DESCRIPTION */

    /**< Japanese */
    CMapName_83pv_RKSJ_H,  /**< Mac OS, JIS X 0208 character set with KanjiTalk6 extensions, Shift_JIS encoding, Script Manager code 1 */
    CMapName_90ms_RKSJ_H,  /**< Microsoft Code Page 932 (lfCharSet 0x80), JIS X 0208 character set with NEC and IBM®extensions */
    CMapName_90ms_RKSJ_V,  /**< Vertical version of 90ms_RKSJ_H */
    CMapName_90msp_RKSJ_H, /**< Same as 90ms_RKSJ_H but replaces half_width Latin characters with proportional forms */
    CMapName_90msp_RKSJ_V, /**< Vertical version of 90msp_RKSJ_H */
    CMapName_90pv_RKSJ_H,  /**< Mac OS, JIS X 0208 character set with KanjiTalk7 extensions, Shift_JIS encoding, Script Manager code 1 */
    CMapName_Add_RKSJ_H,   /**< JIS X 0208 character set with Fujitsu FMR extensions, Shift_JIS encoding */
    CMapName_Add_RKSJ_V,   /**< Vertical version of Add_RKSJ_H */
    CMapName_EUC_H,        /**< JIS X 0208 character set, EUC_JP encoding */
    CMapName_EUC_V,        /**< Vertical version of EUC_H */
    CMapName_Ext_RKSJ_H,   /**< JIS C 6226 (JIS78) character set with NEC extensions, Shift_JIS encoding */
    CMapName_Ext_RKSJ_V,   /**< Vertical version of Ext_RKSJ_H */
    CMapName_H, /**< JIS X 0208 character set, ISO_2022_JP encoding */
    CMapName_V, /**< Vertical version of H */
    CMapName_UniJIS_UCS2_H,    /**< Unicode (UCS-2) encoding for the Adobe_Japan1 character collection */
    CMapName_UniJIS_UCS2_V,    /**< Vertical version of UniJIS_UCS2_H */
    CMapName_UniJIS_UCS2_HW_H, /**< Same as UniJIS_UCS2_H but replaces proportional Latin characters with half_width forms */
    CMapName_UniJIS_UCS2_HW_V, /**< Vertical version of UniJIS_UCS2_HW_H */
    CMapName_UniJIS_UTF16_H,   /**< Unicode (UTF-16BE) encoding for the Adobe_Japan1 character collection; contains mappings for all characters in the JIS X 0213:1000 character set */
    CMapName_UniJIS_UTF16_V,   /**< Vertical version of UniJIS_UTF16_H */

    /**< Korean */
    CMapName_KSC_EUC_H,      /**< KS X 1001:1992 character set, EUC_KR encoding */
    CMapName_KSC_EUC_V,      /**< Vertical version of KSC_EUC_H */
    CMapName_KSCms_UHC_H,    /**< Microsoft Code Page 949 (lfCharSet 0x81), KS X 1001:1992 character set plus 8822 additional hangul, Unified Hangul Code (UHC) encoding */
    CMapName_KSCms_UHC_V,    /**< Vertical version of KSCms_UHC_H */
    CMapName_KSCms_UHC_HW_H, /**< Same as KSCms_UHC_H but replaces proportional Latin characters with half_width forms */
    CMapName_KSCms_UHC_HW_V, /**< Vertical version of KSCms_UHC_HW_H */
    CMapName_KSCpc_EUC_H,    /**< Mac OS, KS X 1001:1992 character set with Mac OS KH extensions, Script Manager Code 3 */
    CMapName_UniKS_UCS2_H,   /**< Unicode (UCS-2) encoding for the Adobe_Korea1 character collection */
    CMapName_UniKS_UCS2_V,   /**< Vertical version of UniKS_UCS2_H */
    CMapName_UniKS_UTF16_H,  /**< Unicode (UTF-16BE) encoding for the Adobe_Korea1 character collection */
    CMapName_UniKS_UTF16_V,  /**< Vertical version of UniKS_UTF16_H */

    /**< Generic */
    CMapName_Identity_H,     /**< The horizontal identity mapping for 2-byte CIDs; may be used with CIDFonts using any Registry, Ordering, and Supplement values. It maps 2_byte character codes ranging from 0 to 65,535 to the same 2_byte CID value, interpreted high_order byte first (see below). */
    CMapName_Identity_V,     /**< Vertical version of Identity_H. The mapping is the same as for Identity_H. */
    CMapName_Identity_UCS,
    CMapName_UNKNOWN
  };

  /**
   * @brief Information about predefined CMap name
   */
  struct CMapNameInfo{
    CMapName mCmapName;    /**< Predefined CMap name */
    std::string mRegistry; /**< Issuer of the character collection */
    std::string mOrdering; /**< Character collection within the specified registry */
    int mSupplement;       /**< The supplement number of the character collection */
  };

  CMapNameInfo getCMapNameInfo(CMapName cmapName);
  char const *cmapNameToString(CMapName cmapName);
}}
