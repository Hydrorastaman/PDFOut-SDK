#pragma once

#include <memory>
#include <InternalTools.h>

namespace pdfout{
  class InputStream;
}

namespace kernel{ namespace font{ namespace cff{

  class Index;
  class TopDictionary;

  /**
   * Implementation for Compact Font Format
   */
  class FontCFF{
  private:
    /**
    * @brief CFF Header.
    */
    PACK(struct CffHeader{
      uint8_t mVersionMajor; /**< Format major version */
      uint8_t mVersionMinor; /**< Format minor version */
      uint8_t mHeaderSize;   /**< Header size in bytes */
      uint8_t mOffSize;      /**< Absolute offset size */
    });

  public:
    FontCFF(pdfout::InputStream *stream);
    ~FontCFF();
    FontCFF(FontCFF const &) = delete;
    FontCFF &operator=(FontCFF const &) = delete;

  private:
    std::unique_ptr<Index> mNameIndex;
    std::unique_ptr<TopDictionary> mTopDictionary;
    std::unique_ptr<Index> mStringIndex;
    std::unique_ptr<Index> mGlobalSubrIndex;
  };

}}}
