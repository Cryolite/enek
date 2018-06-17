#include <enek/unicode/char.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include <string>


TEST(UnicodeCharTest, testGetCharMaxValue)
{
  EXPECT_EQ(0x10FFFF, Enek::Unicode::getCharMaxValue());
}

TEST(UnicodeCharTest, testFormatAsCodePoint)
{
  using Enek::Unicode::formatAsCodePoint;
  {
    std::ostringstream oss;
    oss << U'\u0000' << ' ' << formatAsCodePoint(U'\u0000') << ' '
        << U'\u0000';
    EXPECT_STREQ("0 U+0000 0", oss.str().c_str());
  }
  {
    std::ostringstream oss;
    oss << U'\u0020' << ' ' << formatAsCodePoint(U'\u0020') << ' '
        << U'\u0020';
    EXPECT_STREQ("32 U+0020 32", oss.str().c_str());
  }
  {
    std::ostringstream oss;
    oss << U'\u0100' << ' ' << formatAsCodePoint(U'\u0100') << ' '
        << U'\u0100';
    EXPECT_STREQ("256 U+0100 256", oss.str().c_str());
  }
  {
    std::ostringstream oss;
    oss << U'\u1000' << ' ' << formatAsCodePoint(U'\u1000') << ' '
        << U'\u1000';
    EXPECT_STREQ("4096 U+1000 4096", oss.str().c_str());
  }
  {
    std::ostringstream oss;
    oss << U'\U00010000' << ' ' << formatAsCodePoint(U'\U00010000') << ' '
        << U'\U00010000';
    EXPECT_STREQ("65536 U+10000 65536", oss.str().c_str());
  }
  {
    std::ostringstream oss;
    oss << U'\U0010FFFF' << ' ' << formatAsCodePoint(U'\U0010FFFF') << ' '
        << U'\U0010FFFF';
    EXPECT_STREQ("1114111 U+10FFFF 1114111", oss.str().c_str());
  }
  {
    std::u32string str(U"\u1100\u1161\u11A8");
    std::ostringstream oss;
    oss << formatAsCodePoint(str.cbegin(), str.cend());
    EXPECT_STREQ("U+1100 U+1161 U+11A8", oss.str().c_str());
  }
}

TEST(UnicodeCharTest, testIsOtherSurrogate)
{
  using Enek::Unicode::GeneralCategory::isOtherSurrogate;
  EXPECT_FALSE(isOtherSurrogate(U'\uD7FF'));
  EXPECT_TRUE(isOtherSurrogate(U'\xD800'));
  EXPECT_TRUE(isOtherSurrogate(U'\xDFFF'));
  EXPECT_FALSE(isOtherSurrogate(U'\uE000'));
}

TEST(UnicodeCharTest, testEastAsianWidthOStream)
{
  using Enek::Unicode::EastAsianWidth;
  using Enek::Unicode::getEastAsianWidth;
  std::ostringstream oss;
  oss << EastAsianWidth::fullwidth;
  EXPECT_STREQ("East Asian Fullwidth", oss.str().c_str());
  oss = std::ostringstream();
  oss << EastAsianWidth::halfwidth;
  EXPECT_STREQ("East Asian Halfwidth", oss.str().c_str());
  oss = std::ostringstream();
  oss << EastAsianWidth::wide;
  EXPECT_STREQ("East Asian Wide", oss.str().c_str());
  oss = std::ostringstream();
  oss << EastAsianWidth::narrow;
  EXPECT_STREQ("East Asian Narrow", oss.str().c_str());
  oss = std::ostringstream();
  oss << EastAsianWidth::ambiguous;
  EXPECT_STREQ("East Asian Ambiguous", oss.str().c_str());
  oss = std::ostringstream();
  oss << EastAsianWidth::neutral;
  EXPECT_STREQ("East Asian Neutral", oss.str().c_str());
}

TEST(UnicodeCharTest, testGetEastAsianWidth)
{
  using Enek::Unicode::EastAsianWidth;
  using Enek::Unicode::getEastAsianWidth;
  char32_t c = 32;
  EXPECT_EQ(EastAsianWidth::fullwidth, getEastAsianWidth(U'\u3000'));
  EXPECT_EQ(EastAsianWidth::halfwidth, getEastAsianWidth(U'\u20A9'));
  EXPECT_EQ(EastAsianWidth::wide, getEastAsianWidth(U'\u1100'));
  EXPECT_EQ(EastAsianWidth::narrow, getEastAsianWidth(U'\u0020'));
  EXPECT_EQ(EastAsianWidth::ambiguous, getEastAsianWidth(U'\u0101'));
  EXPECT_EQ(EastAsianWidth::neutral, getEastAsianWidth(U'\u007F'));
}
