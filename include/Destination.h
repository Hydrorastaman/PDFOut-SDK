#pragma once
#ifdef __NOT_IMPLEMENTED___

#include <string>
#include <memory>

class Page;

namespace kernel{
  class ObjectArray;
}

enum DestinationFit{
  DestinationFitXYZ,  /**< [left top zoom] Display the page designated by page, with the coordinates (left, top) positioned at the upper-left corner of the window and the contents of the page magnified by the factor zoom. A null value for any of the parameters left, top, or zoom specifies that the current value of that parameter is to be retained unchanged. A zoom value of 0 has the same meaning as a null value. */
  DestinationFitFull, /**< [] Display the page designated by page, with its contents magnified just enough to fit the entire page within the window both horizontally and vertically. If the required horizontal and vertical magnification factors are different, use the smaller of the two, centering the page within the window in the other dimension. */
  DestinationFitH,    /**< [top] Display the page designated by page, with the vertical coordinate top positioned at the top edge of the window and the contents of the page magnified just enough to fit the entire width of the page within the window. A null value for top specifies that the current value of that parameter is to be retained unchanged. */
  DestinationFitV,    /**< [left] Display the page designated by page, with the horizontal coordinate left positioned at the left edge of the window and the contents of the page magnified just enough to fit the entire height of the page within the window. A null value for left specifies that the current value of that parameter is to be retained unchanged. */
  DestinationFitR,    /**< [left bottom right top] Display the page designated by page, with its contents magnified just enough to fit the rectangle specified by the coordinates left, bottom, right, and topentirely within the window both horizontally and vertically. If the required horizontal and vertical magnification factors are different, use the smaller of the two, centering the rectangle within the window in the other dimension. A null value for any of the parameters may result in unpredictable behavior. */
  DestinationFitB,    /**< [] Display the page designated by page, with its contents magnified just enough to fit its bounding box entirely within the window both horizontally and vertically. If the required horizontal and vertical magnification factors are different, use the smaller of the two, centering the bounding box within the window in the other dimension. */
  DestinationFitBH,   /**< [top] Display the page designated by page, with the vertical coordinate top positioned at the top edge of the window and the contents of the page magnified just enough to fit the entire width of its bounding box within the window. A null value for top specifies that the current value of that parameter is to be retained unchanged. */
  DestinationFitBV    /**< [left] Display the page designated by page, with the horizontal coordinate left positioned at the left edge of the window and the contents of the page magnified just enough to fit the entire height of its bounding box within the window. A null value for left specifies that the current value of that parameter is to be retained unchanged. */
};

/**
 * @brief Defines a particular view of a document. It will always refer to a specific
 * page of the PDF, and may optionally include a smaller subsection of the page as well as
 * a magnification (zoom) factor.
 */
class Destination{
private:
  Destination(void);

public:
  ~Destination(void);    

  static std::unique_ptr<kernel::ObjectArray> createExplicitDestination(Page const *page, DestinationFit fit, int32_t param0 = 0, int32_t param1 = 0, int32_t param2 = 0, int32_t param3 = 0);

private:
  Destination(Destination const &) = delete;
  Destination &operator=(Destination const &) = delete;
};
#endif
