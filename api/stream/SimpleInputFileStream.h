#pragma once

#include <string>
#include <stdio.h>

#include "../UString.h"
#include "../Exception.h"
#include "../PdfOutConfig.h"
#include "InputStream.h"

namespace pdfout{

  class PDFOUT_EXPORTS SimpleInputFileStream : public InputStream{
  public:
    explicit SimpleInputFileStream(pdfout::UString const &fileName)
      : mFile(nullptr){
      #ifdef  _WIN32
        UString const tmp = fileName.encode(Encoding_WCHAR_T);
        errno_t errcode = _wfopen_s(&mFile, (wchar_t const *) tmp.getData(), L"rb");
        if (errcode)
          RUNTIME_EXCEPTION("failed to open file");
      #else
        UString const tmp = fileName.encode(Encoding_UTF8);
        mFile = fopen((char const *) tmp.data(), "rb");
        if (!mFile)
          RUNTIME_EXCEPTION("failed to open file");
      #endif
    }

    explicit SimpleInputFileStream(FILE *file)
      : mFile(file){
    }

    ~SimpleInputFileStream(void){
      if (mFile)
        fclose(mFile);
    }

    uint64_t tellg(void) const{
      if (mFile)
        return ftell(mFile);

      return 0;
    }

    bool seekg(uint64_t pos){
      bool result = false;
      if (mFile)
        result = fseek(mFile, pos, SEEK_SET) == 0;

      return result;
    }

    uint64_t read(void *data, uint32_t sizeOfElements, uint64_t numOfElements) const{
      if (mFile)
        return fread(data, sizeOfElements, numOfElements, mFile);

      return 0;
    }

    FILE * getHandle(void) const {return mFile;}

  private:
    SimpleInputFileStream(SimpleInputFileStream const &);
    SimpleInputFileStream &operator=(SimpleInputFileStream const &);

  private:
    FILE *mFile;
  };

}
