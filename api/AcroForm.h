#pragma once

#include "PdfOutConfig.h"

namespace pdfout{

  enum AcroFormSignatureFlag{
    AcroFormSignatureFlagSignaturesExist = 0x1, /**< If set, the document contains at least one signature field. This flag allows a viewer application to enable user interface items (such as menu items or pushbuttons) related to signature processing without having to scan the entire document for the presence of signature fields. */
    AcroFormSignatureFlagsAppendOnly = 0x2      /**< If set, the document contains signatures that may be invalidated if the file is saved (written) in a way that alters its previous contents, as opposed to an incremental update. Merely updating the file by appending new information to the end of the previous version is safe (see Section G.6, “Updating Example”). Viewer applications can use this flag to present a user requesting a full save with an additional alert box warning that signatures will be invalidated and requiring explicit confirmation before continuing with the operation. */
  };

  enum AcroFormFieldFlags{
    AcroFormFieldFlagsReadOnly = 0x00000001, /**< 1 bit, If set, the user may not change the value of the field. Any associated widget annotations will not interact with the user; that is, they will not respond to mouse clicks or change their appearance in response to mouse motions. This flag is useful for fields whose values are computed or imported from a database. */
    AcroFormFieldFlagsRequired = 0x00000002, /**< 2 bit, If set, the field must have a value at the time it is exported by a submit-form action. */
    AcroFormFieldFlagsNoExport = 0x00000004, /**< 3 bit, If set, the field must not be exported by a submit-form action. */
  };

  enum AcroFormButtonFlags{
    AcroFormButtonFlagsNoToggleToOff    = 0x00004000, /**< 15 bit, (Radio buttons only) If set, exactly one radio button must be selected at all times; clicking the currently selected button has no effect. If clear, clicking the selected button deselects it, leaving no button selected. */
    AcroFormButtonFlagsRadio            = 0x00008000, /**< 16 bit, If set, the field is a set of radio buttons; if clear, the field is a check box. This flag is meaningful only if the Pushbutton flag is clear.*/
    AcroFormButtonFlagsPushbutton       = 0x00010000, /**< 17 bit, If set, the field is a pushbutton that does not retain a permanent value. */
    AcroFormButtonFlagsRadiosInUnison   = 0x02000000  /**< 26 bit, (PDF 1.5) If set, a group of radio buttons within a radio button field that use the same value for the on state will turn on and off in unison; that is if one is checked, they are all checked. If clear, the buttons are mutually exclusive (the same behavior as HTML radio buttons). */
  };

  enum AcroFormTextFlags{
    AcroFormTextFlagsMultiline       = 0x00001000, /**< 13 bit, If set, the field can contain multiple lines of text; if clear, the field's text is restricted to a single line. */
    AcroFormTextFlagsPassword        = 0x00002000, /**< 14 bit, If set, the field is intended for entering a secure password that should not be echoed visibly to the screen. Characters typed from the keyboard should instead be echoed in some unreadable form, such as asterisks or bullet characters. To protect password confidentiality, viewer applications should never store the value of the text field in the PDF file if this flag is set. */
    AcroFormTextFlagsFileSelect      = 0x00100000, /**< 21 bit, (PDF 1.4) If set, the text entered in the field represents the pathname of a file whose contents are to be submitted as the value of the field. */
    AcroFormTextFlagsDoNotSpellCheck = 0x00400000, /**< 23 bit, (PDF 1.4) If set, text entered in the field is not spell-checked. */
    AcroFormTextFlagsDoNotScroll     = 0x00800000, /**< 24 bit, (PDF 1.4) If set, the field does not scroll (horizontally for single-line fields, vertically for multiple-line fields) to accommodate more text than fits within its annotation rectangle. Once the field is full, no further text is accepted. */
    AcroFormTextFlagsComb            = 0x01000000, /**< 25 bit, (PDF 1.5) Meaningful only if the MaxLen entry is present in the text field dictionary (see Table 8.78) and if the Multiline, Password, and FileSelect flags are clear. If set, the field is automatically divided into as many equally spaced positions, or combs, as the value of MaxLen, and the text is laid out into those combs. */
    AcroFormTextFlagsRichText        = 0x02000000, /**< 26 bit, (PDF 1.5) If set, the value of this field should be represented as a rich text string (see “Rich Text Strings” on page 680). If the field has a value, the RVentry of the field dictionary (Table 8.71) specifies the rich text string. */
  };

  enum AcroFormChoiceFlags{
    AcroFormChoiceFlagsCombo             = 0x0020000, /**< 18 bit, If set, the field is a combo box; if clear, the field is a list box. */
    AcroFormChoiceFlagsEdit              = 0x0040000, /**< 19 bit, If set, the combo box includes an editable text box as well as a drop-down list; if clear, it includes only a drop-down list. This flag is meaningful only if the Combo flag is set. */
    AcroFormChoiceFlagsSort              = 0x0080000, /**< 20 bit, If set, the field's option items should be sorted alphabetically. This flag is intended for use by form authoring tools, not by PDF viewer applications. Viewers should simply display the options in the order in which they occur in the Opt array (see Table 8.80). */
    AcroFormChoiceFlagsMultiSelect       = 0x0200000, /**< 22 bit, (PDF 1.4) If set, more than one of the field's option items may be selected simultaneously; if clear, no more than one item at a time may be selected. */
    AcroFormChoiceFlagsDoNotSpellCheck   = 0x0400000, /**< 23 bit, (PDF 1.4) If set, text entered in the field is not spell-checked. This flag is meaningful only if the Combo and Edit flags are both set. */
    AcroFormChoiceFlagsCommitOnSelChange = 0x4000000, /**< 27 bit, (PDF 1.5) If set, the new value is committed as soon as a selection is made with the pointing device. This option enables applications to perform an action once a selection is made, without requiring the user to exit the field. If clear, the new value is not committed until the user exits the field */
  };

  /**
   * @brief Signature action that indicates the set of fields that should be locked.
   */
  enum AcroFormSignatureAction{
    AcroFormSignatureActionAll,     /**< All fields in the document. */
    AcroFormSignatureActionInclude, /**< All fields specified in Fields. */
    AcroFormSignatureActionExclude  /**< All fields except those specified in Fields. */
  };

  /**
   * @brief A digest algorithms to use while signing.
   */
  enum AcroFormDigestMethod{
    AcroFormDigestMethodSHA1,
    AcroFormDigestMethodSHA256,
    AcroFormDigestMethodSHA384,
    AcroFormDigestMethodSHA512,
    AcroFormDigestMethodRIPEMD160
  };

  enum AcroFormSignatureSVFlags{
    AcroFormSignatureSVFlagsFilter           = 0x1,  /**<  */
    AcroFormSignatureSVFlagsSubFilter        = 0x2,  /**<  */
    AcroFormSignatureSVFlagsV                = 0x4,  /**<  */
    AcroFormSignatureSVFlagsReasons          = 0x8,  /**<  */
    AcroFormSignatureSVFlagsLegalAttestation = 0x10, /**<  */
    AcroFormSignatureSVFlagsAddRevInfo       = 0x20, /**<  */
    AcroFormSignatureSVFlagsDigestMethod     = 0x40  /**<  */
  };

  class PDFOUT_EXPORTS AcroForm{
  protected:
    AcroForm(void) {}

  public:
    virtual ~AcroForm(void) {}

  private:
    AcroForm(AcroForm const &);
    AcroForm &operator=(AcroForm const &);
  };

}
