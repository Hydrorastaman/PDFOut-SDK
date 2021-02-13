#pragma once

#ifdef __NOT_IMPLEMENTED___

/**
 * @brief A set of flags specifying various characteristics of the Submit-Form action.
 */
enum SubmitFormActionFlags{
  SubmitFormActionFlagsIncludeExclude       = 0x0001, /**< 1  bit, If clear, the Fields array (see Table 8.85) specifies which fields to include in the submission. (All descendants of the specified fields in the field hierarchy are submitted as well.)
                                                                   If set, the Fields array tells which fields to exclude. All fields in the document's interactive form are submitted except those listed in the Fields array and those whose NoExport flag (see Table 8.70 on page 676) is set.*/
  SubmitFormActionFlagsIncludeNoValueFields = 0x0002, /**< 2  bit, If set, all fields designated by the Fields array and the Include/Exclude flag are submitted, regardless of whether they have a value (V entry in the field dictionary). For fields without a value, only the field name is transmitted.
                                                                   If clear, fields without a value are not submitted. */
  SubmitFormActionFlagsExportFormat         = 0x0004, /**< 3  bit, Meaningful only if the SubmitPDF and XFDF flags are clear. If set, field names and values are submitted in HTML Form format.
                                                                   If clear, they are submitted in Forms Data Format (FDF). */
  SubmitFormActionFlagsGetMethod            = 0x0008, /**< 4  bit, If set, field names and values are submitted using an HTTP GET request. 
                                                                   If clear, they are submitted using a POST request. This flag is meaningful only when the ExportFormat flag is set; if ExportFormat is clear, this flag must also be clear. */
  SubmitFormActionFlagsSubmitCoordinates    = 0x0010, /**< 5  bit, If set, the coordinates of the mouse click that caused the submit-form action are transmitted as part of the form data. The coordinate values are relative to the upper-left corner of the field's widget annotation rectangle. This flag is meaningful only when the ExportFormat flag is set. If ExportFormat is clear, this flag must also be clear.*/
  SubmitFormActionFlagsXFDF                 = 0x0020, /**< 6  bit, (PDF 1.4) Meaningful only if the SubmitPDF flags are clear. If set, field names and values are submitted as XFDF. */
  SubmitFormActionFlagsIncludeAppendSaves   = 0x0040, /**< 7  bit, (PDF 1.4) Meaningful only when the form is being submitted in Forms Data Format (that is, when both the XFDF and ExportFormat flags are clear). If set, the submitted FDF file includes the contents of all incremental updates to the underlying PDF document, as contained in the Differences entry in the FDF dictionary (see Table 8.93 on page 714). If clear, the incremental updates are not included.*/
  SubmitFormActionFlagsIncludeAnnotations   = 0x0080, /**< 8  bit, (PDF 1.4) Meaningful only when the form is being submitted in Forms Data Format (that is, when both the XFDF and ExportFormat flags are clear). If set, the submitted FDF file includes all markup annotations in the underlying PDF document (see “Markup Annotations” on page 616). If clear, markup annotations are not included. */
  SubmitFormActionFlagsSubmitPDF            = 0x0100, /**< 9  bit, (PDF 1.4) If set, the document is submitted as PDF, using the MIME content type application / pdf (described in Internet RFC 2045, Multipurpose Internet Mail Extensions (MIME), Part One: Format of Internet Message Bodies; see the Bibliography). If set, all other flags are ignored except GetMethod. */
  SubmitFormActionFlagsCanonicalFormat      = 0x0200, /**< 10 bit, (PDF 1.4) If set, any submitted field values representing dates are converted to the standard format described in Section 3.8.3, “Dates.” (The interpretation of a form field as a date is not specified explicitly in the field itself but only in the JavaScript code that processes it.) */
  SubmitFormActionFlagsExclNonUserAnnots    = 0x0400, /**< 11 bit, (PDF 1.4) Meaningful only when the form is being submitted in Forms Data Format (that is, when both the XFDF and ExportFormat flags are clear) and the IncludeAnnotations flag is set. If set, it includes only those markup annotations whose T entry (see Table 8.21) matches the name of the current user, as determined by the remote server to which the form is being submitted. */
  SubmitFormActionFlagsExclFKey             = 0x0800, /**< 12 bit, (PDF 1.4) Meaningful only when the form is being submitted in Forms Data Format (that is, when both the XFDF and ExportFormat flags are clear). If set, the submitted FDF excludes the F entry. */
  SubmitFormActionFlagsEmbedForm            = 0x2000  /**< 14 bit, (PDF 1.5) Meaningful only when the form is being submitted in Forms Data Format (that is, when both the XFDF and ExportFormat flags are clear). If set, the F entry of the submitted FDF is a file specification containing an embedded file stream representing the PDF file from which the FDF is being submitted. */
};

/**
 * @brief A set of flags specifying various characteristics of the Reset-Form action.
 */
enum ResetFormActionFlags{
  ResetFormActionFlagsIncludeExclude       = 0x0001, /**< If clear, the Fields array (see Table 8.87) specifies which fields to reset. (All descendants of the specified fields in the field hierarchy are reset as well.) If set, the Fields array indicates which fields to exclude from resetting; that is, all fields in the document's interactive form are reset exceptthose listed in the Fields array. */
};

class OutputStream;

namespace kernel{
  struct SerializeParams;
}

class Action{
protected:
  Action(void);

public:
  virtual ~Action(void);

  virtual void serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const = 0;

private:
  Action(Action const &) = delete;
  Action &operator=(Action const &) = delete;
};

class SubmitFormAction : public Action{
public:
  SubmitFormAction(void);
  ~SubmitFormAction(void);

private:
  SubmitFormAction(SubmitFormAction const &) = delete;
  SubmitFormAction &operator=(SubmitFormAction const &) = delete;
};

class GoToAction : public Action{
public:
  GoToAction(void);
  ~GoToAction(void);

private:
  GoToAction(GoToAction const &) = delete;
  GoToAction &operator=(GoToAction const &) = delete;
};

#endif
