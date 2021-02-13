#include <Tools.h>

namespace pdfout{

  float getWidth(struct Rectangle const &rect){
    return std::fabs(rect.mUpperRightX - rect.mLowerLeftX);
  }

  float getHeight(struct Rectangle const &rect){
    return std::fabs(rect.mUpperRightY - rect.mLowerLeftY);
  }

}
