#pragma once

#include <cmath>
#include <memory>

#include "Types.h"
#include "PdfOutConfig.h"

namespace pdfout{

  PDFOUT_EXPORTS float getWidth(struct Rectangle const &rect);
  PDFOUT_EXPORTS float getHeight(struct Rectangle const &rect);

}
