#include <font/TrueType/TablePost.h>

#include <Exception.h>
#include <font/ToUnicode.h>
#include <font/MacintoshGlyphList.h>

namespace kernel{ namespace font{ namespace truetype{

  TablePost::TablePost(void)
    : Table(){
  }

  TablePost::~TablePost(void){
  }

  void TablePost::load(pdfout::InputStream *stream){
    Endian const endian = getEndian();
    stream->read(&mData, 32, 1);
    if (getEndian() == EndianLittle){
      swapByteOrder(mData.mVersion);
      swapByteOrder(mData.mItalicAngle);
      swapByteOrder(mData.mUnderlinePosition);
      swapByteOrder(mData.mUnderlineThickness);
      swapByteOrder(mData.mIsFixedPitch);
      swapByteOrder(mData.mMinMemType42);
      swapByteOrder(mData.mMaxMemType42);
      swapByteOrder(mData.mMinMemType1);
      swapByteOrder(mData.mMaxMemType1);
    }

    if (mData.mVersion == TableVersion20){
      stream->read(&mData.mNumberOfGlyphs, sizeof(uint16_t), 1);
      if (endian == EndianLittle)
        swapByteOrder(mData.mNumberOfGlyphs);

      mData.mGlyphNameIndex.resize(mData.mNumberOfGlyphs);
      mData.mNames.resize(mData.mNumberOfGlyphs);

      stream->read(mData.mGlyphNameIndex.data(), sizeof(uint16_t), mData.mNumberOfGlyphs);
      if (endian == EndianLittle){
        for (auto &elem : mData.mGlyphNameIndex)
          swapByteOrder(elem);
      }

      char name[256] = {0};
      uint8_t len = 0;
      for (int i = 0; i < mData.mNumberOfGlyphs; ++i){
        stream->read(&len, sizeof(uint8_t), 1);
        stream->read(name, sizeof(uint8_t), len);
        mData.mNames[i].assign(name, name + len);
      }
    }
    else if (mData.mVersion == TableVersion25){
      stream->read(&mData.mNumberOfGlyphs, sizeof(uint16_t), 1);
      swapByteOrder(mData.mNumberOfGlyphs);

      mData.mOffset.resize(mData.mNumberOfGlyphs);
      stream->read(mData.mOffset.data(), sizeof(int8_t), mData.mNumberOfGlyphs);
    }
  }

  void TablePost::save(pdfout::OutputStream *stream) const{
    TablePostData data;
    std::size_t const size = (mData.mVersion == TableVersion20 || mData.mVersion == TableVersion25) ? 34 : 32;
    memcpy(&data, &mData, size);
    Endian const endian = getEndian();

    if (endian == EndianLittle){
      swapByteOrder(data.mVersion);
      swapByteOrder(data.mItalicAngle);
      swapByteOrder(data.mUnderlinePosition);
      swapByteOrder(data.mUnderlineThickness);
      swapByteOrder(data.mIsFixedPitch);
      swapByteOrder(data.mMinMemType42);
      swapByteOrder(data.mMaxMemType42);
      swapByteOrder(data.mMinMemType1);
      swapByteOrder(data.mMaxMemType1);
      swapByteOrder(data.mNumberOfGlyphs);
    }

    stream->write(&data, size, 1);

    if (mData.mVersion == TableVersion20){
      if (endian == EndianLittle){
        uint16_t glyph = 0;
        for (auto const &elem : mData.mGlyphNameIndex){
          glyph = elem;
          swapByteOrder(glyph);
          stream->write(&glyph, sizeof(uint16_t), 1);
        }
      }
      else{
        stream->write(mData.mGlyphNameIndex.data(), sizeof(uint16_t), mData.mNumberOfGlyphs);
      }

      char name[256] = {0};
      uint8_t len = 0;
      for (int i = 0; i < mData.mNumberOfGlyphs; ++i){
        len = mData.mNames[i].length();
        name[0] = len;
        memcpy(&name[1], mData.mNames[i].c_str(), mData.mNames[i].length());
        stream->write(name, sizeof(uint8_t), len + 1);
      }
    }
    else if (mData.mVersion == TableVersion25){
      stream->write(mData.mOffset.data(), sizeof(int8_t), mData.mNumberOfGlyphs);
    }
  }

  int32_t TablePost::getItalicAngle(void) const {return mData.mItalicAngle;}
  bool TablePost::isFixedPitch(void) const {return mData.mIsFixedPitch != 0;}

  void TablePost::getUnicode(uint16_t glyphId, UnicodeInfo &uniInfo) const{
     memset(&uniInfo, 0, sizeof(UnicodeInfo));

     switch (mData.mVersion){
      case TableVersion10:{
        if (glyphId < 258) // use standard Macintosh order
          getUnicodeInMacintoshOrder(glyphId, uniInfo);
        break;
      }

      case TableVersion20:{
        if (glyphId >= mData.mNumberOfGlyphs)
          RUNTIME_EXCEPTION("glyphId is out of range");

        uint16_t const glyphNameId = mData.mGlyphNameIndex[glyphId];
        if (glyphNameId < 258){ // use standard Macintosh order
          getUnicodeInMacintoshOrder(glyphId, uniInfo);
        }
        else if (glyphNameId < 32768){ // use POST table
          std::vector<uint32_t> values;
          toUnicode(glyphId, mData.mNames[glyphNameId - 258], values);
        }
        break;
      }

      case TableVersion25:
        RUNTIME_EXCEPTION("Unsupported POST table v2.5"); 
        break;

      case TableVersion30:
        if (glyphId < 258) // use standard Macintosh order
          getUnicodeInMacintoshOrder(glyphId - 1, uniInfo);
        break;

      case TableVersion40:
        RUNTIME_EXCEPTION("Unsupported POST table v4.0"); 
        break;

      default:
        RUNTIME_EXCEPTION("Invalid value for POST table version");
    }
  }

  std::unique_ptr<TablePost> TablePost::createSubset(std::vector<uint16_t> const &gids) const{
    std::unique_ptr<TablePost> post = std::make_unique<TablePost>();

    memcpy(&post->mData, &mData, 32);
    post->mData.mNumberOfGlyphs = gids.size();
    post->mData.mVersion = TableVersion30;

    //switch (mData.mVersion){
    //  case TableVersion20:{
    //    post->mData.mGlyphNameIndex.reserve(gids.size());
    //    post->mData.mNames.reserve(gids.size());

    //    for (std::size_t i = 0; i < gids.size(); ++i){
    //      uint16_t const gid = gids[i];
    //      uint16_t const nameId = mData.mGlyphNameIndex[gid];
    //      if (nameId < 258){
    //        post->mData.mGlyphNameIndex.push_back(i);
    //        post->mData.mNames.push_back(getGlyphNameInMacintoshOrder(gid));
    //      }
    //      else if (nameId < 32768){
    //        post->mData.mGlyphNameIndex.push_back(i);
    //        post->mData.mNames.push_back(mData.mNames[gid - 258]);
    //      }
    //    }

    //    break;
    //  }

    //  case TableVersion25:
    //    break;
    //}

    return post;
  }


  }}}
