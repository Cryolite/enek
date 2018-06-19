#include <common.hpp>
#include <enek/feature_template/parsing/boolean_literal.hpp>
#include <gtest/gtest.h>
#include <boost/exception/exception.hpp>
#include <ostream>
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
  {
    std::ostringstream oss;
    EXPECT_THROW(bl.dumpXML(oss), std::invalid_argument);
    EXPECT_THROW(bl.dumpXML(oss), boost::exception);
  }
}

TEST(FeatureTemplateParsingBooleanLiteralTest, testParse)
{
  testParse("false",
            "<feature_template type=\"boolean\"><boolean_literal>false</boolean_literal></feature_template>",
            "");
  testParse("true",
            "<feature_template type=\"boolean\"><boolean_literal>true</boolean_literal></feature_template>",
            "");
}

TEST(FeatureTemplateParsingBooleanLiteralTest, testParseFailure)
{
}
