#include <common.hpp>
#include <enek/feature_template/parsing/string_literal.hpp>
#include <gtest/gtest.h>
#include <boost/exception/exception.hpp>
#include <ostream>
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
  {
    std::ostringstream oss;
    EXPECT_THROW(sl.dumpXML(oss), std::invalid_argument);
    EXPECT_THROW(sl.dumpXML(oss), boost::exception);
  }
}

TEST(FeatureTemplateParsingStringLiteralTest, testParse)
{
  testParse("\"\"",
            "<feature_template type=\"string\"><string_literal></string_literal></feature_template>",
            "");
  testParse("\"a\"",
            "<feature_template type=\"string\"><string_literal>a</string_literal></feature_template>",
            "");
}

TEST(FeatureTemplateParsingStringLiteralTest, testParseSimpleEscapeSequence)
{
  testParse("\"\\'\"",
            "<feature_template type=\"string\"><string_literal>'</string_literal></feature_template>",
            "");
  testParse("\"\\\"\"",
            "<feature_template type=\"string\"><string_literal>\"</string_literal></feature_template>",
            "");
  testParse("\"\\?\"",
            "<feature_template type=\"string\"><string_literal>?</string_literal></feature_template>",
            "");
  testParse("\"\\\\\"",
            "<feature_template type=\"string\"><string_literal>\\</string_literal></feature_template>",
            "");
  testParse("\"\\a\"",
            "<feature_template type=\"string\"><string_literal>\a</string_literal></feature_template>",
            "");
  testParse("\"\\b\"",
            "<feature_template type=\"string\"><string_literal>\b</string_literal></feature_template>",
            "");
  testParse("\"\\f\"",
            "<feature_template type=\"string\"><string_literal>\f</string_literal></feature_template>",
            "");
  testParse("\"\\n\"",
            "<feature_template type=\"string\"><string_literal>\n</string_literal></feature_template>",
            "");
  testParse("\"\\r\"",
            "<feature_template type=\"string\"><string_literal>\r</string_literal></feature_template>",
            "");
  testParse("\"\\t\"",
            "<feature_template type=\"string\"><string_literal>\t</string_literal></feature_template>",
            "");
  testParse("\"\\v\"",
            "<feature_template type=\"string\"><string_literal>\v</string_literal></feature_template>",
            "");
}

TEST(FeatureTemplateParsingStringLiteralTest, testParseOctalEscapeSequence)
{
  testParse("\"\\0\"",
            "<feature_template type=\"string\"><string_literal>\0</string_literal></feature_template>",
            "");
  testParse("\"\\0a\"",
            "<feature_template type=\"string\"><string_literal>\0a</string_literal></feature_template>",
            "");
  testParse("\"\\1\"",
            "<feature_template type=\"string\"><string_literal>\1</string_literal></feature_template>",
            "");
  testParse("\"\\1a\"",
            "<feature_template type=\"string\"><string_literal>\1a</string_literal></feature_template>",
            "");
  testParse("\"\\00\"",
            "<feature_template type=\"string\"><string_literal>\0</string_literal></feature_template>",
            "");
  testParse("\"\\00a\"",
            "<feature_template type=\"string\"><string_literal>\0a</string_literal></feature_template>",
            "");
  testParse("\"\\01\"",
            "<feature_template type=\"string\"><string_literal>\1</string_literal></feature_template>",
            "");
  testParse("\"\\01a\"",
            "<feature_template type=\"string\"><string_literal>\1a</string_literal></feature_template>",
            "");
  testParse("\"\\10\"",
            "<feature_template type=\"string\"><string_literal>\10</string_literal></feature_template>",
            "");
  testParse("\"\\10a\"",
            "<feature_template type=\"string\"><string_literal>\10a</string_literal></feature_template>",
            "");
  testParse("\"\\11\"",
            "<feature_template type=\"string\"><string_literal>\11</string_literal></feature_template>",
            "");
  testParse("\"\\11a\"",
            "<feature_template type=\"string\"><string_literal>\11a</string_literal></feature_template>",
            "");
  testParse("\"\\000\"",
            "<feature_template type=\"string\"><string_literal>\0</string_literal></feature_template>",
            "");
  testParse("\"\\000a\"",
            "<feature_template type=\"string\"><string_literal>\0a</string_literal></feature_template>",
            "");
  testParse("\"\\001\"",
            "<feature_template type=\"string\"><string_literal>\1</string_literal></feature_template>",
            "");
  testParse("\"\\001a\"",
            "<feature_template type=\"string\"><string_literal>\1a</string_literal></feature_template>",
            "");
  testParse("\"\\010\"",
            "<feature_template type=\"string\"><string_literal>\10</string_literal></feature_template>",
            "");
  testParse("\"\\010a\"",
            "<feature_template type=\"string\"><string_literal>\10a</string_literal></feature_template>",
            "");
  testParse("\"\\011\"",
            "<feature_template type=\"string\"><string_literal>\11</string_literal></feature_template>",
            "");
  testParse("\"\\011a\"",
            "<feature_template type=\"string\"><string_literal>\11a</string_literal></feature_template>",
            "");
  testParse("\"\\100\"",
            "<feature_template type=\"string\"><string_literal>\100</string_literal></feature_template>",
            "");
  testParse("\"\\100a\"",
            "<feature_template type=\"string\"><string_literal>\100a</string_literal></feature_template>",
            "");
  testParse("\"\\101\"",
            "<feature_template type=\"string\"><string_literal>\101</string_literal></feature_template>",
            "");
  testParse("\"\\101a\"",
            "<feature_template type=\"string\"><string_literal>\101a</string_literal></feature_template>",
            "");
  testParse("\"\\110\"",
            "<feature_template type=\"string\"><string_literal>\110</string_literal></feature_template>",
            "");
  testParse("\"\\110a\"",
            "<feature_template type=\"string\"><string_literal>\110a</string_literal></feature_template>",
            "");
  testParse("\"\\111\"",
            "<feature_template type=\"string\"><string_literal>\111</string_literal></feature_template>",
            "");
  testParse("\"\\111a\"",
            "<feature_template type=\"string\"><string_literal>\111a</string_literal></feature_template>",
            "");
  testParse("\"\\0000\"",
            "<feature_template type=\"string\"><string_literal>\00</string_literal></feature_template>",
            "");
}


TEST(FeatureTemplateParsingStringLiteralTest, testParseHexadecimalEscapeSequence)
{
  testParse("\"g\\x0g\"",
            "<feature_template type=\"string\"><string_literal>g\x0g</string_literal></feature_template>",
            "");
  testParse("\"g\\x1g\"",
            "<feature_template type=\"string\"><string_literal>g\x1g</string_literal></feature_template>",
            "");
  testParse("\"g\\x2g\"",
            "<feature_template type=\"string\"><string_literal>g\x2g</string_literal></feature_template>",
            "");
  testParse("\"g\\x3g\"",
            "<feature_template type=\"string\"><string_literal>g\x3g</string_literal></feature_template>",
            "");
  testParse("\"g\\x4g\"",
            "<feature_template type=\"string\"><string_literal>g\x4g</string_literal></feature_template>",
            "");
  testParse("\"g\\x5g\"",
            "<feature_template type=\"string\"><string_literal>g\x5g</string_literal></feature_template>",
            "");
  testParse("\"g\\x6g\"",
            "<feature_template type=\"string\"><string_literal>g\x6g</string_literal></feature_template>",
            "");
  testParse("\"g\\x7g\"",
            "<feature_template type=\"string\"><string_literal>g\x7g</string_literal></feature_template>",
            "");
  testParse("\"g\\x8g\"",
            "<feature_template type=\"string\"><string_literal>g\x8g</string_literal></feature_template>",
            "");
  testParse("\"g\\x9g\"",
            "<feature_template type=\"string\"><string_literal>g\x9g</string_literal></feature_template>",
            "");
  testParse("\"g\\xag\"",
            "<feature_template type=\"string\"><string_literal>g\xag</string_literal></feature_template>",
            "");
  testParse("\"g\\xbg\"",
            "<feature_template type=\"string\"><string_literal>g\xbg</string_literal></feature_template>",
            "");
  testParse("\"g\\xcg\"",
            "<feature_template type=\"string\"><string_literal>g\xcg</string_literal></feature_template>",
            "");
  testParse("\"g\\xdg\"",
            "<feature_template type=\"string\"><string_literal>g\xdg</string_literal></feature_template>",
            "");
  testParse("\"g\\xeg\"",
            "<feature_template type=\"string\"><string_literal>g\xeg</string_literal></feature_template>",
            "");
  testParse("\"g\\xfg\"",
            "<feature_template type=\"string\"><string_literal>g\xfg</string_literal></feature_template>",
            "");
  testParse("\"g\\xAg\"",
            "<feature_template type=\"string\"><string_literal>g\xAg</string_literal></feature_template>",
            "");
  testParse("\"g\\xBg\"",
            "<feature_template type=\"string\"><string_literal>g\xBg</string_literal></feature_template>",
            "");
  testParse("\"g\\xCg\"",
            "<feature_template type=\"string\"><string_literal>g\xCg</string_literal></feature_template>",
            "");
  testParse("\"g\\xDg\"",
            "<feature_template type=\"string\"><string_literal>g\xDg</string_literal></feature_template>",
            "");
  testParse("\"g\\xEg\"",
            "<feature_template type=\"string\"><string_literal>g\xEg</string_literal></feature_template>",
            "");
  testParse("\"g\\xFg\"",
            "<feature_template type=\"string\"><string_literal>g\xFg</string_literal></feature_template>",
            "");
  testParse("\"g\\x00g\"",
            "<feature_template type=\"string\"><string_literal>g\x0g</string_literal></feature_template>",
            "");
  testParse("\"g\\x01g\"",
            "<feature_template type=\"string\"><string_literal>g\x01g</string_literal></feature_template>",
            "");
  testParse("\"g\\x10g\"",
            "<feature_template type=\"string\"><string_literal>g\x10g</string_literal></feature_template>",
            "");
  testParse("\"g\\x11g\"",
            "<feature_template type=\"string\"><string_literal>g\x11g</string_literal></feature_template>",
            "");
  testParse("\"g\\x10FFFFg\"",
            "<feature_template type=\"string\"><string_literal>g\U0010FFFFg</string_literal></feature_template>",
            "");
  testParse("\"g\\x110000g\"",
            "<feature_template succeed=\"false\" type=\"string\"><string_literal succeed=\"false\"/></feature_template>",
            "1:3: error: A hexadecimal escape sequence exceeds the highest Unicode code point value.\n"
            "\"g\\x110000g\"\n"
            "  ^^^^^^^^\n");
  testParse("\"g\\x10000000000000000g\"",
            "<feature_template succeed=\"false\" type=\"string\"><string_literal succeed=\"false\"/></feature_template>",
            "1:3: error: A hexadecimal escape sequence is too large.\n"
            "\"g\\x10000000000000000g\"\n"
            "  ^^^^^^^^^^^^^^^^^^^\n");
  testParse("\"g\\xD800g\"",
            "<feature_template succeed=\"false\" type=\"string\"><string_literal succeed=\"false\"/></feature_template>",
            "1:3: error: A hexadecimal escape sequence is a Unicode surrogate code point.\n"
            "\"g\\xD800g\"\n"
            "  ^^^^^^\n");
}

TEST(FeatureTemplateParsingStringLiteralTest, testParseUniversalCharacterName)
{
  testParse("\"g\\u0020g\"",
            "<feature_template type=\"string\"><string_literal>g g</string_literal></feature_template>",
            "");
  testParse("\"g\\uD800g\"",
            "<feature_template succeed=\"false\" type=\"string\"><string_literal succeed=\"false\"/></feature_template>",
            "1:3: error: A universal character name is a Unicode surrogate code point.\n"
            "\"g\\uD800g\"\n"
            "  ^^^^^^\n");
  testParse("\"g\\U00000020g\"",
            "<feature_template type=\"string\"><string_literal>g g</string_literal></feature_template>",
            "");
  testParse("\"g\\U0010FFFFg\"",
            "<feature_template type=\"string\"><string_literal>g\U0010FFFFg</string_literal></feature_template>",
            "");
  testParse("\"g\\U00110000g\"",
            "<feature_template succeed=\"false\" type=\"string\"><string_literal succeed=\"false\"/></feature_template>",
            "1:3: error: A universal character name exceeds the highest Unicode code point value.\n"
            "\"g\\U00110000g\"\n"
            "  ^^^^^^^^^^\n");
}
