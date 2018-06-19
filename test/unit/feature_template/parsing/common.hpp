#if !defined(ENEK_TEST_UNIT_FEATURE_TEMPLATE_PARSING_COMMON_HPP_INCLUDE_GUARD)
#define ENEK_TEST_UNIT_FEATURE_TEMPLATE_PARSING_COMMON_HPP_INCLUDE_GUARD

#include <enek/feature_template/parsing/ast.hpp>
#include <string>


Enek::FeatureTemplate::Parsing::AST testParse(
  std::string const &text_to_be_parsed,
  std::string const &dump_to_be_expected,
  std::string const &error_message_to_be_expected);

#endif // !defined(ENEK_TEST_UNIT_FEATURE_TEMPLATE_PARSING_COMMON_HPP_INCLUDE_GUARD)
