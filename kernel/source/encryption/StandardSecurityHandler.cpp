#include <encryption/StandardSecurityHandler.h>

#include <Encryption.h>
#include <InternalTools.h>

#include <object/ObjectInteger.h>
#include <object/ObjectBoolean.h>
#include <object/ObjectNumber.h>
#include <object/ObjectASCIIString.h>
#include <object/ObjectByteString.h>
#include <object/ObjectIndirectReference.h>
#include <encryption/StandardEncryptionDictionary.h>

// CryptoPP
#include <sha.h>
#include <md5.h>
#include <aes.h>
#include <arc4.h>
#include <osrng.h>
#include <modes.h>
#include <cryptlib.h>
#include <randpool.h>

using namespace pdfout;

namespace{
  uint32_t const userAccessRev3 = UserAccessFillAndSign | UserAccessAccessible | UserAccessDocAssembly | UserAccessHightPrint;
  std::vector<uint8_t> const padValue = {0x28, 0xBF, 0x4E, 0x5E, 0x4E, 0x75, 0x8A, 0x41, 0x64, 0x00, 0x4E, 0x56, 0xFF, 0xFA, 0x01, 0x08, 0x2E, 0x2E, 0x00, 0xB6, 0xD0, 0x68, 0x3E, 0x80, 0x2F, 0x0C, 0xA9, 0xFE, 0x64, 0x53, 0x69, 0x7A};
}

namespace kernel{ namespace encryption{

  StandardSecurityHandler::StandardSecurityHandler(uint32_t v, uint32_t userAccess, uint32_t keyLength, std::vector<uint8_t> const &documentId, bool isDocMetadataEncrypted, pdfout::UString const &userPassword, pdfout::UString const &ownerPassword)
    : SecurityHandler(v, keyLength),
    mRevision(calculateRevision(v, userAccess)),
    mUserAccess(userAccess | UserAccessDefault),
    mDocumentId(documentId),
    mIsDocMetadataEncrypted(isDocMetadataEncrypted),
    mEncryptionKey(){

    std::vector<uint8_t> userPwd;
    std::vector<uint8_t> ownerPwd;

    pdfout::UString userPwdUtf8 = userPassword.saslPrepProfile();
    pdfout::UString ownerPwdUtf8 = ownerPassword.saslPrepProfile();

    if (mRevision <= 4){
      userPwd = padPassword(userPwdUtf8.getData(), userPwdUtf8.getSize());
      ownerPwd = padPassword(ownerPwdUtf8.getData(), ownerPwdUtf8.getSize());
    }
    else if (mRevision == 5){
      userPwd = truncatePassword(userPwdUtf8.getData(), userPwdUtf8.getSize(), 127);
      ownerPwd = truncatePassword(ownerPwdUtf8.getData(), ownerPwdUtf8.getSize(), 127);
    }
    else
      RUNTIME_EXCEPTION("Unknown revision number");

    if (mRevision <= 4){
      calculateOwnerKeyRev4(userPwd, ownerPwd);
      calculateEncryptionKeyRev4(userPwd);

      if (mRevision == 2)
        calculateUserKeyRev2();
      else if (mRevision == 3 || mRevision == 4)
        calculateUserKeyRev4();
    }
    else if (mRevision == 5){
      calculateEncryptionKeyRev5();
      calculateUserKeyRev5(userPwd);
      calculateOwnerKeyRev5(ownerPwd);
      calculatePermsValueRev5();
    }
    else
      RUNTIME_EXCEPTION("Invalid revision number");
  }

  StandardSecurityHandler::~StandardSecurityHandler(void) {}

  std::unique_ptr<ObjectIndirectReference> StandardSecurityHandler::serialize(pdfout::OutputStream *stream, SerializeParams *params) const{
    auto nonEcryptParams = removeEncryption(params);

    std::unique_ptr<StandardEncryptionDictionary> encryptionDictionary = std::make_unique<StandardEncryptionDictionary>();
    std::unique_ptr<ObjectIndirectReference> ref = encryptionDictionary->getReference();

    SecurityHandler::serialize(encryptionDictionary.get(), stream, nonEcryptParams.get());

    encryptionDictionary->addKey(StandardEncryptionDictionaryKeyR, std::make_unique<ObjectInteger>(mRevision));
    encryptionDictionary->addKey(StandardEncryptionDictionaryKeyO, std::make_unique<ObjectASCIIString>(mOwnerKey.data(), mOwnerKey.size()));
    encryptionDictionary->addKey(StandardEncryptionDictionaryKeyU, std::make_unique<ObjectASCIIString>(mUserKey.data(), mUserKey.size()));
    encryptionDictionary->addKey(StandardEncryptionDictionaryKeyP, std::make_unique<ObjectInteger>(mUserAccess));

    if (mRevision == 4)
      encryptionDictionary->addKey(StandardEncryptionDictionaryKeyEncryptMetadata, std::make_unique<ObjectBoolean>(mIsDocMetadataEncrypted));
    else if (mRevision == 5){
      encryptionDictionary->addKey(StandardEncryptionDictionaryKeyEncryptMetadata, std::make_unique<ObjectBoolean>(mIsDocMetadataEncrypted));
      encryptionDictionary->addKey(StandardEncryptionDictionaryKeyUE, std::make_unique<ObjectASCIIString>(mUserEncryptionKey.data(), mUserEncryptionKey.size()));
      encryptionDictionary->addKey(StandardEncryptionDictionaryKeyOE, std::make_unique<ObjectASCIIString>(mOwnerEncryptionKey.data(), mOwnerEncryptionKey.size()));
      encryptionDictionary->addKey(StandardEncryptionDictionaryKeyPerms, std::make_unique<ObjectASCIIString>(mPermsValue.data(), mPermsValue.size()));
    }

    encryptionDictionary->serialize(stream, nonEcryptParams.get());
    return ref;
  }

  std::vector<uint8_t> StandardSecurityHandler::getEncryptionKey(void) const{
    return mEncryptionKey;
  }

  uint32_t StandardSecurityHandler::calculateRevision(uint32_t v, uint32_t userAccess) const{
    bool const isRev3Access = userAccess & userAccessRev3;
    if (v == 1 && !isRev3Access)
      return 2;
    else if ((v == 2 || v == 3) || (v == 1 && isRev3Access))
      return 3;
    else if (v == 4)
      return 4;
    else if (v == 5)
      return 5;

    RUNTIME_EXCEPTION("Failed to calculate revision value.");
  }

  void StandardSecurityHandler::calculateOwnerKeyRev4(std::vector<uint8_t> const &userPassword, std::vector<uint8_t> const &ownerPassword){
    using namespace CryptoPP;

    MD5 hash;

    std::vector<uint8_t> const &ownerPwdPad = ownerPassword.empty() ? userPassword : ownerPassword;
    std::vector<uint8_t> md5(MD5::DIGESTSIZE);
    hash.CalculateDigest(md5.data(), ownerPwdPad.data(), ownerPwdPad.size());

    if (mRevision == 3 || mRevision == 4){
      std::vector<uint8_t> md5tmp(MD5::DIGESTSIZE);
      for (int i = 0; i < 50; i += 2){
        hash.CalculateDigest(md5tmp.data(), md5.data(), MD5::DIGESTSIZE);
        hash.CalculateDigest(md5.data(), md5tmp.data(), MD5::DIGESTSIZE);
      }
    }

    uint32_t const keyLength = (mRevision == 2) ? 5 : mKeyLength / 8;
    std::vector<uint8_t> rc4key = md5;
    ARC4 rc4(rc4key.data(), keyLength);

    std::vector<uint8_t> userPwdPad = userPassword;
    std::size_t const userPwdPadSize = userPwdPad.size();
    rc4.ProcessString(userPwdPad.data(), userPwdPadSize);

    if (mRevision == 3 || mRevision == 4){
      for (uint8_t i = 1; i <= 19; ++i){
        rc4key = md5;
        for (auto &elem : rc4key)
          elem ^= i;

        rc4.SetKey(rc4key.data(), keyLength);
        rc4.ProcessString(userPwdPad.data(), userPwdPadSize);
      }
    }

    mOwnerKey = userPwdPad;
  }

  void StandardSecurityHandler::calculateUserKeyRev2(void){
    using namespace CryptoPP;

    ARC4 rc4(mEncryptionKey.data(), mEncryptionKey.size());

    mUserKey.resize(32);
    rc4.ProcessData(mUserKey.data(), padValue.data(), padValue.size());
  }

  void StandardSecurityHandler::calculateUserKeyRev4(void){
    using namespace CryptoPP;

    MD5 hash;
    hash.Update(padValue.data(), padValue.size());
    hash.Update(mDocumentId.data(), mDocumentId.size());

    std::vector<uint8_t> out(MD5::DIGESTSIZE);
    hash.Final(out.data());

    std::vector<uint8_t> rc4key = mEncryptionKey;
    ARC4 rc4(rc4key.data(), rc4key.size());
    rc4.ProcessString(out.data(), MD5::DIGESTSIZE);

    for (uint8_t i = 1; i <= 19; ++i){
      rc4key = mEncryptionKey;
      for (auto &elem : rc4key)
        elem ^= i;

      rc4.SetKey(rc4key.data(), rc4key.size());
      rc4.ProcessString(out.data(), MD5::DIGESTSIZE);
    }

    mUserKey = padPassword(out);
  }

  void StandardSecurityHandler::calculateEncryptionKeyRev4(std::vector<uint8_t> const &password){
    using namespace CryptoPP;

    MD5 hash;
    hash.Update(password.data(), password.size());
    hash.Update(mOwnerKey.data(), mOwnerKey.size());
    uint8_t const ext[4] = {mUserAccess & 0xFF, (mUserAccess >>  8) & 0xFF, (mUserAccess >> 16) & 0xFF, (mUserAccess >> 24) & 0xFF};
    hash.Update(ext, 4);
    hash.Update(mDocumentId.data(), mDocumentId.size());

    if (mRevision == 4 && !mIsDocMetadataEncrypted){
      uint8_t const stub[4] = {0xFF, 0xFF, 0xFF, 0xFF};
      hash.Update(stub, 4);
    }

    std::vector<uint8_t> md5(MD5::DIGESTSIZE);
    hash.Final(md5.data());

    if (mRevision == 3 || mRevision == 4){
      std::vector<uint8_t> md5tmp(MD5::DIGESTSIZE);
      std::size_t const length = mKeyLength / 8;
      for (int i = 0; i < 50; i += 2){
        hash.CalculateDigest(md5tmp.data(), md5.data(), length);
        hash.CalculateDigest(md5.data(), md5tmp.data(), length);
      }
    }

    std::size_t const keyLength = (mRevision == 2) ? 5 : mKeyLength / 8;
    mEncryptionKey.resize(keyLength, 0);
    memcpy(mEncryptionKey.data(), md5.data(), keyLength * sizeof(uint8_t));
  }

  void StandardSecurityHandler::calculateEncryptionKeyRev5(void){
    mEncryptionKey.resize(32);
    fillRandom(mEncryptionKey);
  }

  std::vector<uint8_t> StandardSecurityHandler::padPassword(uint8_t const *password, std::size_t length) const{
    std::vector<uint8_t> result(32, 0);
    if (length < 32){
      memcpy(result.data(), password, length);
      memcpy(&result[length], padValue.data(), 32 - length);
    }
    else
      memcpy(result.data(), password, 32);

    return result;
  }

  std::vector<uint8_t> StandardSecurityHandler::padPassword(std::vector<uint8_t> const &password) const{
    return padPassword(password.data(), password.size());
  }

  std::vector<uint8_t> StandardSecurityHandler::calculateKeyRev5(std::vector<uint8_t> const &password, std::vector<uint8_t> const &validationSalt, std::vector<uint8_t> const &keySalt, std::vector<uint8_t> const &externalData) const{
    using namespace CryptoPP;

    SHA256 sha;
    sha.Update((byte *) password.data(), password.size());
    sha.Update((byte *) validationSalt.data(), validationSalt.size());
    if (!externalData.empty())
      sha.Update((byte *) externalData.data(), externalData.size());

    uint8_t hash[CryptoPP::SHA256::DIGESTSIZE] = {0};
    sha.Final(hash);

    std::vector<uint8_t> key(48, 0);
    memcpy(&key[0], hash, 32);
    memcpy(&key[32], validationSalt.data(), 8);
    memcpy(&key[40], keySalt.data(), 8);

    return key;
  }

  void StandardSecurityHandler::calculateUserKeyRev5(std::vector<uint8_t> const &userPassword){
    std::vector<uint8_t> validationSalt(8, 0);
    std::vector<uint8_t> keySalt(8, 0);
    fillRandom(validationSalt);
    fillRandom(keySalt);

    mUserKey = calculateKeyRev5(userPassword, validationSalt, keySalt, {});
    mUserEncryptionKey = calculateUserEncryptionKey(userPassword, keySalt, {});
  }

  void StandardSecurityHandler::calculateOwnerKeyRev5(std::vector<uint8_t> const &ownerPassword){
    std::vector<uint8_t> validationSalt(8, 0);
    std::vector<uint8_t> keySalt(8, 0);
    fillRandom(validationSalt);
    fillRandom(keySalt);

    mOwnerKey = calculateKeyRev5(ownerPassword, validationSalt, keySalt, mUserKey);
    mOwnerEncryptionKey = calculateUserEncryptionKey(ownerPassword, keySalt, mUserKey);
  }

  std::vector<uint8_t> StandardSecurityHandler::calculateUserEncryptionKey(std::vector<uint8_t> const &password, std::vector<uint8_t> const &keySalt, std::vector<uint8_t> const &externalData) const{
    using namespace CryptoPP;

    SHA256 sha;
    sha.Update((byte *) password.data(), password.size());
    sha.Update((byte *) keySalt.data(), keySalt.size());
    if (!externalData.empty())
      sha.Update((byte *) externalData.data(), externalData.size());

    std::vector<uint8_t> hash(CryptoPP::SHA256::DIGESTSIZE, 0);
    sha.Final(hash.data());

    std::vector<uint8_t> iv(CryptoPP::AES::BLOCKSIZE, 0);
    CBC_Mode<AES>::Encryption encryptor;
    encryptor.SetKeyWithIV(hash.data(), hash.size(), iv.data());

    StreamTransformationFilter ss(encryptor, nullptr, BlockPaddingSchemeDef::NO_PADDING);
    ss.Put(mEncryptionKey.data(), mEncryptionKey.size());
    ss.MessageEnd();

    std::vector<uint8_t> cipher(32, 0);
    ss.Get(cipher.data(), 32);
    return cipher;
  }

  void StandardSecurityHandler::calculatePermsValueRev5(void){
    using namespace CryptoPP;

    std::vector<uint8_t> perms(16, 0);
    perms[0] = mUserAccess & 0xFF;
    perms[1] = (mUserAccess >> 8) & 0xFF;
    perms[2] = (mUserAccess >> 16) & 0xFF;
    perms[3] = (mUserAccess >> 24) & 0xFF;
    perms[4] = 0xFF;
    perms[5] = 0xFF;
    perms[6] = 0xFF;
    perms[7] = 0xFF;
    perms[8] = mIsDocMetadataEncrypted ? 'T' : 'F';
    perms[9] = 'a';
    perms[10] = 'd';
    perms[11] = 'b';
    perms[12] = 0;
    perms[13] = 0;
    perms[14] = 0;
    perms[15] = 0;

    ECB_Mode<AES>::Encryption encryptor;
    encryptor.SetKey(mEncryptionKey.data(), mEncryptionKey.size());

    StreamTransformationFilter ss(encryptor, nullptr, BlockPaddingSchemeDef::NO_PADDING);
    ss.Put(perms.data(), perms.size());
    ss.MessageEnd();

    mPermsValue.resize(16, 0);
    ss.Get(mPermsValue.data(), 16);
  }

  std::vector<uint8_t> StandardSecurityHandler::truncatePassword(uint8_t const *password, std::size_t length, std::size_t maxLength) const{
    std::size_t const targetLength = std::min(length, maxLength);
    std::vector<uint8_t> truncatedPwd(targetLength, 0);
    memcpy(truncatedPwd.data(), password, targetLength);
    return truncatedPwd;
  }

}}
