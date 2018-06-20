#include "common.hpp"
#include <enek/feature_template/visitor/xml_dumper.hpp>
#include <enek/feature_template/parsing/parse.hpp>
#include <enek/feature_template/parsing/ast.hpp>
#include <enek/feature_template/parsing/placeholder_type.hpp>
#include <enek/feature_template/type.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include <string>


Enek::FeatureTemplate::Parsing::AST testParse(
  std::string const &text_to_be_parsed,
  bool succeed,
  Enek::FeatureTemplate::Type type,
  std::string const &dump_to_be_expected,
  std::string const &error_message_to_be_expected)
{
  namespace Parsing = Enek::FeatureTemplate::Parsing;
  std::ostringstream error_oss;
  Parsing::AST ast = Parsing::parse(Parsing::PlaceholderType::word,
                                    text_to_be_parsed,
                                    error_oss);
  EXPECT_EQ(succeed, ast.succeed())
    << "parsed text: " << text_to_be_parsed; // LCOV_EXCL_LINE;
  EXPECT_EQ(type, ast.getType());
  EXPECT_EQ(ast.succeed(), error_oss.str().empty());
  EXPECT_STREQ(error_oss.str().c_str(), error_message_to_be_expected.c_str())
    << "parsed text: " << text_to_be_parsed; // LCOV_EXCL_LINE
  {
    std::ostringstream dump_oss;
    Enek::FeatureTemplate::dumpXML(ast, dump_oss);
    EXPECT_STREQ(dump_oss.str().c_str(), dump_to_be_expected.c_str())
      << "parsed text: " << text_to_be_parsed; // LCOV_EXCL_LINE
  }
  return ast;
}
