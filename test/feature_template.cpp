#include <enek/feature_template/parsing/parse.hpp>
#include <gtest/gtest.h>


namespace{

void testParse(
  std::string const &to_be_parsed,
  std::string const &dump_to_be_expected,
  std::string const &error_message_to_be_expected)
{
  namespace Parsing = Enek::FeatureTemplate::Parsing;
  std::ostringstream error_oss;
  Parsing::AST ast = Parsing::parse(to_be_parsed, error_oss);
  if (ast.succeed()) {
    std::ostringstream dump_oss;
    ast.dumpXML(dump_oss);
    ASSERT_STREQ(dump_oss.str().c_str(), dump_to_be_expected.c_str())
      << "text: " << to_be_parsed;
  }
  else {
    ASSERT_STREQ("", dump_to_be_expected.c_str());
  }
  ASSERT_STREQ(error_oss.str().c_str(), error_message_to_be_expected.c_str());
}

} // namespace *unnamed*

TEST(FeatureTemplateTest, acceptIntegerLiteral)
{
  testParse("0",
            "<feature_template><integer_literal>0</integer_literal></feature_template>",
            "");
  testParse("1",
            "<feature_template><integer_literal>1</integer_literal></feature_template>",
            "");
  testParse("-1",
            "<feature_template><integer_literal>-1</integer_literal></feature_template>",
            "");
  testParse("9223372036854775807",
            "<feature_template><integer_literal>9223372036854775807</integer_literal></feature_template>",
            "");
  testParse("9223372036854775808",
            "",
            "1:1: error: An integer literal is too large. It should be within the range [-9223372036854775808, 9223372036854775807].\n"
            "9223372036854775808\n"
            "^^^^^^^^^^^^^^^^^^^\n");
  testParse("-9223372036854775808",
            "<feature_template><integer_literal>-9223372036854775808</integer_literal></feature_template>",
            "");
  testParse("-9223372036854775809",
            "",
            "1:1: error: An integer literal is too large. It should be within the range [-9223372036854775808, 9223372036854775807].\n"
            "-9223372036854775809\n"
            "^^^^^^^^^^^^^^^^^^^^\n");
}

TEST(FeatureTemplateTest, acceptFloatingLiteral)
{
  testParse("0.",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.e0",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.e+0",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.e-0",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.E0",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.E+0",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.E-0",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.0",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.0e0",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.0e+0",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.0e-0",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.0E0",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.0E+0",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("0.0E-0",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse(".0",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse(".0e0",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse(".0e+0",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse(".0e-0",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse(".0E0",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse(".0E+0",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse(".0E-0",
            "<feature_template><floating_literal>0</floating_literal></feature_template>",
            "");
  testParse("5.",
            "<feature_template><floating_literal>5</floating_literal></feature_template>",
            "");
  testParse("5.e1",
            "<feature_template><floating_literal>50</floating_literal></feature_template>",
            "");
  testParse("5.e+1",
            "<feature_template><floating_literal>50</floating_literal></feature_template>",
            "");
  testParse("5.e-1",
            "<feature_template><floating_literal>0.5</floating_literal></feature_template>",
            "");
  testParse("5.E1",
            "<feature_template><floating_literal>50</floating_literal></feature_template>",
            "");
  testParse("5.E+1",
            "<feature_template><floating_literal>50</floating_literal></feature_template>",
            "");
  testParse("5.E-1",
            "<feature_template><floating_literal>0.5</floating_literal></feature_template>",
            "");
  testParse("12.5",
            "<feature_template><floating_literal>12.5</floating_literal></feature_template>",
            "");
  testParse("12.5e1",
            "<feature_template><floating_literal>125</floating_literal></feature_template>",
            "");
  testParse("12.5e+1",
            "<feature_template><floating_literal>125</floating_literal></feature_template>",
            "");
  testParse("12.5e-1",
            "<feature_template><floating_literal>1.25</floating_literal></feature_template>",
            "");
  testParse("12.5E1",
            "<feature_template><floating_literal>125</floating_literal></feature_template>",
            "");
  testParse("12.5E+1",
            "<feature_template><floating_literal>125</floating_literal></feature_template>",
            "");
  testParse("12.5E-1",
            "<feature_template><floating_literal>1.25</floating_literal></feature_template>",
            "");
}

TEST(FeatureTemplateTest, acceptBooleanLiteral)
{
  testParse("false",
            "<feature_template><boolean_literal>false</boolean_literal></feature_template>",
            "");
  testParse("true",
            "<feature_template><boolean_literal>true</boolean_literal></feature_template>",
            "");
}

TEST(FeatureTemplateTest, acceptStringLiteral)
{
  testParse("\"\"",
            "<feature_template><string_literal></string_literal></feature_template>",
            "");
  testParse("\"a\"",
            "<feature_template><string_literal>a</string_literal></feature_template>",
            "");
  testParse("\"\\'\"",
            "<feature_template><string_literal>'</string_literal></feature_template>",
            "");
  testParse("\"\\\"\"",
            "<feature_template><string_literal>\"</string_literal></feature_template>",
            "");
  testParse("\"\\?\"",
            "<feature_template><string_literal>?</string_literal></feature_template>",
            "");
  testParse("\"\\\\\"",
            "<feature_template><string_literal>\\</string_literal></feature_template>",
            "");
  testParse("\"\\a\"",
            "<feature_template><string_literal>\a</string_literal></feature_template>",
            "");
  testParse("\"\\b\"",
            "<feature_template><string_literal>\b</string_literal></feature_template>",
            "");
  testParse("\"\\f\"",
            "<feature_template><string_literal>\f</string_literal></feature_template>",
            "");
  testParse("\"\\n\"",
            "<feature_template><string_literal>\n</string_literal></feature_template>",
            "");
  testParse("\"\\r\"",
            "<feature_template><string_literal>\r</string_literal></feature_template>",
            "");
  testParse("\"\\t\"",
            "<feature_template><string_literal>\t</string_literal></feature_template>",
            "");
  testParse("\"\\v\"",
            "<feature_template><string_literal>\v</string_literal></feature_template>",
            "");
  testParse("\"\\0\"",
            "<feature_template><string_literal>\0</string_literal></feature_template>",
            "");
  testParse("\"\\0a\"",
            "<feature_template><string_literal>\0a</string_literal></feature_template>",
            "");
  testParse("\"\\1\"",
            "<feature_template><string_literal>\1</string_literal></feature_template>",
            "");
  testParse("\"\\1a\"",
            "<feature_template><string_literal>\1a</string_literal></feature_template>",
            "");
  testParse("\"\\00\"",
            "<feature_template><string_literal>\0</string_literal></feature_template>",
            "");
  testParse("\"\\00a\"",
            "<feature_template><string_literal>\0a</string_literal></feature_template>",
            "");
  testParse("\"\\01\"",
            "<feature_template><string_literal>\1</string_literal></feature_template>",
            "");
  testParse("\"\\01a\"",
            "<feature_template><string_literal>\1a</string_literal></feature_template>",
            "");
  testParse("\"\\10\"",
            "<feature_template><string_literal>\10</string_literal></feature_template>",
            "");
  testParse("\"\\10a\"",
            "<feature_template><string_literal>\10a</string_literal></feature_template>",
            "");
  testParse("\"\\11\"",
            "<feature_template><string_literal>\11</string_literal></feature_template>",
            "");
  testParse("\"\\11a\"",
            "<feature_template><string_literal>\11a</string_literal></feature_template>",
            "");
  testParse("\"\\000\"",
            "<feature_template><string_literal>\0</string_literal></feature_template>",
            "");
  testParse("\"\\000a\"",
            "<feature_template><string_literal>\0a</string_literal></feature_template>",
            "");
  testParse("\"\\001\"",
            "<feature_template><string_literal>\1</string_literal></feature_template>",
            "");
  testParse("\"\\001a\"",
            "<feature_template><string_literal>\1a</string_literal></feature_template>",
            "");
  testParse("\"\\010\"",
            "<feature_template><string_literal>\10</string_literal></feature_template>",
            "");
  testParse("\"\\010a\"",
            "<feature_template><string_literal>\10a</string_literal></feature_template>",
            "");
  testParse("\"\\011\"",
            "<feature_template><string_literal>\11</string_literal></feature_template>",
            "");
  testParse("\"\\011a\"",
            "<feature_template><string_literal>\11a</string_literal></feature_template>",
            "");
  testParse("\"\\100\"",
            "<feature_template><string_literal>\100</string_literal></feature_template>",
            "");
  testParse("\"\\100a\"",
            "<feature_template><string_literal>\100a</string_literal></feature_template>",
            "");
  testParse("\"\\101\"",
            "<feature_template><string_literal>\101</string_literal></feature_template>",
            "");
  testParse("\"\\101a\"",
            "<feature_template><string_literal>\101a</string_literal></feature_template>",
            "");
  testParse("\"\\110\"",
            "<feature_template><string_literal>\110</string_literal></feature_template>",
            "");
  testParse("\"\\110a\"",
            "<feature_template><string_literal>\110a</string_literal></feature_template>",
            "");
  testParse("\"\\111\"",
            "<feature_template><string_literal>\111</string_literal></feature_template>",
            "");
  testParse("\"\\0000\"",
            "<feature_template><string_literal>\00</string_literal></feature_template>",
            "");
}
