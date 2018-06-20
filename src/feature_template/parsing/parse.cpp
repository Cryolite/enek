#include <enek/feature_template/parsing/parse.hpp>

#include <enek/feature_template/parsing/grammar.hpp>
#include <enek/feature_template/parsing/ast.hpp>
#include <enek/feature_template/parsing/text_position_iterator.hpp>
#include <enek/feature_template/parsing/placeholder_type.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/end.hpp>
#include <boost/range/begin.hpp>
#include <filesystem>
#include <ostream>
#include <string>
#include <utility>


namespace Enek::FeatureTemplate::Parsing{

using Path = std::filesystem::path;

Enek::FeatureTemplate::Parsing::AST
parse(Enek::FeatureTemplate::Parsing::PlaceholderType placeholder_type,
      Path const &path,
      std::string const &text,
      std::ostream &os)
{
  namespace qi = boost::spirit::qi;
  namespace Parsing = Enek::FeatureTemplate::Parsing;
  using BaseIterator = std::string::const_iterator;
  using BaseIteratorRange = boost::iterator_range<BaseIterator>;
  BaseIteratorRange const text_range(text.cbegin(), text.cend());
  using Iterator = Parsing::TextPositionIterator<BaseIterator>;
  using IteratorRange = boost::iterator_range<Iterator>;
  IteratorRange parse_range = Iterator::makeIteratorRange(text_range);
  Iterator first = boost::begin(parse_range);
  Iterator last = boost::end(parse_range);
  Parsing::Grammar<Iterator> grammar(placeholder_type, path, text_range, os);
  Parsing::SkipGrammar<Iterator> skip_grammar;
  Parsing::AST ast;
  qi::phrase_parse(first, std::move(last), grammar, skip_grammar, ast);
  return ast;
}

Enek::FeatureTemplate::Parsing::AST
parse(Enek::FeatureTemplate::Parsing::PlaceholderType placeholder_type,
      std::string const &text,
      std::ostream &os)
{
  return Enek::FeatureTemplate::Parsing::parse(placeholder_type, Path(), text, os);
}

} // namespace Enek::FeatureTemplate::Parsing
