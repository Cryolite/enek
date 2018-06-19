#include "common.hpp"
#include <gtest/gtest.h>


TEST(FeatureTemplateTest, acceptStringLiteral)
{
  testParse("\"\"",
            "<feature_template type=\"string\"><string_literal></string_literal></feature_template>",
            "");
  testParse("\"a\"",
            "<feature_template type=\"string\"><string_literal>a</string_literal></feature_template>",
            "");
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
  testParse("\"\\0000\"",
            "<feature_template type=\"string\"><string_literal>\00</string_literal></feature_template>",
            "");
}
