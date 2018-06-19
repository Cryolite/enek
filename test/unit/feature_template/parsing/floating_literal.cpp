#include <common.hpp>
#include <enek/feature_template/parsing/floating_literal.hpp>
#include <gtest/gtest.h>
#include <boost/exception/exception.hpp>
#include <ostream>
#include <stdexcept>


using FloatingLiteral = Enek::FeatureTemplate::Parsing::FloatingLiteral;

TEST(FeatureTemplateParsingFloatingLiteralTest, testDefaultConstructor)
{
  FloatingLiteral fl;
  EXPECT_FALSE(fl.isInitialized());
  EXPECT_THROW(fl.succeed(), std::invalid_argument);
  EXPECT_THROW(fl.succeed(), boost::exception);
  EXPECT_THROW(fl.getType(), std::invalid_argument);
  EXPECT_THROW(fl.getType(), boost::exception);
  EXPECT_THROW(fl.getValue(), std::invalid_argument);
  EXPECT_THROW(fl.getValue(), boost::exception);
  {
    std::ostringstream oss;
    EXPECT_THROW(fl.dumpXML(oss), std::invalid_argument);
    EXPECT_THROW(fl.dumpXML(oss), boost::exception);
  }
}

TEST(FeatureTemplateParsingFloatingLiteralTest, testParse)
{
  testParse("0.",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.e0",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.e+0",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.e-0",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.E0",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.E+0",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.E-0",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.0",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.0e0",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.0e+0",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.0e-0",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.0E0",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.0E+0",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.0E-0",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse(".0",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse(".0e0",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse(".0e+0",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse(".0e-0",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse(".0E0",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse(".0E+0",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse(".0E-0",
            "<feature_template type=\"floating\"><floating_literal>0</floating_literal></feature_template>",
            "");
}

TEST(FeatureTemplateParsingFloatingLiteralTest, testParseFailure)
{
  testParse("1.0E+10000",
            "<feature_template succeed=\"false\" type=\"floating\"><floating_literal succeed=\"false\"/></feature_template>",
            "1:1: error: An error occurred while parsing a floating literal.\n"
            "1.0E+10000\n"
            "^^^^^^^^^^\n");
}
