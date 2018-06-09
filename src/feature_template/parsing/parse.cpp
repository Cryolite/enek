#include <enek/feature_template/parsing/parse.hpp>

#include <enek/feature_template/parsing/grammar.hpp>
#include <enek/feature_template/parsing/ast.hpp>
#include <enek/feature_template/parsing/text_position_iterator.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/range/iterator_range.hpp>
#include <filesystem>
#include <ostream>
#include <string>
#include <utility>


namespace Enek::FeatureTemplate::Parsing{

using Path = std::filesystem::path;

Enek::FeatureTemplate::Parsing::AST
parse(Path const &path, std::string const &text, std::ostream &os)
{
  namespace qi = boost::spirit::qi;
  namespace Parsing = Enek::FeatureTemplate::Parsing;
  using BaseIterator = std::string::const_iterator;
  BaseIterator const first = text.cbegin();
  BaseIterator const last = text.cend();
  using BaseIteratorRange = boost::iterator_range<BaseIterator>;
  BaseIteratorRange const text_range(first, last);
  using Iterator = Parsing::TextPositionIterator<BaseIterator>;
  Parsing::Grammar<Iterator> grammar(path, text_range, os);
  Parsing::SkipGrammar<Iterator> skip_grammar;
  Iterator first_, last_;
  std::tie(first_, last_) = Iterator::makeIteratorRange(first, last);
  Parsing::AST ast;
  qi::phrase_parse(first_, std::move(last_), grammar, skip_grammar, ast);
  return ast;
}

Enek::FeatureTemplate::Parsing::AST
parse(std::string const &text, std::ostream &os)
{
  return Enek::FeatureTemplate::Parsing::parse(Path(), text, os);
}

} // namespace Enek::FeatureTemplate::Parsing
