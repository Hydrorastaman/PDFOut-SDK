#pragma once

#include "../PdfOutConfig.h"

namespace pdfout{

  class PDFOUT_EXPORTS OutputStream{
  public:
    OutputStream(void) {}
    virtual ~OutputStream(void) {}

    /**
     * Gets position in output sequence
     * @return The current position in the stream
     */
    virtual uint64_t tellp(void) const = 0;

    /**
     * Sets position in output sequence
     * @param [in] New absolute position within the stream
     */
    virtual bool seekp(uint64_t pos) = 0;

    /**
     * Writes block of data
     * @param data [in] Pointer to the array of elements to be written
     * @param sizeOfElements [in] Size in bytes of each element to be written
     * @param numOfElements [in] Number of elements, each one with a size of size bytes
     * @return The total number of elements successfully written
     */
    virtual uint64_t write(void const *data, uint32_t sizeOfElements, uint64_t numOfElements) = 0;

    /**
     * Flushes stream
     */
    virtual void flush(void) = 0;

  private:
    OutputStream(OutputStream const &);
    OutputStream &operator=(OutputStream const &);
  };

}
