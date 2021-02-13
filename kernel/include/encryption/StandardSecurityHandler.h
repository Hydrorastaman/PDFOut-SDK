#pragma once

#include <UString.h>
#include <encryption/SecurityHandler.h>

namespace kernel{ namespace encryption{

  class StandardSecurityHandler : public SecurityHandler{
  public:
    StandardSecurityHandler(uint32_t v, uint32_t userAccess, uint32_t keyLength, std::vector<uint8_t> const &documentId, bool isDocMetadataEncrypted, pdfout::UString const &userPassword, pdfout::UString const &ownerPassword);
    ~StandardSecurityHandler(void);

    std::unique_ptr<ObjectIndirectReference> serialize(pdfout::OutputStream *stream, SerializeParams *params) const;
    std::vector<uint8_t> getEncryptionKey(void) const;

  private:
    uint32_t calculateRevision(uint32_t v, uint32_t userAccess) const;

    /**
     * Computation methods for revision 2, 3 and 4
     */
    void calculateOwnerKeyRev4(std::vector<uint8_t> const &userPassword, std::vector<uint8_t> const &ownerPassword);
    void calculateEncryptionKeyRev4(std::vector<uint8_t> const &password);
    void calculateUserKeyRev2(void);
    void calculateUserKeyRev4(void);
    std::vector<uint8_t> padPassword(uint8_t const *password, std::size_t length) const;
    std::vector<uint8_t> padPassword(std::vector<uint8_t> const &password) const;

    /**
     * Computation methods for revision 5
     */
    void calculateEncryptionKeyRev5(void);
    std::vector<uint8_t> calculateKeyRev5(std::vector<uint8_t> const &password, std::vector<uint8_t> const &validationSalt, std::vector<uint8_t> const &keySalt, std::vector<uint8_t> const &externalData) const; /**< for revision 5 */
    void calculateUserKeyRev5(std::vector<uint8_t> const &userPassword);
    void calculateOwnerKeyRev5(std::vector<uint8_t> const &ownerPassword);
    std::vector<uint8_t> calculateUserEncryptionKey(std::vector<uint8_t> const &password, std::vector<uint8_t> const &keySalt, std::vector<uint8_t> const &externalData) const;
    void calculatePermsValueRev5(void);
    std::vector<uint8_t> truncatePassword(uint8_t const *password, std::size_t length, std::size_t maxLength) const;

  private:
    StandardSecurityHandler(StandardSecurityHandler const &) = delete;
    StandardSecurityHandler &operator=(StandardSecurityHandler const &) = delete;

  protected:
    uint32_t mRevision;
    uint32_t mUserAccess;
    std::vector<uint8_t> mDocumentId;
    bool mIsDocMetadataEncrypted;
    std::vector<uint8_t> mUserKey;  /**< U entry  */
    std::vector<uint8_t> mOwnerKey; /**< O entry */

    std::vector<uint8_t> mUserEncryptionKey;   /**< UE entry, rev 5 */
    std::vector<uint8_t> mOwnerEncryptionKey;  /**< OE entry, rev 5 */
    std::vector<uint8_t> mPermsValue;          /**< Perms entry, rev 5 */

    std::vector<uint8_t> mEncryptionKey;
  };

}}
