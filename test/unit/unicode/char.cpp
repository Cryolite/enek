#include <enek/unicode/char.hpp>
#include <gtest/gtest.h>


TEST(UnicodeCharTest, testGetCharMaxValue)
{
  EXPECT_EQ(0x10FFFF, Enek::Unicode::getCharMaxValue());
}

TEST(UtilCharTest, testIsOtherSurrogate)
{
  using Enek::Unicode::GeneralCategory::isOtherSurrogate;
  EXPECT_FALSE(isOtherSurrogate(U'\uD7FF'));
  EXPECT_TRUE(isOtherSurrogate(U'\xD800'));
  EXPECT_TRUE(isOtherSurrogate(U'\xDFFF'));
  EXPECT_FALSE(isOtherSurrogate(U'\uE000'));
}
