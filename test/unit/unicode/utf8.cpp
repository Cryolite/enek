#include <enek/unicode/utf8.hpp>
#include <gtest/gtest.h>
#include <boost/exception/get_error_info.hpp>
#include <boost/exception/exception.hpp>
#include <regex>
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

TEST(UnicodeUtf8Test, testGetWidthOnConsole)
{
  using Enek::Unicode::UTF8::getWidthOnConsole;
  {
    std::string str("\u0067\u0308");
    EXPECT_EQ(1, getWidthOnConsole(str));
  }
  {
    std::string str("\uAC01");
    EXPECT_EQ(2, getWidthOnConsole(str));
  }
  {
    std::string str("\u1100\u1161\u11A8");
    EXPECT_EQ(2, getWidthOnConsole(str));
  }
}

TEST(UnicodeUtf8Test, testGetWidthOnConsoleForInvalidCharacter)
{
  using Enek::Unicode::UTF8::getWidthOnConsole;
  {
    // The straight forward encoding of the sequence of code points
    // `U+D852 U+DF62' (results in a valid code point `U+24B62' if this
    // sequence is interpreted as a UTF-16 string) to UTF-8, which should
    // result in an error.
    std::string str("\xED\xA1\x92\xED\xBD\xA2");
    try {
      EXPECT_EQ(1, getWidthOnConsole(str));
    }
    catch (boost::exception const &e) {
      {
        char const * const *p = boost::get_error_info<boost::throw_file>(e);
        ASSERT_NE(p, nullptr);
        EXPECT_TRUE(std::regex_search(*p, std::regex(R"(utf8\.cpp$)")));
      }
      {
        char const * const *p = boost::get_error_info<boost::throw_function>(e);
        ASSERT_NE(p, nullptr);
      }
      {
        int const *p = boost::get_error_info<boost::throw_line>(e);
        ASSERT_NE(p, nullptr);
      }
      {
        std::exception const *p = dynamic_cast<std::exception const *>(&e);
        ASSERT_NE(p, nullptr);
        EXPECT_STREQ("The argument `str' is not a valid UTF-8 string.",
                     p->what());
      }
    }
  }
  {
    // The straight forward encoding of an unpaired surrogate `U+D800' to
    // UTF-8, which should result in an error.
    std::string str("\xED\xA0\x80");
    try {
      EXPECT_EQ(1, getWidthOnConsole(str));
    }
    catch (boost::exception const &e) {
      {
        char const * const *p = boost::get_error_info<boost::throw_file>(e);
        ASSERT_NE(p, nullptr);
        EXPECT_TRUE(std::regex_search(*p, std::regex(R"(utf8\.cpp$)")));
      }
      {
        char const * const *p = boost::get_error_info<boost::throw_function>(e);
        ASSERT_NE(p, nullptr);
      }
      {
        int const *p = boost::get_error_info<boost::throw_line>(e);
        ASSERT_NE(p, nullptr);
      }
      {
        std::exception const *p = dynamic_cast<std::exception const *>(&e);
        ASSERT_NE(p, nullptr);
        EXPECT_STREQ("The argument `str' is not a valid UTF-8 string.",
                     p->what());
      }
    }
  }
}
