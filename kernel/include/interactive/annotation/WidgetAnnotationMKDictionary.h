#pragma once

#include <object/ObjectDictionary.h>

namespace kernel{ namespace interactive{

  enum WidgetAnnotationMKDictionaryKey{
    WidgetAnnotationMKDictionaryKeyR,  /**< (Optional) The number of degrees by which the widget annotation is rotated counterclockwise relative to the page. The value must be a multiple of 90. */
    WidgetAnnotationMKDictionaryKeyBC, /**< (Optional) An array of numbers in the range 0.0 to 1.0 specifying the color of the widget annotation's border. The number of array elements determines the color space in which the color is defined. */
    WidgetAnnotationMKDictionaryKeyBG, /**< (Optional) An array of numbers in the range 0.0 to 1.0 specifying the color of the widget annotation's background. The number of array elements determines the color space, as described above for BC. */
    WidgetAnnotationMKDictionaryKeyCA, /**< (Optional; button fields only) The widget annotation's normal caption, displayed when it is not interacting with the user. */
    WidgetAnnotationMKDictionaryKeyRC, /**< (Optional; pushbutton fields only) The widget annotation's rollover caption, displayed when the user rolls the cursor into its active area without pressing the mouse button. */
    WidgetAnnotationMKDictionaryKeyAC, /**< (Optional; pushbutton fields only) The widget annotation's alternate (down) caption, displayed when the mouse button is pressed within its active area. */
    WidgetAnnotationMKDictionaryKeyI,  /**< (Optional; pushbutton fields only; must be an indirect reference) A form XObject defining the widget annotation's normal icon, displayed when it is not interacting with the user. */
    WidgetAnnotationMKDictionaryKeyRI, /**< (Optional; pushbutton fields only; must be an indirect reference) A form XObject defining the widget annotation's rollover icon, displayed when the user rolls the cursor into its active area without pressing the mouse button. */
    WidgetAnnotationMKDictionaryKeyIX, /**< (Optional; pushbutton fields only; must be an indirect reference) A form XObject defining the widget annotation's alternate (down) icon, displayed when the mouse button is pressed within its active area. */
    WidgetAnnotationMKDictionaryKeyIF, /**< (Optional; pushbutton fields only) An icon fit dictionary specifying how to display the widget annotation's icon within its annotation rectangle. If present, the icon fit dictionary applies to all of the annotation's icons (normal, rollover, and alternate). */
    WidgetAnnotationMKDictionaryKeyTP  /**< (Optional; pushbutton fields only) A code indicating where to position the text of the widget annotation's caption relative to its icon. */
  };

  /**
   * @brief A line annotation displays a single straight line on the page.
   */
  class WidgetAnnotationMKDictionary : public ObjectDictionary{
  public:
    WidgetAnnotationMKDictionary(IndirectType indirectable = IndirectTypeIndirectable);
    ~WidgetAnnotationMKDictionary(void);

    void addKey(WidgetAnnotationMKDictionaryKey key, std::unique_ptr<Object> value);

  private:
    static std::unordered_map<WidgetAnnotationMKDictionaryKey, std::pair<std::string , uint32_t>> mWidgetAnnotationMKMap;

  private:
    WidgetAnnotationMKDictionary(WidgetAnnotationMKDictionary const &) = delete;
    WidgetAnnotationMKDictionary &operator=(WidgetAnnotationMKDictionary const &) = delete;
  };

}}
