#include <font/TrueType/TableName.h>

#include <memory>
#include <algorithm>

#include <Types.h>
#include <InternalTools.h>

using namespace pdfout;

namespace{

  /**
   * @brief Header of the names table
   */
  PACK(struct TrueTypeNamingTable{
    uint16_t mFormat;        /**< Format selector, 0 or 1 */
    uint16_t mCount;         /**< Number of name records */
    uint16_t mStringOffset;  /**< Offset to start of string storage (from start of table) */
  });

  /**
   * @brief Records in the names table
   */
  PACK(struct TrueTypeNameRecord{
    uint16_t mPlatformID;   /**< Platform ID */
    uint16_t mEncodingID;   /**< Platform-specific encoding ID */
    uint16_t mLanguageID;   /**< Language ID */
    uint16_t mNameID;       /**< Name ID */
    uint16_t mStringLength; /**< String length (in bytes) */
    uint16_t mStringOffset; /**< String offset from start of storage area (in bytes) */
  });

  /**
   * @brief Language tag record for names table
   */
  PACK(struct LangTagRecord{
    uint16_t mLength; /**< Language-tag string length (in bytes) */
    uint16_t mOffset; /**< Language-tag string offset from start of storage area (in bytes). */
  });

  enum TableNameFormat{
    TableNameFormat0 = 0,
    TableNameFormat1 = 1
  };

}

namespace kernel{ namespace font{ namespace truetype{

  TableName::TableName(void)
    : Table(){
  }

  TableName::~TableName(void){
  }

  void TableName::load(pdfout::InputStream *stream){
    Endian const endian = getEndian();
    uint64_t startOfTable = stream->tellg();

    TrueTypeNamingTable namingTable = {0};
    stream->read(&namingTable, sizeof(TrueTypeNamingTable), 1);
    if (endian == EndianLittle){
      swapByteOrder(namingTable.mFormat);
      swapByteOrder(namingTable.mCount);
      swapByteOrder(namingTable.mStringOffset);
    }

    std::vector<TrueTypeNameRecord> records;

    TrueTypeNameRecord nameRecord = {0};
    for (auto i = 0; i < namingTable.mCount; ++i){
      stream->read(&nameRecord, sizeof(TrueTypeNameRecord), 1);
      if (endian == EndianLittle){
        swapByteOrder(nameRecord.mPlatformID);
        swapByteOrder(nameRecord.mEncodingID);
        swapByteOrder(nameRecord.mLanguageID);
        swapByteOrder(nameRecord.mNameID);
        swapByteOrder(nameRecord.mStringLength);
        swapByteOrder(nameRecord.mStringOffset);
      }

      records.push_back(nameRecord);
    }

    std::vector<LangTagRecord> langTags;
    if (namingTable.mFormat == TableNameFormat1){
      uint16_t langTagCount = 0;
      stream->read(&langTagCount, sizeof(uint16_t), 1);
      if (endian == EndianLittle)
        swapByteOrder(langTagCount);

      langTags.resize(langTagCount);
      stream->read(langTags.data(), sizeof(LangTagRecord), langTagCount);

      if (endian == EndianLittle){
        for (auto &elem : langTags){
          swapByteOrder(elem.mLength);
          swapByteOrder(elem.mOffset);
        }
      }
    }

    mFormat = namingTable.mFormat;

    uint64_t offset = 0;
    NameRecord record = {0};
    std::vector<uint8_t> data(2048);
    for (auto const &elem : records){
      offset = startOfTable + namingTable.mStringOffset + elem.mStringOffset;

      if (data.size() < elem.mStringLength)
        data.resize(elem.mStringLength);

      stream->seekg(offset);
      stream->read(data.data(), sizeof(uint8_t), elem.mStringLength);

      record.mEncodingID = elem.mEncodingID;
      record.mLanguageID = elem.mLanguageID;
      record.mNameID = elem.mNameID;
      record.mPlatformID = elem.mPlatformID;
      record.mName.assign(data.data(), data.data() + elem.mStringLength);
      mNameRecord.push_back(record);
    }

    for (auto const &elem : langTags){
      offset = startOfTable + elem.mOffset;

      if (data.size() < elem.mLength)
        data.resize(elem.mLength);

      stream->seekg(offset);
      stream->read(data.data(), sizeof(uint8_t), elem.mLength);

      mLangTagRecord.push_back(data);
    }
  }

  void TableName::save(pdfout::OutputStream *stream) const{
    Endian const endian = getEndian();

    TrueTypeNamingTable namingTable = {0};
    namingTable.mCount = mNameRecord.size();
    namingTable.mFormat = mFormat;
    namingTable.mStringOffset = sizeof(TrueTypeNamingTable) + mNameRecord.size() * sizeof(TrueTypeNameRecord) + mLangTagRecord.size() * sizeof(LangTagRecord);
    if (endian == EndianLittle){
      swapByteOrder(namingTable.mFormat);
      swapByteOrder(namingTable.mCount);
      swapByteOrder(namingTable.mStringOffset);
    }
    stream->write(&namingTable, sizeof(TrueTypeNamingTable), 1);

    uint16_t recordOffset = 0;
    TrueTypeNameRecord nameRecord = {0};
    for (auto const &elem : mNameRecord){
      nameRecord.mEncodingID = elem.mEncodingID;
      nameRecord.mLanguageID = elem.mLanguageID;
      nameRecord.mNameID = elem.mNameID;
      nameRecord.mPlatformID = elem.mPlatformID;
      nameRecord.mStringLength = elem.mName.size();
      nameRecord.mStringOffset = recordOffset;
      recordOffset += nameRecord.mStringLength;

      if (endian == EndianLittle){
        swapByteOrder(nameRecord.mPlatformID);
        swapByteOrder(nameRecord.mEncodingID);
        swapByteOrder(nameRecord.mLanguageID);
        swapByteOrder(nameRecord.mNameID);
        swapByteOrder(nameRecord.mStringLength);
        swapByteOrder(nameRecord.mStringOffset);
      }

      stream->write(&nameRecord, sizeof(TrueTypeNameRecord), 1);
    }

    if (mFormat == TableNameFormat1){
      uint16_t langTagCount = mLangTagRecord.size();
      if (endian == EndianLittle)
        swapByteOrder(langTagCount);

      stream->write(&langTagCount, sizeof(uint16_t), 1);

      LangTagRecord langRecord = {0};
      for (auto const &elem : mLangTagRecord){
        langRecord.mLength = elem.size();
        langRecord.mOffset = recordOffset;
        recordOffset += langRecord.mLength;

        if (endian == EndianLittle){
          swapByteOrder(langRecord.mLength);
          swapByteOrder(langRecord.mOffset);
        }

        stream->write(&langRecord, sizeof(LangTagRecord), 1);
      }
    }

    for (auto const &elem : mNameRecord)
      stream->write(elem.mName.data(), sizeof(uint8_t), elem.mName.size());

    for (auto const &elem : mLangTagRecord)
      stream->write(elem.data(), sizeof(uint8_t), elem.size());
  }

  NameRecord const *TableName::getNameRecord(NameId nameId) const{
    auto iter = std::find_if(std::begin(mNameRecord), std::end(mNameRecord), [nameId](NameRecord elem)->bool {return elem.mNameID == (uint16_t) nameId;});
    if (iter != std::end(mNameRecord))
      return &(*iter);

    return nullptr;
  }

  pdfout::Encoding TableName::getEncoding(uint16_t platformId, uint16_t encodingId) const{
    switch (platformId){
      case PlatformIdUnicode:
        return Encoding_UNICODE_1_1;

      case PlatformIdMacintosh:
        if (encodingId == MacintoshEncodingRoman)
          return Encoding_ROMAN8;
        break;

      case PlatformIdWindows:
        switch (encodingId){
          case MicrosoftEncodingSymbol:
            return Encoding_UTF16BE;

          case MicrosoftEncodingUSC2:
            return Encoding_UCS2BE;

          case MicrosoftEncodingUSC4:
            return Encoding_UCS4BE;
        }
        break;
    }

    return Encoding_UNKNOWN;
  }

  pdfout::UString TableName::getPostScriptName(void) const{
    return getName(NameIdPostScriptName);
  }

  pdfout::UString TableName::getFamilyName(void) const{
    return getName(NameIdFontFamilyName);
  }

  pdfout::UString TableName::getSubFamilyName(void) const{
    return getName(NameIdFontSubfamilyName);
  }

  std::unique_ptr<TableName> TableName::createSubset(void) const{
    std::unique_ptr<TableName> name = std::make_unique<TableName>();
    //return name;

    name->mFormat = TableNameFormat0;
    UString familyName = getFamilyName().encode(Encoding_CHAR);
    NameRecord family = {PlatformIdMacintosh, MacintoshEncodingRoman, 0, NameIdFontFamilyName, std::vector<uint8_t>(familyName.getData(), familyName.getData() + familyName.getSize())};
    name->mNameRecord.push_back(family);

    UString subfamilyName = getSubFamilyName().encode(Encoding_CHAR);
    NameRecord subfamily = {PlatformIdMacintosh, MacintoshEncodingRoman, 0, NameIdFontSubfamilyName, std::vector<uint8_t>(subfamilyName.getData(), subfamilyName.getData() + subfamilyName.getSize())};
    name->mNameRecord.push_back(subfamily);

    UString uniqueIdName = UString("This is an unique ID", Encoding_CHAR);
    NameRecord uniqueId = {PlatformIdMacintosh, MacintoshEncodingRoman, 0, NameIdUniqueFontId, std::vector<uint8_t>(uniqueIdName.getData(), uniqueIdName.getData() + uniqueIdName.getSize())};
    name->mNameRecord.push_back(uniqueId);

    UString postscriptName = getPostScriptName().encode(Encoding_CHAR);
    if (!postscriptName.getSize())
      postscriptName = getFamilyName().encode(Encoding_CHAR);

    NameRecord postscript = {PlatformIdMacintosh, MacintoshEncodingRoman, 0, NameIdPostScriptName, std::vector<uint8_t>(postscriptName.getData(), postscriptName.getData() + postscriptName.getSize())};
    name->mNameRecord.push_back(postscript);

    return name;
  }

  pdfout::UString TableName::getName(NameId nameId) const{
    pdfout::Encoding encoding = Encoding_UNKNOWN;
    for (auto const &elem : mNameRecord){
      if (elem.mNameID == nameId){
        encoding = getEncoding(elem.mPlatformID, elem.mEncodingID);
        if (encoding != Encoding_UNKNOWN)
          return UString(elem.mName.data(), elem.mName.size(), encoding).encode(Encoding_UTF8);
      }
    }

    return UString(nullptr, 0, Encoding_UTF8);
  }

}}}
