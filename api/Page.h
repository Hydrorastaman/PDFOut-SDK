#pragma once

#include "Types.h"
#include "XMPDocument.h"
#include "PdfOutConfig.h"
#include "ContentStream.h"
//#include <controls/Control.h>
//#include <annotations/TextAnnotation.h>
//#include <annotations/LinkAnnotation.h>

namespace pdfout{

  enum PageRotate{
    PageRotate0 = 0,
    PageRotate90 = 90,
    PageRotate180 = 180,
    PageRotate270 = 270
  };

  /**
   * @brief Implementation of PDF page object
   */
  class PDFOUT_EXPORTS Page{
  protected:
    Page(void) {}

  public:
    virtual ~Page(void) {}

    /**
     * @brief A rectangle (see Section 3.8.4, “Rectangles”), expressed in default user space units,
     * defining the boundaries of the physical medium on which the page is intended to be displayed
     * or printed (see Section 10.10.1, “Page Boundaries”).
     */
    virtual void setMediaBox(struct Rectangle const &mediaBox) = 0;
    virtual bool getMediaBox(struct Rectangle &mediaBox) const = 0;

    /**
     * @brief A rectangle, expressed in default user space units, defining the visible region of default
     * user space. When the page is displayed or printed, its contents are to be clipped (cropped)
     * to this rectangle and then imposed on the output medium in some implementation-defined manner
     * (see Section 10.10.1, “Page Boundaries”). Default value: the value of MediaBox.
     */
    virtual void setCropBox(struct Rectangle const &cropBox) = 0;
    virtual bool getCropBox(struct Rectangle &cropBox) const = 0;

    /**
     * @brief A rectangle, expressed in default user space units, defining the region to which the contents
     * of the page should be clipped when output in a production environment
     * (see Section 10.10.1, “Page Boundaries”). Default value: the value of CropBox.
     */
    virtual void setBleedBox(struct Rectangle const &bleedBox) = 0;
    virtual bool getBleedBox(struct Rectangle &bleedBox) const = 0;

    /**
     * @brief A rectangle, expressed in default user space units, defining the intended dimensions of
     * the finished page after trimming (see Section 10.10.1, “Page Boundaries”). Default value: the value of CropBox.
     */
    virtual void setTrimBox(struct Rectangle const &trimBox) = 0;
    virtual bool getTrimBox(struct Rectangle &trimBox) const = 0;

    /**
     * @brief A rectangle, expressed in default user space units, defining the extent of the page's
     * meaningful content (including potential white space) as intended by the page's creator
     * (see Section 10.10.1, “Page Boundaries”). Default value: the value of CropBox.
     */
    virtual void setArtBox(struct Rectangle const &artBox) = 0;
    virtual bool getArtBox(struct Rectangle &artBox) const = 0;

    // BoxColorInfo
    // Contents
    /**
     * @brief A content stream (see Section 3.7.1, “Content Streams”) describing the contents of this page.
     * If this entry is absent, the page is empty.
     */
    virtual ContentStream *getContents(void) const = 0;

    /**
     * @brief The number of degrees by which the page should be rotated clockwise when displayed
     * or printed. The value must be a multiple of 90. Default value: 0.
     */
    virtual void setRotate(PageRotate rotate) = 0;
    virtual PageRotate getRotate(void) const = 0;

    // Group
    // Thumb
    // B

    /**
     * @brief The page's display duration (also called its advance timing): the maximum length of time, in seconds,
     * that the page is displayed during presentations before the viewer application automatically advances
     * to the next page (see Section 8.3.3, “Presentations”). By default, the viewer does not advance automatically.
     */
    virtual void setDur(float duration) = 0;
    virtual bool getDur(float &duration) const = 0;

    // Trans
    // Annots
    // AA
    // Metadata
    virtual void setMetadata(XMPDocument const &xmp) = 0;
    // PieceInfo
    // StructParents
    // ID
    // PZ
    // SeparationInfo
    // Tabs
    // TemplateInstantiated
    // PresSteps

    /**
     * @brief A positive number giving the size of default user space units, in multiples of 1/72 inch.
     * Default value: 1.0 (user unit is 1/72 inch).
     */
    virtual void setUserUnit(float userUnit) = 0;
    virtual bool getUserUnit(float &userUnit) const = 0;

    // VP


    // Controls
    //virtual void addControl(Control *control) = 0;

    // Annotations
    //virtual TextAnnotation *addTextAnnotation(struct Rectangle const &rect, pdfout::UString const &name) = 0;
    //virtual LinkAnnotation *addLinkAnnotation(struct Rectangle const &rect, pdfout::UString const &name) = 0;

  private:
    Page(Page const &);
    Page &operator=(Page const &);
  };
}
