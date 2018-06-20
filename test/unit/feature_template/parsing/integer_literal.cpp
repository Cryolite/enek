#include <common.hpp>
#include <enek/feature_template/parsing/integer_literal.hpp>
#include <gtest/gtest.h>
#include <boost/exception/exception.hpp>
#include <stdexcept>


using Type = Enek::FeatureTemplate::Type;
using IntegerLiteral = Enek::FeatureTemplate::Parsing::IntegerLiteral;

TEST(FeatureTemplateParsingIntegerLiteralTest, testDefaultConstructor)
{
  IntegerLiteral il;
  EXPECT_FALSE(il.isInitialized());
  EXPECT_THROW(il.succeed(), std::invalid_argument);
  EXPECT_THROW(il.succeed(), boost::exception);
  EXPECT_THROW(il.getType(), std::invalid_argument);
  EXPECT_THROW(il.getType(), boost::exception);
  EXPECT_THROW(il.getValue(), std::invalid_argument);
  EXPECT_THROW(il.getValue(), boost::exception);
}

TEST(FeatureTemplateParsingIntegerLiteralTest, testParse)
{
  testParse("0",
            true,
            Type::integer,
            "<integer_literal>0</integer_literal>",
            "");
  testParse("1",
            true,
            Type::integer,
            "<integer_literal>1</integer_literal>",
            "");
  testParse("9223372036854775807",
            true,
            Type::integer,
            "<integer_literal>9223372036854775807</integer_literal>",
            "");
  testParse("-1",
            true,
            Type::integer,
            "<integer_literal>-1</integer_literal>",
            "");
  testParse("-9223372036854775808",
            true,
            Type::integer,
            "<integer_literal>-9223372036854775808</integer_literal>",
            "");
}

TEST(FeatureTemplateParsingIntegerLiteralTest, testParseFailure)
{
  testParse("9223372036854775808",
            false,
            Type::integer,
            "<integer_literal succeed=\"false\"/>",
            "1:1: error: An integer literal is too large.\n"
            "9223372036854775808\n"
            "^^^^^^^^^^^^^^^^^^^\n");
  testParse("9300000000000000000",
            false,
            Type::integer,
            "<integer_literal succeed=\"false\"/>",
            "1:1: error: An integer literal is too large.\n"
            "9300000000000000000\n"
            "^^^^^^^^^^^^^^^^^^^\n");
  testParse("-9223372036854775809",
            false,
            Type::integer,
            "<integer_literal succeed=\"false\"/>",
            "1:1: error: An integer literal is too small.\n"
            "-9223372036854775809\n"
            "^^^^^^^^^^^^^^^^^^^^\n");
  testParse("-9300000000000000000",
            false,
            Type::integer,
            "<integer_literal succeed=\"false\"/>",
            "1:1: error: An integer literal is too small.\n"
            "-9300000000000000000\n"
            "^^^^^^^^^^^^^^^^^^^^\n");
}
