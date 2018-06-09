#if !defined(ENEK_FEATURE_TEMPLATE_PARSING_PARSE_HPP_INCLUDE_GUARD)
#define ENEK_FEATURE_TEMPLATE_PARSING_PARSE_HPP_INCLUDE_GUARD

#include <enek/feature_template/parsing/ast.hpp>
#include <filesystem>
#include <ostream>
#include <string>


namespace Enek::FeatureTemplate::Parsing{

Enek::FeatureTemplate::Parsing::AST
parse(std::filesystem::path const &path,
      std::string const &text,
      std::ostream &os);

Enek::FeatureTemplate::Parsing::AST
parse(std::string const &text, std::ostream &os);

} // namespace Enek::FeatureTemplate::Parsing

#endif // !defined(ENEK_FEATURE_TEMPLATE_PARSING_PARSE_HPP_INCLUDE_GUARD)
