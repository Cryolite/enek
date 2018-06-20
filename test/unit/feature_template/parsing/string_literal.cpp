#include <common.hpp>
#include <enek/feature_template/parsing/string_literal.hpp>
#include <gtest/gtest.h>
#include <boost/exception/exception.hpp>
#include <stdexcept>


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
            "<string_literal></string_literal>",
            "");
  testParse("\"a\"",
            "<string_literal>a</string_literal>",
            "");
}

TEST(FeatureTemplateParsingStringLiteralTest, testParseSimpleEscapeSequence)
{
  testParse("\"\\'\"",
            "<string_literal>'</string_literal>",
            "");
  testParse("\"\\\"\"",
            "<string_literal>\"</string_literal>",
            "");
  testParse("\"\\?\"",
            "<string_literal>?</string_literal>",
            "");
  testParse("\"\\\\\"",
            "<string_literal>\\</string_literal>",
            "");
  testParse("\"\\a\"",
            "<string_literal>\a</string_literal>",
            "");
  testParse("\"\\b\"",
            "<string_literal>\b</string_literal>",
            "");
  testParse("\"\\f\"",
            "<string_literal>\f</string_literal>",
            "");
  testParse("\"\\n\"",
            "<string_literal>\n</string_literal>",
            "");
  testParse("\"\\r\"",
            "<string_literal>\r</string_literal>",
            "");
  testParse("\"\\t\"",
            "<string_literal>\t</string_literal>",
            "");
  testParse("\"\\v\"",
            "<string_literal>\v</string_literal>",
            "");
}

TEST(FeatureTemplateParsingStringLiteralTest, testParseOctalEscapeSequence)
{
  testParse("\"\\0\"",
            "<string_literal>\0</string_literal>",
            "");
  testParse("\"\\0a\"",
            "<string_literal>\0a</string_literal>",
            "");
  testParse("\"\\1\"",
            "<string_literal>\1</string_literal>",
            "");
  testParse("\"\\1a\"",
            "<string_literal>\1a</string_literal>",
            "");
  testParse("\"\\00\"",
            "<string_literal>\0</string_literal>",
            "");
  testParse("\"\\00a\"",
            "<string_literal>\0a</string_literal>",
            "");
  testParse("\"\\01\"",
            "<string_literal>\1</string_literal>",
            "");
  testParse("\"\\01a\"",
            "<string_literal>\1a</string_literal>",
            "");
  testParse("\"\\10\"",
            "<string_literal>\10</string_literal>",
            "");
  testParse("\"\\10a\"",
            "<string_literal>\10a</string_literal>",
            "");
  testParse("\"\\11\"",
            "<string_literal>\11</string_literal>",
            "");
  testParse("\"\\11a\"",
            "<string_literal>\11a</string_literal>",
            "");
  testParse("\"\\000\"",
            "<string_literal>\0</string_literal>",
            "");
  testParse("\"\\000a\"",
            "<string_literal>\0a</string_literal>",
            "");
  testParse("\"\\001\"",
            "<string_literal>\1</string_literal>",
            "");
  testParse("\"\\001a\"",
            "<string_literal>\1a</string_literal>",
            "");
  testParse("\"\\010\"",
            "<string_literal>\10</string_literal>",
            "");
  testParse("\"\\010a\"",
            "<string_literal>\10a</string_literal>",
            "");
  testParse("\"\\011\"",
            "<string_literal>\11</string_literal>",
            "");
  testParse("\"\\011a\"",
            "<string_literal>\11a</string_literal>",
            "");
  testParse("\"\\100\"",
            "<string_literal>\100</string_literal>",
            "");
  testParse("\"\\100a\"",
            "<string_literal>\100a</string_literal>",
            "");
  testParse("\"\\101\"",
            "<string_literal>\101</string_literal>",
            "");
  testParse("\"\\101a\"",
            "<string_literal>\101a</string_literal>",
            "");
  testParse("\"\\110\"",
            "<string_literal>\110</string_literal>",
            "");
  testParse("\"\\110a\"",
            "<string_literal>\110a</string_literal>",
            "");
  testParse("\"\\111\"",
            "<string_literal>\111</string_literal>",
            "");
  testParse("\"\\111a\"",
            "<string_literal>\111a</string_literal>",
            "");
  testParse("\"\\0000\"",
            "<string_literal>\00</string_literal>",
            "");
}


TEST(FeatureTemplateParsingStringLiteralTest, testParseHexadecimalEscapeSequence)
{
  testParse("\"g\\x0g\"",
            "<string_literal>g\x0g</string_literal>",
            "");
  testParse("\"g\\x1g\"",
            "<string_literal>g\x1g</string_literal>",
            "");
  testParse("\"g\\x2g\"",
            "<string_literal>g\x2g</string_literal>",
            "");
  testParse("\"g\\x3g\"",
            "<string_literal>g\x3g</string_literal>",
            "");
  testParse("\"g\\x4g\"",
            "<string_literal>g\x4g</string_literal>",
            "");
  testParse("\"g\\x5g\"",
            "<string_literal>g\x5g</string_literal>",
            "");
  testParse("\"g\\x6g\"",
            "<string_literal>g\x6g</string_literal>",
            "");
  testParse("\"g\\x7g\"",
            "<string_literal>g\x7g</string_literal>",
            "");
  testParse("\"g\\x8g\"",
            "<string_literal>g\x8g</string_literal>",
            "");
  testParse("\"g\\x9g\"",
            "<string_literal>g\x9g</string_literal>",
            "");
  testParse("\"g\\xag\"",
            "<string_literal>g\xag</string_literal>",
            "");
  testParse("\"g\\xbg\"",
            "<string_literal>g\xbg</string_literal>",
            "");
  testParse("\"g\\xcg\"",
            "<string_literal>g\xcg</string_literal>",
            "");
  testParse("\"g\\xdg\"",
            "<string_literal>g\xdg</string_literal>",
            "");
  testParse("\"g\\xeg\"",
            "<string_literal>g\xeg</string_literal>",
            "");
  testParse("\"g\\xfg\"",
            "<string_literal>g\xfg</string_literal>",
            "");
  testParse("\"g\\xAg\"",
            "<string_literal>g\xAg</string_literal>",
            "");
  testParse("\"g\\xBg\"",
            "<string_literal>g\xBg</string_literal>",
            "");
  testParse("\"g\\xCg\"",
            "<string_literal>g\xCg</string_literal>",
            "");
  testParse("\"g\\xDg\"",
            "<string_literal>g\xDg</string_literal>",
            "");
  testParse("\"g\\xEg\"",
            "<string_literal>g\xEg</string_literal>",
            "");
  testParse("\"g\\xFg\"",
            "<string_literal>g\xFg</string_literal>",
            "");
  testParse("\"g\\x00g\"",
            "<string_literal>g\x0g</string_literal>",
            "");
  testParse("\"g\\x01g\"",
            "<string_literal>g\x01g</string_literal>",
            "");
  testParse("\"g\\x10g\"",
            "<string_literal>g\x10g</string_literal>",
            "");
  testParse("\"g\\x11g\"",
            "<string_literal>g\x11g</string_literal>",
            "");
  testParse("\"g\\x10FFFFg\"",
            "<string_literal>g\U0010FFFFg</string_literal>",
            "");
  testParse("\"g\\x110000g\"",
            "<string_literal succeed=\"false\"/>",
            "1:3: error: A hexadecimal escape sequence exceeds the highest Unicode code point value.\n"
            "\"g\\x110000g\"\n"
            "  ^^^^^^^^\n");
  testParse("\"g\\x10000000000000000g\"",
            "<string_literal succeed=\"false\"/>",
            "1:3: error: A hexadecimal escape sequence is too large.\n"
            "\"g\\x10000000000000000g\"\n"
            "  ^^^^^^^^^^^^^^^^^^^\n");
  testParse("\"g\\xD800g\"",
            "<string_literal succeed=\"false\"/>",
            "1:3: error: A hexadecimal escape sequence is a Unicode surrogate code point.\n"
            "\"g\\xD800g\"\n"
            "  ^^^^^^\n");
}

TEST(FeatureTemplateParsingStringLiteralTest, testParseUniversalCharacterName)
{
  testParse("\"g\\u0020g\"",
            "<string_literal>g g</string_literal>",
            "");
  testParse("\"g\\uD800g\"",
            "<string_literal succeed=\"false\"/>",
            "1:3: error: A universal character name is a Unicode surrogate code point.\n"
            "\"g\\uD800g\"\n"
            "  ^^^^^^\n");
  testParse("\"g\\U00000020g\"",
            "<string_literal>g g</string_literal>",
            "");
  testParse("\"g\\U0010FFFFg\"",
            "<string_literal>g\U0010FFFFg</string_literal>",
            "");
  testParse("\"g\\U00110000g\"",
            "<string_literal succeed=\"false\"/>",
            "1:3: error: A universal character name exceeds the highest Unicode code point value.\n"
            "\"g\\U00110000g\"\n"
            "  ^^^^^^^^^^\n");
}
