#pragma once

#include <memory>

#include <Page.h>
#include <PaperSize.h>
#include <ContentStreamImpl.h>
#include <document/DocumentMetadataStream.h>

namespace kernel{
  enum DocumentPageKey;
  class DocumentPage;
  class DocumentPagesDictionary;
  class ResourceManager;
  struct SerializeParams;
  class ObjectIndirectReference;
}

namespace pdfout{ namespace impl{

  /**
   * @brief Implementation of PDF page object
   */
  class PageImpl : public Page{
  public:
    explicit PageImpl(kernel::ResourceManager *resourceManager);
    ~PageImpl(void);

    void serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;
    std::unique_ptr<kernel::ObjectIndirectReference> getReference(void) const;
    void setParent(kernel::DocumentPagesDictionary *parent);

    // Type, set by Page constructor
    // Parent, set by PageTree
    // LastModified
    // Resources, set by constructor

    /**
     * @brief A rectangle (see Section 3.8.4, “Rectangles”), expressed in default user space units,
     * defining the boundaries of the physical medium on which the page is intended to be displayed
     * or printed (see Section 10.10.1, “Page Boundaries”).
     */
    void setMediaBox(struct Rectangle const &mediaBox);
    bool getMediaBox(struct Rectangle &mediaBox) const;

    /**
     * @brief A rectangle, expressed in default user space units, defining the visible region of default
     * user space. When the page is displayed or printed, its contents are to be clipped (cropped)
     * to this rectangle and then imposed on the output medium in some implementation-defined manner
     * (see Section 10.10.1, “Page Boundaries”). Default value: the value of MediaBox.
     */
    void setCropBox(struct Rectangle const &cropBox);
    bool getCropBox(struct Rectangle &cropBox) const;

    /**
     * @brief A rectangle, expressed in default user space units, defining the region to which the contents
     * of the page should be clipped when output in a production environment
     * (see Section 10.10.1, “Page Boundaries”). Default value: the value of CropBox.
     */
    void setBleedBox(struct Rectangle const &bleedBox);
    bool getBleedBox(struct Rectangle &bleedBox) const;

    /**
     * @brief A rectangle, expressed in default user space units, defining the intended dimensions of
     * the finished page after trimming (see Section 10.10.1, “Page Boundaries”). Default value: the value of CropBox.
     */
    void setTrimBox(struct Rectangle const &trimBox);
    bool getTrimBox(struct Rectangle &trimBox) const;

    /**
     * @brief A rectangle, expressed in default user space units, defining the extent of the page's
     * meaningful content (including potential white space) as intended by the page's creator
     * (see Section 10.10.1, “Page Boundaries”). Default value: the value of CropBox.
     */
    void setArtBox(struct Rectangle const &artBox);
    bool getArtBox(struct Rectangle &artBox) const;

    // BoxColorInfo
    // Contents
    /**
     * @brief A content stream (see Section 3.7.1, “Content Streams”) describing the contents of this page.
     * If this entry is absent, the page is empty.
     */
    ContentStream *getContents(void) const;

    /**
     * @brief The number of degrees by which the page should be rotated clockwise when displayed
     * or printed. The value must be a multiple of 90. Default value: 0.
     */
    void setRotate(PageRotate rotate);
    PageRotate getRotate(void) const;

    // Group
    // Thumb
    // B

    /**
     * @brief The page's display duration (also called its advance timing): the maximum length of time, in seconds,
     * that the page is displayed during presentations before the viewer application automatically advances
     * to the next page (see Section 8.3.3, “Presentations”). By default, the viewer does not advance automatically.
     */
    void setDur(float duration);
    bool getDur(float &duration) const;

    // Trans
    // Annots
    // AA
    // Metadata
    void setMetadata(XMPDocument const &xmp);

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
    void setUserUnit(float userUnit);
    bool getUserUnit(float &userUnit) const;

    // VP


    // Controls
    //void addControl(Control *control);

    // Annotations
    //TextAnnotation *addTextAnnotation(struct Rectangle const &rect, pdfout::UString const &name);
    //LinkAnnotation *addLinkAnnotation(struct Rectangle const &rect, pdfout::UString const &name);

  private:
    bool getBox(kernel::DocumentPageKey key, struct Rectangle &box) const;
    bool getNumber(kernel::DocumentPageKey key, float &value) const;
    //Annotation *addAnnotation(std::unique_ptr<Annotation> annot);

  private:
    PageImpl(PageImpl const &) = delete;
    PageImpl &operator=(PageImpl const &) = delete;

  private:
    std::unique_ptr<kernel::DocumentPage> mPage;
    kernel::ResourceManager *mResourceManager;
    std::unique_ptr<ContentStreamImpl> mContentStream;
    std::vector<std::unique_ptr<kernel::ObjectIndirectReference>> mControlsRef;
    //std::vector<std::unique_ptr<Annotation>> mAnnotations;
  };

}}
