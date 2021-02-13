#pragma once

namespace pdfout{

  /**
   * @biref User access permissions
   */
  enum UserAccess{
    UserAccessDefault     = 0xFFFFF0C0, /**< Deny all access */
    UserAccessPrint       = 0x00000004, /**< (Revision 2) Print the document.
                                             (Revision 3 or greater) Print the document (possibly not at the highest quality level, depending on whether ::UserAccessHightPrint is also set). */
    UserAccessEdit        = 0x00000008, /**< Modify the contents of the document by operations other than those controlled by ::UserAccessEditNotes, ::UserAccessFillAndSign, and ::UserAccessDocAssembly. */
    UserAccessCopy        = 0x00000010, /**< (Revision 2) Copy or otherwise extract text and graphics from the document, including extracting text and graphics (in support of accessibility to users with disabilities or for other purposes).
                                             (Revision 3 or greater) Copy or otherwise extract text and graphics from the document by operations other than that controlled by ::UserAccessAccessible. */
    UserAccessEditNotes   = 0x00000020, /**< Add or modify text annotations, fill in interactive form fields, and, if ::UserAccessEdit is also set, create or modify interactive form fields (including signature fields). */
    UserAccessFillAndSign = 0x00000100, /**< (Revision 3 or greater) Fill in existing interactive form fields (including signature fields), even if ::UserAccessEditNotes is clear. */
    UserAccessAccessible  = 0x00000200, /**< (Revision 3 or greater) Extract text and graphics (in support of accessibility to users with disabilities or for other purposes). */
    UserAccessDocAssembly = 0x00000400, /**< (Revision 3 or greater) Assemble the document (insert, rotate, or delete pages and create bookmarks or thumbnail images), even if ::UserAccessEdit is clear. */
    UserAccessHightPrint  = 0x00000800  /**< (Revision 3 or greater) Print the document to a representation from which a faithful digital copy of the PDF content could be generated. When this flag is clear (and ::UserAccessPrint is set), printing is limited to a low-level representation of the appearance, possibly of degraded quality.*/
  };

  /**
   * @brief Document encryption algorithms
   */
  enum EncryptionAlgorithm{
    EncryptionAlgorithmNone,  /**< Disable encryption */
    EncryptionAlgorithmRC4v1, /**< RC4 encryption using a 40-bit key */
    EncryptionAlgorithmRC4v2, /**< RC4 encryption using a 128-bit key */
    EncryptionAlgorithmAESv2, /**< AES encryption using a 128-bit key */
    EncryptionAlgorithmAESv3  /**< AES encryption using a 256-bit key */
  };

}
