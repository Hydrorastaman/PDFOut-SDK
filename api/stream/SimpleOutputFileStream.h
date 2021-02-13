#pragma once

#include <cstdio>

#include "../UString.h"
#include "../PdfOutConfig.h"
#include "OutputStream.h"

namespace pdfout{

  class PDFOUT_EXPORTS SimpleOutputFileStream : public OutputStream{
  public:
    explicit SimpleOutputFileStream(pdfout::UString const &fileName)
      : mFile(nullptr){
      #ifdef  _WIN32
        _wfopen_s(&mFile, (wchar_t const *) fileName.encode(Encoding_WCHAR_T).getData(), L"wb");
      #else
        mFile = fopen((char const *) fileName.data(), "wb");
      #endif
    }

    explicit SimpleOutputFileStream(FILE *file)
      : mFile(file){
    }

    ~SimpleOutputFileStream(void){
      if (mFile)
        fclose(mFile);
    }

    uint64_t tellp(void) const{
      if (mFile)
        return ftell(mFile);

      return 0;
    }

    bool seekp(uint64_t pos){
      bool result = false;
      if (mFile)
        result = fseek(mFile, pos, SEEK_SET) == 0;

      return result;
    }

    uint64_t write(void const *data, uint32_t sizeOfElements, uint64_t numOfElements){
      if (mFile)
        return fwrite(data, sizeOfElements, numOfElements, mFile);

      return 0;
    }

    void flush(void){
      if (mFile)
        fflush(mFile);
    }

  private:
    SimpleOutputFileStream(SimpleOutputFileStream const &);
    SimpleOutputFileStream &operator=(SimpleOutputFileStream const &);

  private:
    FILE *mFile;
  };

}
