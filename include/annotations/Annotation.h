#pragma once
#ifdef __NOT_IMPLEMENTED___

#include <memory>
#include <string>

#include <Types.h>
#include <UString.h>

enum AnnotationFlag{
  AnnotationFlagInvisible      = 0x001, /**< If set, do not display the annotation if it does not belong to one of the standard annotation types and no annotation handler is available. If clear, display such an unknown annotation using an appearance stream specified by its appearance dictionary. */
  AnnotationFlagHidden         = 0x002, /**< (PDF 1.2) If set, do not display or print the annotation or allow it to interact with the user, regardless of its annotation type or whether an annotation handler is available. */
  AnnotationFlagPrint          = 0x004, /**< (PDF 1.3) If set, print the annotation when the page is printed. If clear, never print the annotation, regardless of whether it is displayed on the screen. */
  AnnotationFlagNoZoom         = 0x008, /**< (PDF 1.3) If set, do not scale the annotation's appearance to match the magnification of the page. */
  AnnotationFlagNoRotate       = 0x010, /**< (PDF 1.3) If set, do not rotate the annotation's appearance to match the rotation of the page. */
  AnnotationFlagNoView         = 0x020, /**< (PDF 1.3) If set, do not display the annotation on the screen or allow it to interact with the user. The annotation may be printed (depending on the setting of the Print flag) but should be considered hidden for purposes of on-screen display and user interaction. */
  AnnotationFlagReadOnly       = 0x040, /**< (PDF 1.3) If set, do not allow the annotation to interact with the user. */
  AnnotationFlagLocked         = 0x080, /**< (PDF 1.4) If set, do not allow the annotation to be deleted or its properties (including position and size) to be modified by the user. However, this flag does not restrict changes to the annotation's contents, such as the value of a form field. */
  AnnotationFlagToggleNoView   = 0x100, /**< (PDF 1.5) If set, invert the interpretation of the NoView flag for certain events. */
  AnnotationFlagLockedContents = 0x200  /**< (PDF 1.7) If set, do not allow the contents of the annotation to be modified by the user. This flag does not restrict deletion of the annotation or changes to other annotation properties, such as position and size. */
};

enum AnnotationBorderStyle{
  AnnotationBorderStyleSolid,    /**< A solid rectangle surrounding the annotation. */
  AnnotationBorderStyleDashed,   /**< A dashed rectangle surrounding the annotation. The dash pattern is specified by the D entry (see below). */
  AnnotationBorderStyleBeveled,  /**< A simulated embossed rectangle that appears to be raised above the surface of the page. */
  AnnotationBorderStyleInset,    /**< A simulated engraved rectangle that appears to be recessed below the surface of the page. */
  AnnotationBorderStyleUnderline /**< A single line along the bottom of the annotation rectangle. */
};

enum AnnotationBorderEffect{
  AnnotationBorderEffectS, /**< No effect: the border is as described by the annotation dictionary's BS entry. */
  AnnotationBorderEffectC  /**< The border should appear “cloudy”. The width and dash array specified by BS are honored. */
};

/**
 * @brief The annotation's highlighting mode
 */
enum AnnotationHighlightingMode{
  AnnotationHighlightingModeNone,    /**< No highlighting. */
  AnnotationHighlightingModeInvert,  /**< Invert the contents of the annotation rectangle. */
  AnnotationHighlightingModeOutline, /**< Invert the annotation's border. */
  AnnotationHighlightingModePush     /**< Display the annotation as if it were being pushed below the surface of the page.*/
};

std::string const &annotationHighlightingModeToString(AnnotationHighlightingMode mode);

/**
 * @brief The form of quadding (justification) to be used in displaying the annotation's text.
 */
enum AnnotationJustification{
  AnnotationJustificationLeft = 0,     /**< Left-justified. */
  AnnotationJustificationCentered = 1, /**< Centered. */
  AnnotationJustificationRight = 2     /**< Right-justified. */
};

enum Trigger{
  TriggerE,  /**< An action to be performed when the cursor enters the annotation's active area. */
  TriggerX,  /**< An action to be performed when the cursor exits the annotation's active area. */
  TriggerD,  /**< An action to be performed when the mouse button is pressed inside the annotation's active area. */
  TriggerU,  /**< An action to be performed when the mouse button is released inside the annotation's active area. 
                  Note: For backward compatibility, the A entry in an annotation dictionary, if present, takes precedence over this entry. */
  TriggerPO, /**< An action to be performed when the page containing the annotation is opened (for example, when the user navigates to it from the next or previous page or by means of a link annotation or outline item). The action is executed after the O action in the page's additional-actions dictionary (see Table 8.45) and the OpenAction entry in the document catalog (see Table 3.25), if such actions are present. */
  TriggerPC, /**< An action to be performed when the page containing the annotation is closed (for example, when the user navigates to the next or previous page, or follows a link annotation or outline item). The action is executed before the C action in the page's additional-actions dictionary (see Table 8.45), if present. */
  TriggerPV, /**< An action to be performed when the page containing the annotation becomes visible in the viewer application's user interface. */
  TriggerPI, /**< An action to be performed when the page containing the annotation is no longer visible in the viewer application's user interface. */
  TriggerFo, /**< (Controls only) An action to be performed when the annotation receives the input focus. */
  TriggerBl  /**< (Controls only) An action to be performed when the annotation loses the input focus. */
};

class OutputStream;
class FormXObject;

namespace kernel{
  class ObjectIndirectReference;
  struct SerializeParams;
  class ResourceManager;

  namespace interactive{
    class AnnotationDictionary;
  }
}

class Annotation{
protected:
  Annotation(kernel::ResourceManager *resourceManager, struct Rectangle const &rect, pdfout::UString const &name);

public:
  virtual ~Annotation(void);

  virtual std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const = 0;
  struct Rectangle getRect(void) const;

  /**
   * Sets text to be displayed for the annotation or, if this type of annotation does not display text, 
   * an alternate description of the annotation's contents in human-readable form.
   * @param contents [in] text message
  */
  void setContents(pdfout::UString const &contents);

  /**
   * Specifying various characteristics of the annotation
   * @param flags [in] This parameter can be one or more of the ::AnnotationFlag values
   */
  void setFlags(uint32_t flags);

protected:
  void serialize(kernel::interactive::AnnotationDictionary *annotationDictionary) const;

private:
  Annotation(Annotation const &) = delete;
  Annotation &operator=(Annotation const &) = delete;

protected:
  kernel::ResourceManager *mResourceManager;
  struct Rectangle mRect;
  UString mName;
  UString mContents;
  uint32_t mFlags;
  std::vector<std::unique_ptr<FormXObject>> mFormXObjects;
};
#endif
