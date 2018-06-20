#include <common.hpp>
#include <enek/feature_template/parsing/string_literal.hpp>
#include <gtest/gtest.h>
#include <boost/exception/exception.hpp>
#include <stdexcept>


using Type = Enek::FeatureTemplate::Type;
using StringLiteral = Enek::FeatureTemplate::Parsing::StringLiteral;

TEST(FeatureTemplateParsingStringLiteralTest, testDefaultConstructor)
{
  StringLiteral sl;
  EXPECT_FALSE(sl.isInitialized());
  EXPECT_THROW(sl.succeed(), std::invalid_argument);
  EXPECT_THROW(sl.succeed(), boost::exception);
  EXPECT_THROW(sl.getType(), std::invalid_argument);
  EXPECT_THROW(sl.getType(), boost::exception);
  EXPECT_THROW(sl.getValue(), std::invalid_argument);
  EXPECT_THROW(sl.getValue(), boost::exception);
}

TEST(FeatureTemplateParsingStringLiteralTest, testParse)
{
  testParse("\"\"",
            true,
            Type::string,
            "<string_literal></string_literal>",
            "");
  testParse("\"a\"",
            true,
            Type::string,
            "<string_literal>a</string_literal>",
            "");
}

TEST(FeatureTemplateParsingStringLiteralTest, testParseSimpleEscapeSequence)
{
  testParse("\"\\'\"",
            true,
            Type::string,
            "<string_literal>'</string_literal>",
            "");
  testParse("\"\\\"\"",
            true,
            Type::string,
            "<string_literal>\"</string_literal>",
            "");
  testParse("\"\\?\"",
            true,
            Type::string,
            "<string_literal>?</string_literal>",
            "");
  testParse("\"\\\\\"",
            true,
            Type::string,
            "<string_literal>\\</string_literal>",
            "");
  testParse("\"\\a\"",
            true,
            Type::string,
            "<string_literal>\a</string_literal>",
            "");
  testParse("\"\\b\"",
            true,
            Type::string,
            "<string_literal>\b</string_literal>",
            "");
  testParse("\"\\f\"",
            true,
            Type::string,
            "<string_literal>\f</string_literal>",
            "");
  testParse("\"\\n\"",
            true,
            Type::string,
            "<string_literal>\n</string_literal>",
            "");
  testParse("\"\\r\"",
            true,
            Type::string,
            "<string_literal>\r</string_literal>",
            "");
  testParse("\"\\t\"",
            true,
            Type::string,
            "<string_literal>\t</string_literal>",
            "");
  testParse("\"\\v\"",
            true,
            Type::string,
            "<string_literal>\v</string_literal>",
            "");
}

TEST(FeatureTemplateParsingStringLiteralTest, testParseOctalEscapeSequence)
{
  testParse("\"\\0\"",
            true,
            Type::string,
            "<string_literal>\0</string_literal>",
            "");
  testParse("\"\\0a\"",
            true,
            Type::string,
            "<string_literal>\0a</string_literal>",
            "");
  testParse("\"\\1\"",
            true,
            Type::string,
            "<string_literal>\1</string_literal>",
            "");
  testParse("\"\\1a\"",
            true,
            Type::string,
            "<string_literal>\1a</string_literal>",
            "");
  testParse("\"\\00\"",
            true,
            Type::string,
            "<string_literal>\0</string_literal>",
            "");
  testParse("\"\\00a\"",
            true,
            Type::string,
            "<string_literal>\0a</string_literal>",
            "");
  testParse("\"\\01\"",
            true,
            Type::string,
            "<string_literal>\1</string_literal>",
            "");
  testParse("\"\\01a\"",
            true,
            Type::string,
            "<string_literal>\1a</string_literal>",
            "");
  testParse("\"\\10\"",
            true,
            Type::string,
            "<string_literal>\10</string_literal>",
            "");
  testParse("\"\\10a\"",
            true,
            Type::string,
            "<string_literal>\10a</string_literal>",
            "");
  testParse("\"\\11\"",
            true,
            Type::string,
            "<string_literal>\11</string_literal>",
            "");
  testParse("\"\\11a\"",
            true,
            Type::string,
            "<string_literal>\11a</string_literal>",
            "");
  testParse("\"\\000\"",
            true,
            Type::string,
            "<string_literal>\0</string_literal>",
            "");
  testParse("\"\\000a\"",
            true,
            Type::string,
            "<string_literal>\0a</string_literal>",
            "");
  testParse("\"\\001\"",
            true,
            Type::string,
            "<string_literal>\1</string_literal>",
            "");
  testParse("\"\\001a\"",
            true,
            Type::string,
            "<string_literal>\1a</string_literal>",
            "");
  testParse("\"\\010\"",
            true,
            Type::string,
            "<string_literal>\10</string_literal>",
            "");
  testParse("\"\\010a\"",
            true,
            Type::string,
            "<string_literal>\10a</string_literal>",
            "");
  testParse("\"\\011\"",
            true,
            Type::string,
            "<string_literal>\11</string_literal>",
            "");
  testParse("\"\\011a\"",
            true,
            Type::string,
            "<string_literal>\11a</string_literal>",
            "");
  testParse("\"\\100\"",
            true,
            Type::string,
            "<string_literal>\100</string_literal>",
            "");
  testParse("\"\\100a\"",
            true,
            Type::string,
            "<string_literal>\100a</string_literal>",
            "");
  testParse("\"\\101\"",
            true,
            Type::string,
            "<string_literal>\101</string_literal>",
            "");
  testParse("\"\\101a\"",
            true,
            Type::string,
            "<string_literal>\101a</string_literal>",
            "");
  testParse("\"\\110\"",
            true,
            Type::string,
            "<string_literal>\110</string_literal>",
            "");
  testParse("\"\\110a\"",
            true,
            Type::string,
            "<string_literal>\110a</string_literal>",
            "");
  testParse("\"\\111\"",
            true,
            Type::string,
            "<string_literal>\111</string_literal>",
            "");
  testParse("\"\\111a\"",
            true,
            Type::string,
            "<string_literal>\111a</string_literal>",
            "");
  testParse("\"\\0000\"",
            true,
            Type::string,
            "<string_literal>\00</string_literal>",
            "");
}


TEST(FeatureTemplateParsingStringLiteralTest, testParseHexadecimalEscapeSequence)
{
  testParse("\"g\\x0g\"",
            true,
            Type::string,
            "<string_literal>g\x0g</string_literal>",
            "");
  testParse("\"g\\x1g\"",
            true,
            Type::string,
            "<string_literal>g\x1g</string_literal>",
            "");
  testParse("\"g\\x2g\"",
            true,
            Type::string,
            "<string_literal>g\x2g</string_literal>",
            "");
  testParse("\"g\\x3g\"",
            true,
            Type::string,
            "<string_literal>g\x3g</string_literal>",
            "");
  testParse("\"g\\x4g\"",
            true,
            Type::string,
            "<string_literal>g\x4g</string_literal>",
            "");
  testParse("\"g\\x5g\"",
            true,
            Type::string,
            "<string_literal>g\x5g</string_literal>",
            "");
  testParse("\"g\\x6g\"",
            true,
            Type::string,
            "<string_literal>g\x6g</string_literal>",
            "");
  testParse("\"g\\x7g\"",
            true,
            Type::string,
            "<string_literal>g\x7g</string_literal>",
            "");
  testParse("\"g\\x8g\"",
            true,
            Type::string,
            "<string_literal>g\x8g</string_literal>",
            "");
  testParse("\"g\\x9g\"",
            true,
            Type::string,
            "<string_literal>g\x9g</string_literal>",
            "");
  testParse("\"g\\xag\"",
            true,
            Type::string,
            "<string_literal>g\xag</string_literal>",
            "");
  testParse("\"g\\xbg\"",
            true,
            Type::string,
            "<string_literal>g\xbg</string_literal>",
            "");
  testParse("\"g\\xcg\"",
            true,
            Type::string,
            "<string_literal>g\xcg</string_literal>",
            "");
  testParse("\"g\\xdg\"",
            true,
            Type::string,
            "<string_literal>g\xdg</string_literal>",
            "");
  testParse("\"g\\xeg\"",
            true,
            Type::string,
            "<string_literal>g\xeg</string_literal>",
            "");
  testParse("\"g\\xfg\"",
            true,
            Type::string,
            "<string_literal>g\xfg</string_literal>",
            "");
  testParse("\"g\\xAg\"",
            true,
            Type::string,
            "<string_literal>g\xAg</string_literal>",
            "");
  testParse("\"g\\xBg\"",
            true,
            Type::string,
            "<string_literal>g\xBg</string_literal>",
            "");
  testParse("\"g\\xCg\"",
            true,
            Type::string,
            "<string_literal>g\xCg</string_literal>",
            "");
  testParse("\"g\\xDg\"",
            true,
            Type::string,
            "<string_literal>g\xDg</string_literal>",
            "");
  testParse("\"g\\xEg\"",
            true,
            Type::string,
            "<string_literal>g\xEg</string_literal>",
            "");
  testParse("\"g\\xFg\"",
            true,
            Type::string,
            "<string_literal>g\xFg</string_literal>",
            "");
  testParse("\"g\\x00g\"",
            true,
            Type::string,
            "<string_literal>g\x0g</string_literal>",
            "");
  testParse("\"g\\x01g\"",
            true,
            Type::string,
            "<string_literal>g\x01g</string_literal>",
            "");
  testParse("\"g\\x10g\"",
            true,
            Type::string,
            "<string_literal>g\x10g</string_literal>",
            "");
  testParse("\"g\\x11g\"",
            true,
            Type::string,
            "<string_literal>g\x11g</string_literal>",
            "");
  testParse("\"g\\x10FFFFg\"",
            true,
            Type::string,
            "<string_literal>g\U0010FFFFg</string_literal>",
            "");
  testParse("\"g\\x110000g\"",
            false,
            Type::string,
            "<string_literal succeed=\"false\"/>",
            "1:3: error: A hexadecimal escape sequence exceeds the highest Unicode code point value.\n"
            "\"g\\x110000g\"\n"
            "  ^^^^^^^^\n");
  testParse("\"g\\x10000000000000000g\"",
            false,
            Type::string,
            "<string_literal succeed=\"false\"/>",
            "1:3: error: A hexadecimal escape sequence is too large.\n"
            "\"g\\x10000000000000000g\"\n"
            "  ^^^^^^^^^^^^^^^^^^^\n");
  testParse("\"g\\xD800g\"",
            false,
            Type::string,
            "<string_literal succeed=\"false\"/>",
            "1:3: error: A hexadecimal escape sequence is a Unicode surrogate code point.\n"
            "\"g\\xD800g\"\n"
            "  ^^^^^^\n");
}

TEST(FeatureTemplateParsingStringLiteralTest, testParseUniversalCharacterName)
{
  testParse("\"g\\u0020g\"",
            true,
            Type::string,
            "<string_literal>g g</string_literal>",
            "");
  testParse("\"g\\uD800g\"",
            false,
            Type::string,
            "<string_literal succeed=\"false\"/>",
            "1:3: error: A universal character name is a Unicode surrogate code point.\n"
            "\"g\\uD800g\"\n"
            "  ^^^^^^\n");
  testParse("\"g\\U00000020g\"",
            true,
            Type::string,
            "<string_literal>g g</string_literal>",
            "");
  testParse("\"g\\U0010FFFFg\"",
            true,
            Type::string,
            "<string_literal>g\U0010FFFFg</string_literal>",
            "");
  testParse("\"g\\U00110000g\"",
            false,
            Type::string,
            "<string_literal succeed=\"false\"/>",
            "1:3: error: A universal character name exceeds the highest Unicode code point value.\n"
            "\"g\\U00110000g\"\n"
            "  ^^^^^^^^^^\n");
}
