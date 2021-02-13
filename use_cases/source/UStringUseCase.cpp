#include <gtest/gtest.h>

#include <memory>
#include <string>

#include <UString.h>

using namespace pdfout;

class UStringUseCase : public ::testing::Test{
};

TEST_F(UStringUseCase, CtrCharConst){
  char const *str = "Hello, world!";
  std::size_t const len = strlen(str);

  UString const ustr(str, Encoding_ASCII);

  ASSERT_EQ(len, strlen((char const *)ustr.getData()));
  ASSERT_EQ(len + 1, ustr.getSize());
  ASSERT_EQ(memcmp(str, ustr.getData(), (len + 1) * sizeof(char)), 0);
}

TEST_F(UStringUseCase, CtrWcharConst){
  wchar_t const *str = L"Hello, world!";
  std::size_t const len = wcslen(str);

  UString const ustr(str, Encoding_WCHAR_T);

  ASSERT_EQ(len, wcslen((wchar_t const *)ustr.getData()));
  ASSERT_EQ((len + 1) * sizeof(wchar_t), ustr.getSize());
  ASSERT_EQ(memcmp(str, ustr.getData(), (len + 1) * sizeof(wchar_t)), 0);
}

TEST_F(UStringUseCase, CtrStrConst){
  std::string const str = "Hello, world!";
  std::size_t const len = str.length();
  UString const ustr(str.c_str());

  ASSERT_EQ(len, strlen((char const *) ustr.getData()));
  ASSERT_EQ((len + 1), ustr.getSize());
  ASSERT_EQ(memcmp(str.c_str(), ustr.getData(), (len + 1) * sizeof(char)), 0);
}

TEST_F(UStringUseCase, CtrWstrConst){
  std::wstring const str = L"Hello, world!";
  std::size_t const len = str.length();
  UString const ustr(str.c_str());

  ASSERT_EQ(len, wcslen((wchar_t const *) ustr.getData()));
  ASSERT_EQ((len + 1) * sizeof(wchar_t), ustr.getSize());
  ASSERT_EQ(memcmp(str.c_str(), ustr.getData(), (len + 1) * sizeof(wchar_t)), 0);
}

TEST_F(UStringUseCase, CtrRawConst){
  uint8_t const str[] = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!'};
  std::size_t const len = sizeof(str) / sizeof(uint8_t);
  UString const ustr(str, len, Encoding_ASCII);

  ASSERT_EQ(len, ustr.getSize());
  ASSERT_EQ(memcmp(str, ustr.getData(), len), 0);
}

TEST_F(UStringUseCase, CtrCopyChar){
  char const *str = "Hello, world!";
  UString const ustr(str, Encoding_ASCII);
  UString const ustr2(ustr);

  ASSERT_EQ(ustr.getSize(), ustr2.getSize());
  ASSERT_EQ(ustr.getEncoding(), ustr2.getEncoding());
  ASSERT_EQ((memcmp(ustr.getData(), ustr2.getData(), ustr.getSize())), 0);
}

TEST_F(UStringUseCase, CtrMoveChar){
  char const *str = "Hello, world!";
  UString ustr(str, Encoding_ASCII);

  Encoding const encoding = ustr.getEncoding();
  std::size_t const size = ustr.getSize();
  std::vector<uint8_t> const data(ustr.getData(), ustr.getData() + ustr.getSize());

  UString const ustr2 = std::move(ustr);

  ASSERT_EQ(ustr.getSize(), 0);
  ASSERT_EQ(ustr.getData(), nullptr);

  ASSERT_EQ(ustr2.getSize(), size);
  ASSERT_EQ(ustr2.getEncoding(), encoding);
  ASSERT_EQ((memcmp(ustr2.getData(), data.data(), ustr2.getSize())), 0);
}

TEST_F(UStringUseCase, AssignOpChar){
  char const *str = "Hello, world!";
  UString const ustr(str, Encoding_ASCII);

  Encoding const ustr2enc = Encoding_850;
  UString ustr2("some test message", ustr2enc);

  ASSERT_EQ(ustr2.getEncoding(), Encoding_850);

  ustr2 = ustr;

  ASSERT_EQ(ustr.getSize(), ustr2.getSize());
  ASSERT_EQ(ustr.getEncoding(), ustr2.getEncoding());
  ASSERT_EQ((memcmp(ustr.getData(), ustr2.getData(), ustr.getSize())), 0);
}

TEST_F(UStringUseCase, AssignOpMoveChar){
  char const *str = "Hello, world!";
  UString ustr(str, Encoding_ASCII);

  Encoding const ustr2enc = Encoding_850;
  UString ustr2("some test message", ustr2enc);

  ASSERT_EQ(ustr2.getEncoding(), Encoding_850);

  Encoding const encoding = ustr.getEncoding();
  std::size_t const size = ustr.getSize();
  std::vector<uint8_t> const data(ustr.getData(), ustr.getData() + ustr.getSize());

  ustr2 = std::move(ustr);

  ASSERT_EQ(ustr.getSize(), 0);
  ASSERT_EQ(ustr.getData(), nullptr);

  ASSERT_EQ(ustr2.getSize(), size);
  ASSERT_EQ(ustr2.getEncoding(), encoding);
  ASSERT_EQ((memcmp(ustr2.getData(), data.data(), ustr2.getSize())), 0);
}
