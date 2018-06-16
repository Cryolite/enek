#include <enek/unicode/utf8.hpp>
#include <gtest/gtest.h>
#include <string>


TEST(UnicodeUtf8Test, testInvalidStringError)
{
  {
    Enek::Unicode::UTF8::InvalidStringError e("An invalid string.");
    EXPECT_STREQ("An invalid string.", e.what());
  }
  {
    std::string what("An invalid string.");
    Enek::Unicode::UTF8::InvalidStringError e(what);
    EXPECT_STREQ("An invalid string.", e.what());
  }
}

TEST(UnicodeUtf8Test, testAppendCodePoint)
{
  using Enek::Unicode::UTF8::appendCodePoint;
  {
    std::string s;
    appendCodePoint(s, U'\u0020');
    ASSERT_EQ(1, s.size());
    EXPECT_EQ('\x20', s[0]);
  }
  {
    std::string s;
    appendCodePoint(s, U'\u00A2');
    ASSERT_EQ(2, s.size());
    EXPECT_EQ("\xC2\xA2", s);
  }
  {
    std::string s;
    appendCodePoint(s, U'\u0800');
    ASSERT_EQ(3, s.size());
    EXPECT_EQ("\xE0\xA0\x80", s);
  }
  {
    std::string s;
    appendCodePoint(s, U'\U0001F363');
    ASSERT_EQ(4, s.size());
    EXPECT_EQ("\xF0\x9F\x8D\xA3", s);
  }
}

TEST(UnicodeUtf8Test, testAppendCodePointRefuseSurrogate)
{
  using Enek::Unicode::UTF8::appendCodePoint;
  std::string s;
  EXPECT_EXIT(
    [&s]() noexcept { appendCodePoint(s, U'\xD800'); }();,
    ::testing::KilledBySignal(SIGABRT),
    R"(utf8\.cpp:[[:digit:]]+: .+: Failed to append the Unicode code point `U\+D800'\.
(Git commit hash: [[:xdigit:]]+
)?Backtrace:
)");
}

TEST(UnicodeUtf8Test, testAppendCodePointRefuseOutOfRange)
{
  using Enek::Unicode::UTF8::appendCodePoint;
  std::string s;
  EXPECT_EXIT(
    [&s]() noexcept { appendCodePoint(s, U'\U00110000'); }();,
    ::testing::KilledBySignal(SIGABRT),
    R"(utf8\.cpp:[[:digit:]]+: .+: The argument `c' \(= `U\+110000'\) exceeds the highest Unicode code point value\.
(Git commit hash: [[:xdigit:]]+
)?Backtrace:
)");
}
