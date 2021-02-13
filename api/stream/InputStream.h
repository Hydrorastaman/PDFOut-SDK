#pragma once

#include "../PdfOutConfig.h"

namespace pdfout{

  class PDFOUT_EXPORTS InputStream{
  protected:
    InputStream(void) {}

  public:
    virtual ~InputStream(void) {}

    /**
     * Gets position in input sequence
     * @return The current position in the stream
     */
    virtual uint64_t tellg(void) const = 0;

    /**
     * Sets position in input sequence
     * @param pos [in] New absolute position within the stream
     */
    virtual bool seekg(uint64_t pos) = 0; 

    /**
     * Read block of data from stream
     * @param data [in] Pointer to a block of memory with a size of at least (size*count) bytes
     * @param sizeOfElements [in] Size, in bytes, of each element to be read
     * @param numOfElements [in] Number of elements, each one with a size of size bytes
     * @return The total number of elements successfully read
     */
    virtual uint64_t read(void *data, uint32_t sizeOfElements, uint64_t numOfElements) const = 0;

  private:
    InputStream(InputStream const &);
    InputStream &operator=(InputStream const &);
  };

}
