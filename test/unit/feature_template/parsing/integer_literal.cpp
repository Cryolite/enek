#include <enek/feature_template/parsing/integer_literal.hpp>
#include <enek/feature_template/parsing/text_position_iterator.hpp>
#include <gtest/gtest.h>
#include <filesystem>
#include <sstream>
#include <string>
#include <optional>
#include <cstdint>


using IntegerLiteral = Enek::FeatureTemplate::Parsing::IntegerLiteral;

TEST(FeatureTemplateParsingIntegerLiteralTest, testDefaultConstructor)
{
  IntegerLiteral il;
  EXPECT_FALSE(il.isInitialized());
}

TEST(FeatureTemplateParsingIntegerLiteralTest, testInitializeWithPositiveInteger)
{
  IntegerLiteral il;
  std::string text("9223372036854775807");
  using BaseIterator = std::string::const_iterator;
  using BaseIteratorRange = boost::iterator_range<BaseIterator>;
  BaseIteratorRange text_range(text.cbegin(), text.cend());
  using Path = std::filesystem::path;
  using Iterator = Enek::FeatureTemplate::Parsing::TextPositionIterator<BaseIterator>;
  using IteratorRange = boost::iterator_range<Iterator>;
  IteratorRange parse_range = Iterator::makeIteratorRange(text_range);
  std::ostringstream oss;
  il.initialize(parse_range, Path(), text_range, oss);
  ASSERT_TRUE(il.isInitialized());
  EXPECT_TRUE(il.succeed());
  std::optional<std::int_fast64_t> value = il.getValue();
  ASSERT_TRUE(value.has_value());
  EXPECT_EQ(9223372036854775807, *value);
  EXPECT_TRUE(oss.str().empty());
}

TEST(FeatureTemplateParsingIntegerLiteralTest, testInitializeWithNegativeInteger)
{
  IntegerLiteral il;
  std::string text("-9223372036854775808");
  using BaseIterator = std::string::const_iterator;
  using BaseIteratorRange = boost::iterator_range<BaseIterator>;
  BaseIteratorRange text_range(text.cbegin(), text.cend());
  using Path = std::filesystem::path;
  using Iterator = Enek::FeatureTemplate::Parsing::TextPositionIterator<BaseIterator>;
  using IteratorRange = boost::iterator_range<Iterator>;
  IteratorRange parse_range = Iterator::makeIteratorRange(text_range);
  std::ostringstream oss;
  il.initialize(parse_range, Path(), text_range, oss);
  ASSERT_TRUE(il.isInitialized());
  EXPECT_TRUE(il.succeed());
  std::optional<std::int_fast64_t> value = il.getValue();
  ASSERT_TRUE(value.has_value());
  EXPECT_TRUE(std::numeric_limits<std::int64_t>::min() == *value);
  EXPECT_TRUE(oss.str().empty());
}

#if 0

TEST(FeatureTemplateParsingIntegerLiteralTest, testInitializeWithTooLargeValue0)
{
  IntegerLiteral il;
  std::string text("9300000000000000000");
  using BaseIterator = std::string::const_iterator;
  using BaseIteratorRange = boost::iterator_range<BaseIterator>;
  BaseIteratorRange text_range(text.cbegin(), text.cend());
  using Iterator = Enek::FeatureTemplate::Parsing::TextPositionIterator<BaseIterator>;
  using IteratorRange = boost::iterator_range<Iterator>;
  IteratorRange parse_range = Iterator::makeIteratorRange(text_range);
  using Path = std::filesystem::path;
  std::ostringstream oss;
  il.initialize(parse_range, Path(), text_range, oss);
  ASSERT_TRUE(il.isInitialized());
  EXPECT_FALSE(il.succeed());
  std::optional<std::int_fast64_t> value = il.getValue();
  //ASSERT_FALSE(value.has_value());
  //EXPECT_TRUE(std::numeric_limits<std::int64_t>::min() == *value);
  EXPECT_STREQ(R"(filename:1:1: error: An integer literal is too large. It should be within the range [)", oss.str().c_str());
}

#endif

