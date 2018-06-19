#include <common.hpp>
#include <enek/feature_template/parsing/integer_literal.hpp>
#include <gtest/gtest.h>
#include <boost/exception/exception.hpp>
#include <ostream>
#include <stdexcept>


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
  {
    std::ostringstream oss;
    EXPECT_THROW(il.dumpXML(oss), std::invalid_argument);
    EXPECT_THROW(il.dumpXML(oss), boost::exception);
  }
}

TEST(FeatureTemplateParsingIntegerLiteralTest, testParse)
{
  testParse("0",
            "<feature_template type=\"integer\"><integer_literal>0</integer_literal></feature_template>",
            "");
  testParse("1",
            "<feature_template type=\"integer\"><integer_literal>1</integer_literal></feature_template>",
            "");
  testParse("9223372036854775807",
            "<feature_template type=\"integer\"><integer_literal>9223372036854775807</integer_literal></feature_template>",
            "");
  testParse("-1",
            "<feature_template type=\"integer\"><integer_literal>-1</integer_literal></feature_template>",
            "");
  testParse("-9223372036854775808",
            "<feature_template type=\"integer\"><integer_literal>-9223372036854775808</integer_literal></feature_template>",
            "");
}

TEST(FeatureTemplateParsingIntegerLiteralTest, testParseFailure)
{
  testParse("9223372036854775808",
            "<feature_template succeed=\"false\" type=\"integer\"><integer_literal succeed=\"false\"/></feature_template>",
            "1:1: error: An integer literal is too large.\n"
            "9223372036854775808\n"
            "^^^^^^^^^^^^^^^^^^^\n");
  testParse("9300000000000000000",
            "<feature_template succeed=\"false\" type=\"integer\"><integer_literal succeed=\"false\"/></feature_template>",
            "1:1: error: An integer literal is too large.\n"
            "9300000000000000000\n"
            "^^^^^^^^^^^^^^^^^^^\n");
  testParse("-9223372036854775809",
            "<feature_template succeed=\"false\" type=\"integer\"><integer_literal succeed=\"false\"/></feature_template>",
            "1:1: error: An integer literal is too small.\n"
            "-9223372036854775809\n"
            "^^^^^^^^^^^^^^^^^^^^\n");
  testParse("-9300000000000000000",
            "<feature_template succeed=\"false\" type=\"integer\"><integer_literal succeed=\"false\"/></feature_template>",
            "1:1: error: An integer literal is too small.\n"
            "-9300000000000000000\n"
            "^^^^^^^^^^^^^^^^^^^^\n");
}
