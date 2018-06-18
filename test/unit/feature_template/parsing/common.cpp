#include "common.hpp"
#include <enek/feature_template/parsing/parse.hpp>
#include <enek/feature_template/input_type.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include <string>


void testParse(
  std::string const &text_to_be_parsed,
  std::string const &dump_to_be_expected,
  std::string const &error_message_to_be_expected)
{
  namespace Parsing = Enek::FeatureTemplate::Parsing;
  std::ostringstream error_oss;
  Parsing::AST ast = Parsing::parse(
    Enek::FeatureTemplate::InputType::word, text_to_be_parsed, error_oss);
  std::ostringstream dump_oss;
  ast.dumpXML(dump_oss);
  ASSERT_STREQ(dump_oss.str().c_str(), dump_to_be_expected.c_str())
    << "parsed text: " << text_to_be_parsed;
  ASSERT_STREQ(error_oss.str().c_str(), error_message_to_be_expected.c_str())
    << "parsed text: " << text_to_be_parsed;
}
