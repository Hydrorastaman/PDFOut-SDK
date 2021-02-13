#include <font/TrueType/FontTrueTypeCollection.h>

#include <InternalTools.h>
#include <stream/InputStream.h>
#include <font/TrueType/FontTrueType.h>

namespace{
  /**
    * @brief TrueType TTC Header Version 1.0
    */
  PACK(struct TrueTypeTTCv10{
    char mTag[4];            /**< TrueType Collection ID string: 'ttcf' */
    uint16_t mMajorVersion;  /**< Version of the TTC Header (1.0), 0x00010000, or 2.0, 0x00020000 */
    uint16_t mMinorVersion;  /**< Minor version of the TTC Header, 0 */
    uint32_t mNumFonts;      /**< Number of fonts in TTC */
    //uint32_t mOffsetTable[mNumFonts]; /**< Array of offsets to the OffsetTable for each font from the beginning of the file */
  });

  char const *tagTTCF = "ttcf";

  /**
    * @brief TrueType TTC Header Version 2.0 (DSIG part)
    */
  PACK(struct TrueTypeTTCv20DSIG{
    uint32_t  mUlDsigTag;     /**< Tag indicating that a DSIG table exists, 0x44534947 ('DSIG') (null if no signature) */
    uint32_t  mUlDsigLength;  /**< The length (in bytes) of the DSIG table (null if no signature) */
    uint32_t  mUlDsigOffset;  /**< The offset (in bytes) of the DSIG table from the beginning of the TTC file (null if no signature) */
  });
}

namespace kernel{ namespace font{ namespace truetype{

  FontTrueTypeCollection::FontTrueTypeCollection(pdfout::InputStream *stream)
    : mStream(stream),
    mOffsetTable(){

    stream->seekg(0);
    TrueTypeTTCv10 header = {0};
    if (stream->read(&header, sizeof(TrueTypeTTCv10), 1) != sizeof(TrueTypeTTCv10))
      RUNTIME_EXCEPTION("Invalid TrueType TTC header");

    Endian const endian = getEndian();
    if (endian == EndianLittle){
      swapByteOrder(header.mMajorVersion);
      swapByteOrder(header.mMinorVersion);
      swapByteOrder(header.mNumFonts);
    }

    if (!memcmp(header.mTag, tagTTCF, 4 * sizeof(uint8_t))){
      // TTC font
      mOffsetTable.resize(header.mNumFonts);
      stream->read(mOffsetTable.data(), sizeof(uint32_t), header.mNumFonts);

      if (endian == EndianLittle){
        for (auto &elem : mOffsetTable)
          swapByteOrder(elem);
      }
    }
    else{
      // TrueType font
      mOffsetTable.push_back(0);
    }
  }

  FontTrueTypeCollection::~FontTrueTypeCollection(void) {}

  uint32_t FontTrueTypeCollection::getNumFonts(void) const{
    return mOffsetTable.size();
  }

  std::unique_ptr<FontTrueType> FontTrueTypeCollection::createFont(uint32_t index) const{
    if (index >= mOffsetTable.size())
      RUNTIME_EXCEPTION("Invalid TrueType font index in the TrueType Collection");

    mStream->seekg(mOffsetTable[index]);
    return std::make_unique<FontTrueType>(mStream);
  }

}}}
