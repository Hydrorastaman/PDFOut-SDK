#pragma once
#ifdef __NOT_IMPLEMENTED___

#include <annotations/MarkupAnnotation.h>

namespace kernel{ namespace interactive{ namespace annotation{
  class TextAnnotationDictionary;
}}}

/**
 * @brief The name of an icon to be used in displaying the annotation.
 */
enum TextAnnotationIcon{
  TextAnnotationIconComment,
  TextAnnotationIconKey,
  TextAnnotationIconNote,
  TextAnnotationIconHelp,
  TextAnnotationIconNewParagraph,
  TextAnnotationIconParagraph,
  TextAnnotationIconInsert,
  TextAnnotationIconNone
};

/**
 * @brief The author-specific state associated with text annotation.
 */
enum TextAnnotationState{
  TextAnnotationStateMarked,    /**< The annotation has been marked by the user. */
  TextAnnotationStateUnmarked,  /**< The annotation has not been marked by the user (the default). */
  TextAnnotationStateAccepted,  /**< The user agrees with the change. */
  TextAnnotationStateRejected,  /**< The user disagrees with the change. */
  TextAnnotationStateCancelled, /**< The change has been cancelled. */
  TextAnnotationStateCompleted, /**< The change has been completed. */
  TextAnnotationStateNone       /**< The user has indicated nothing about the change (the default). */
};

/**
 * @brief A text annotation represents a “sticky note” attached to a point in the PDF document. 
 * When closed, the annotation appears as an icon; when open, it displays a pop-up window containing the text of the note in a font and size chosen by the viewer application.
 */
class TextAnnotation : public MarkupAnnotation{
  friend class Page;

protected:
  TextAnnotation(kernel::ResourceManager *resourceManager, struct Rectangle const &rect, pdfout::UString const &name);

public:
  ~TextAnnotation(void);

  std::unique_ptr<kernel::ObjectIndirectReference> serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

  /**
   * Specifying whether the annotation should initially be displayed open.
   */
  void setInitialState(bool isOpen);

  /**
   * The name of an icon to be used in displaying the annotation.
   */
  void setIconName(TextAnnotationIcon iconName);

  /**
   * The state to which the original annotation should be set.
   * @param state [in] The annotation state, one of the ::TextAnnotationState value.
   */
  void setState(TextAnnotationState state);

private:
  TextAnnotation(TextAnnotation const &) = delete;
  TextAnnotation &operator=(TextAnnotation const &) = delete;

private:
  bool mInitialState;
  TextAnnotationIcon mIconName;
  TextAnnotationState mState;
};
#endif
