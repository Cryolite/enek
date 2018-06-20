#include <common.hpp>
#include <enek/feature_template/parsing/boolean_literal.hpp>
#include <gtest/gtest.h>
#include <boost/exception/exception.hpp>
#include <stdexcept>


using BooleanLiteral = Enek::FeatureTemplate::Parsing::BooleanLiteral;

TEST(FeatureTemplateParsingBooleanLiteralTest, testDefaultConstructor)
{
  BooleanLiteral bl;
  EXPECT_FALSE(bl.isInitialized());
  EXPECT_THROW(bl.succeed(), std::invalid_argument);
  EXPECT_THROW(bl.succeed(), boost::exception);
  EXPECT_THROW(bl.getType(), std::invalid_argument);
  EXPECT_THROW(bl.getType(), boost::exception);
  EXPECT_THROW(bl.getValue(), std::invalid_argument);
  EXPECT_THROW(bl.getValue(), boost::exception);
}

TEST(FeatureTemplateParsingBooleanLiteralTest, testParse)
{
  testParse("false",
            "<boolean_literal>false</boolean_literal>",
            "");
  testParse("true",
            "<boolean_literal>true</boolean_literal>",
            "");
}

TEST(FeatureTemplateParsingBooleanLiteralTest, testParseFailure)
{
}
