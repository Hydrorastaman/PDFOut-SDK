#include <font/TrueType/Cmap.h>

namespace kernel{ namespace font{ namespace truetype{

  Cmap::Cmap(Format format, uint16_t platformId, uint16_t encodingId)
    : mFormat(format),
    mLanguage(0),
    mPlatformId(platformId),
    mEncodingId(encodingId){
  }

  Cmap::~Cmap(void) {}
  Format Cmap::getFormat(void) const {return mFormat;}
  uint16_t Cmap::getLanguage(void) const {return mLanguage;}
  uint16_t Cmap::getPlatformId(void) const {return mPlatformId;}
  uint16_t Cmap::getEncodingId(void) const {return mEncodingId;}

}}}
