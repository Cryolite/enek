#include <enek/unicode/char.hpp>
#include <gtest/gtest.h>
#include <sstream>


TEST(UnicodeCharTest, testGetCharMaxValue)
{
  EXPECT_EQ(0x10FFFF, Enek::Unicode::getCharMaxValue());
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
