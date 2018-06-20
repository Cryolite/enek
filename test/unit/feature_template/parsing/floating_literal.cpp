#include <common.hpp>
#include <enek/feature_template/parsing/floating_literal.hpp>
#include <gtest/gtest.h>
#include <boost/exception/exception.hpp>
#include <stdexcept>


using Type = Enek::FeatureTemplate::Type;
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
}

TEST(FeatureTemplateParsingFloatingLiteralTest, testParse)
{
  testParse("0.",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
  testParse("0.e0",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
  testParse("0.e+0",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
  testParse("0.e-0",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
  testParse("0.E0",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
  testParse("0.E+0",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
  testParse("0.E-0",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
  testParse("0.0",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
  testParse("0.0e0",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
  testParse("0.0e+0",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
  testParse("0.0e-0",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
  testParse("0.0E0",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
  testParse("0.0E+0",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
  testParse("0.0E-0",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
  testParse(".0",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
  testParse(".0e0",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
  testParse(".0e+0",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
  testParse(".0e-0",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
  testParse(".0E0",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
  testParse(".0E+0",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
  testParse(".0E-0",
            true,
            Type::floating,
            "<floating_literal>0</floating_literal>",
            "");
}

TEST(FeatureTemplateParsingFloatingLiteralTest, testParseFailure)
{
  testParse("1.0E+10000",
            false,
            Type::floating,
            "<floating_literal succeed=\"false\"/>",
            "1:1: error: An error occurred while parsing a floating literal.\n"
            "1.0E+10000\n"
            "^^^^^^^^^^\n");
}
