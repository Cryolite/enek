#include <enek/unicode/utf8.hpp>
#include <gtest/gtest.h>
#include <string>


TEST(UnicodeUtf8Test, testAppendCodePoint)
{
  {
    std::string s;
    Enek::Unicode::appendCodePoint(s, U'\u0020');
    ASSERT_EQ(1, s.size());
    EXPECT_EQ('\x20', s[0]);
  }
  {
    std::string s;
    Enek::Unicode::appendCodePoint(s, U'\u00A2');
    ASSERT_EQ(2, s.size());
    EXPECT_EQ("\xC2\xA2", s);
  }
  {
    std::string s;
    Enek::Unicode::appendCodePoint(s, U'\u0800');
    ASSERT_EQ(3, s.size());
    EXPECT_EQ("\xE0\xA0\x80", s);
  }
  {
    std::string s;
    Enek::Unicode::appendCodePoint(s, U'\U0001F363');
    ASSERT_EQ(4, s.size());
    EXPECT_EQ("\xF0\x9F\x8D\xA3", s);
  }
}

TEST(UnicodeUtf8Test, testAppendCodePointRefuseSurrogate)
{
  std::string s;
  EXPECT_EXIT(
    [&s]() noexcept { Enek::Unicode::appendCodePoint(s, U'\xD800'); }();,
    ::testing::KilledBySignal(SIGABRT),
    R"(utf8\.cpp:[[:digit:]]+: .+: Failed to append the Unicode code point `U\+D800'\.
(Git commit hash: [[:xdigit:]]+
)?Backtrace:
)");
}

TEST(UnicodeUtf8Test, testAppendCodePointRefuseOutOfRange)
{
  std::string s;
  EXPECT_EXIT(
    [&s]() noexcept { Enek::Unicode::appendCodePoint(s, U'\U00110000'); }();,
    ::testing::KilledBySignal(SIGABRT),
    R"(utf8\.cpp:[[:digit:]]+: .+: The argument `c' \(= `U\+110000'\) exceeds the highest Unicode code point value\.
(Git commit hash: [[:xdigit:]]+
)?Backtrace:
)");
}
