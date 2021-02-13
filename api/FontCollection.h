#pragma once

#include "PdfOutConfig.h"

namespace pdfout{

  class PDFOUT_EXPORTS Font;

  class PDFOUT_EXPORTS FontCollection{
  protected:
    FontCollection(void) {}

  public:
    virtual ~FontCollection(void) {}

    virtual std::size_t getCount(void) const = 0;
    virtual Font *getFont(std::size_t index) const = 0;

  private:
    FontCollection(FontCollection const &);
    FontCollection &operator=(FontCollection const &);
  };

}
