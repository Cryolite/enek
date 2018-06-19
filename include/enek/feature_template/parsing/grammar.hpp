#if !defined(ENEK_FEATURE_TEMPLATE_PARSING_GRAMMAR_HPP_INCLUDE_GUARD)
#define ENEK_FEATURE_TEMPLATE_PARSING_GRAMMAR_HPP_INCLUDE_GUARD

#include <enek/feature_template/parsing/ast.hpp>
#include <enek/feature_template/parsing/string_literal.hpp>
#include <enek/feature_template/parsing/boolean_literal.hpp>
#include <enek/feature_template/parsing/floating_literal.hpp>
#include <enek/feature_template/parsing/integer_literal.hpp>
#include <enek/feature_template/parsing/input_type.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/bind/bind_member_function.hpp>
#include <boost/phoenix/bind/bind_function.hpp>
#include <boost/phoenix/core/reference.hpp>
#include <boost/range/iterator_range.hpp>
#include <filesystem>
#include <ostream>
#include <utility>


namespace Enek::FeatureTemplate::Parsing{

template<typename Iterator>
struct SkipGrammar
  : boost::spirit::qi::grammar<Iterator>
{
private:
  using Rule = boost::spirit::qi::rule<Iterator>;

public:
  SkipGrammar()
    : SkipGrammar::base_type(skip_, "skip")
  {
    namespace qi = boost::spirit::qi;

    single_line_comment_
      = "//" >> *(qi::char_ - qi::eol) >> (qi::eol | qi::eoi)
      ;
    block_comment_
      = "/*" >> *(block_comment_ | qi::char_ - "*/") > "*/"
      ;
    skip_
      = qi::space
      | single_line_comment_
      | block_comment_
      ;

    single_line_comment_.name("single-line-comment");
    block_comment_.name("block-comment");
    skip_.name("skip");
  }

private:
  Rule single_line_comment_;
  Rule block_comment_;
  Rule skip_;
}; // struct SkipGrammar

template<typename Iterator>
struct Grammar
  : boost::spirit::qi::grammar<Iterator, Enek::FeatureTemplate::Parsing::AST()>
{
private:
  using BaseIterator = typename Iterator::BaseIterator;

  using IntegerLiteral = Enek::FeatureTemplate::Parsing::IntegerLiteral;
  using FloatingLiteral = Enek::FeatureTemplate::Parsing::FloatingLiteral;
  using BooleanLiteral = Enek::FeatureTemplate::Parsing::BooleanLiteral;
  using StringLiteral = Enek::FeatureTemplate::Parsing::StringLiteral;
  using AST = Enek::FeatureTemplate::Parsing::AST;

public:
  using Path = std::filesystem::path;
  using BaseIteratorRange = boost::iterator_range<BaseIterator>;

private:
  using IteratorRange = boost::iterator_range<Iterator>;

private:
  void initializeIntegerLiteral(IntegerLiteral &self,
                                IteratorRange const &parse_range)
  {
    self.initialize(parse_range, path_, text_range_, os_);
  }

  static void moveAssignIntegerLiteral(IntegerLiteral &from, AST &to)
  {
    to.moveAssign(std::move(from));
  }

  void initializeFloatingLiteral(FloatingLiteral &self,
                                 IteratorRange const &parse_range)
  {
    self.initialize(parse_range, path_, text_range_, os_);
  }

  static void moveAssignFloatingLiteral(FloatingLiteral &from, AST &to)
  {
    to.moveAssign(std::move(from));
  }

  static void initializeBooleanLiteral(BooleanLiteral &self, bool value) noexcept
  {
    self.initialize(value);
  }

  static void moveAssignBooleanLiteral(BooleanLiteral &from, AST &to)
  {
    to.moveAssign(std::move(from));
  }

  void initializeStringLiteral(StringLiteral &self,
                               IteratorRange const &parse_range)
  {
    self.initialize(parse_range, path_, text_range_, os_);
  }

  static void moveAssignStringLiteral(StringLiteral &from, AST &to)
  {
    to.moveAssign(std::move(from));
  }

public:
  Grammar(Enek::FeatureTemplate::Parsing::InputType input_type,
          Path const &path,
          BaseIteratorRange const &text_range,
          std::ostream &os)
    : Grammar::base_type(feature_template_, "feature_template")
    , input_type_(input_type)
    , path_(path)
    , text_range_(text_range)
    , os_(os)
    , integer_literal_()
    , floating_literal_()
    , boolean_literal_()
    , escape_sequence_()
    , s_char_()
    , string_literal_()
  {
    namespace phx = boost::phoenix;
    namespace qi = boost::spirit::qi;

    /*identifier_
      = qi::char_('a')
      ;*/
    /*single_word_template_
      = qi::lit("$word")
      ;
    positional_word_template_
      = qi::lexeme['$' >> qi::uint_]
      ;
    input_template_
      = qi::lit("$input")
      ;*/
    integer_literal_
      = qi::raw[qi::lexeme[-qi::lit('-') >> ('0' | ((qi::digit - '0') >> *qi::digit))]]
      [phx::bind(&Grammar::initializeIntegerLiteral, this, qi::_val, qi::_1)]
      ;
    floating_literal_
      = qi::raw[qi::lexeme[
        -qi::lit('-')
        >> ((*qi::digit >> '.' >> +qi::digit | +qi::digit >> '.') >> -((qi::lit('e') | 'E') >> -(qi::lit('+') | '-') >> +qi::digit)
            | +qi::digit >> (qi::lit('e') | 'E') >> -(qi::lit('+') | '-') >> +qi::digit)
        ]]
      [phx::bind(&Grammar::initializeFloatingLiteral, this, qi::_val, qi::_1)]
      ;
    boolean_literal_
      = qi::bool_[phx::bind(&initializeBooleanLiteral, qi::_val, qi::_1)]
      ;
    escape_sequence_
      = (qi::lit("\\'") | "\\\"" | "\\?" | "\\\\" | "\\a" | "\\b" | "\\f" | "\\n" | "\\r" | "\\t" | "\\v")
      | '\\' >> qi::repeat(1, 3)[qi::char_("0-7")]
      | "\\x" >> +qi::xdigit
      | "\\u" >> qi::repeat(4)[qi::xdigit]
      | "\\U" >> qi::repeat(8)[qi::xdigit]
      ;
    s_char_
      = (qi::char_ - qi::char_("\"\\"))
      | escape_sequence_
      ;
    string_literal_
      = qi::raw[qi::lexeme['"' >> *s_char_ >> '"']]
        [phx::bind(&Grammar::initializeStringLiteral, this, qi::_val, qi::_1)]
      ;
    primary_feature_template_
      // `floating_literal_' should be placed prior to `integer_literal_' since
      // `integer_literal_' could eagerly consume a prefix of
      // `floating_literal_'.
      = floating_literal_[phx::bind(&moveAssignFloatingLiteral, qi::_1, qi::_val)]
      | integer_literal_[phx::bind(&moveAssignIntegerLiteral, qi::_1, qi::_val)]
      | boolean_literal_[phx::bind(&moveAssignBooleanLiteral, qi::_1, qi::_val)]
      | string_literal_[phx::bind(&moveAssignStringLiteral, qi::_1, qi::_val)]
      //| '(' >> feature_template_ >> ')'
      //| input_template_
      //| single_word_template_
      //| positional_word_template_
      //| identifier_
      ;
    /*feature_template_list_
      = feature_template_list_ % ','
      ;*/
    postfix_feature_template_
      = primary_feature_template_
      //| postfix_feature_template_ >> '[' >> feature_template_ >> ']'
      //| qi::lit("regex_search") >> '(' >> feature_template_list_ >> ')'
      //| qi::lit("regex_replace") >> '(' >> feature_template_list_ >> ')'
      //| qi::lit("integer") >> '(' >> feature_template_list_ >> ')'
      //| qi::lit("float") >> '(' >> feature_template_list_ >> ')'
      //| postfix_feature_template_ >> '.' >> "index"
      //| postfix_feature_template_ >> '.' >> "first"
      //| postfix_feature_template_ >> '.' >> "last"
      //| postfix_feature_template_ >> '.' >> "length"
      ;
    unary_feature_template_
      = postfix_feature_template_
      //| '+' >> unary_feature_template_
      //| '-' >> unary_feature_template_
      //| '!' >> unary_feature_template_
      ;
    multiplicative_feature_template_
      = unary_feature_template_
      //| multiplicative_feature_template_ >> '*' >> unary_feature_template_
      //| multiplicative_feature_template_ >> '/' >> unary_feature_template_
      //| multiplicative_feature_template_ >> '%' >> unary_feature_template_
      ;
    additive_feature_template_
      = multiplicative_feature_template_
      //| additive_feature_template_ >> '+' >> multiplicative_feature_template_
      //| additive_feature_template_ >> '-' >> multiplicative_feature_template_
      ;
    relational_feature_template_
      = additive_feature_template_
      //| additive_feature_template_ >> "==" >> additive_feature_template_
      //| additive_feature_template_ >> "!=" >> additive_feature_template_
      //| additive_feature_template_ >> '<' >> additive_feature_template_
      //| additive_feature_template_ >> '>' >> additive_feature_template_
      //| additive_feature_template_ >> "<=" >> additive_feature_template_
      //| additive_feature_template_ >> ">=" >> additive_feature_template_
      ;
    conjunctive_feature_template_
      = relational_feature_template_
      //| conjunctive_feature_template_ >> "&&" >> relational_feature_template_
      ;
    disjunctive_feature_template_
      = conjunctive_feature_template_
      //| disjunctive_feature_template_ >> "||" >> conjunctive_feature_template_
      ;
    feature_template_
      = disjunctive_feature_template_
      //| disjunctive_feature_template_ >> '?' >> feature_template_ >> ':' >> feature_template_
      ;

    integer_literal_.name("integer-literal");
    floating_literal_.name("floating-literal");
    boolean_literal_.name("boolean-literal");
    escape_sequence_.name("escape-sequence");
    s_char_.name("s-char");
    string_literal_.name("string-literal");

    feature_template_.name("feature_template");
    disjunctive_feature_template_.name("disjunctive_feature_template");
    conjunctive_feature_template_.name("conjunctive_feature_template");
    relational_feature_template_.name("relational_feature_template");
    additive_feature_template_.name("additive_feature_template");
    multiplicative_feature_template_.name("multiplicative_feature_template");
    unary_feature_template_.name("unary_feature_template");
    postfix_feature_template_.name("postfix_feature_template");
    feature_template_list_.name("feature_template_list");
    primary_feature_template_.name("primary_feature_template");
    input_template_.name("input_template");
    positional_word_template_.name("positional_word_template");
    single_word_template_.name("single_word_template");
    identifier_.name("identifier");
  }

private:
  template<typename Attribute = void>
  using Rule = boost::spirit::qi::rule<Iterator, Attribute()>;

private:
  Enek::FeatureTemplate::Parsing::InputType input_type_;
  Path const &path_;
  BaseIteratorRange const &text_range_;
  std::ostream &os_;
  Rule<IntegerLiteral> integer_literal_;
  Rule<FloatingLiteral> floating_literal_;
  Rule<BooleanLiteral> boolean_literal_;
  Rule<> escape_sequence_;
  Rule<> s_char_;
  Rule<StringLiteral> string_literal_;

  Rule<AST> feature_template_;
  boost::spirit::qi::rule<Iterator, AST()> disjunctive_feature_template_;
  boost::spirit::qi::rule<Iterator, AST()> conjunctive_feature_template_;
  boost::spirit::qi::rule<Iterator, AST()> relational_feature_template_;
  boost::spirit::qi::rule<Iterator, AST()> additive_feature_template_;
  boost::spirit::qi::rule<Iterator, AST()> multiplicative_feature_template_;
  boost::spirit::qi::rule<Iterator, AST()> unary_feature_template_;
  boost::spirit::qi::rule<Iterator, AST()> postfix_feature_template_;
  boost::spirit::qi::rule<Iterator, AST()> feature_template_list_;
  boost::spirit::qi::rule<Iterator, AST()> primary_feature_template_;
  boost::spirit::qi::rule<Iterator, AST()> input_template_;
  boost::spirit::qi::rule<Iterator, AST()> positional_word_template_;
  boost::spirit::qi::rule<Iterator, AST()> single_word_template_;
  boost::spirit::qi::rule<Iterator, AST()> identifier_;
}; // struct Grammar

} // namespace Enek::FeatureTemplate::Parsing

#endif // !defined(ENEK_FEATURE_TEMPLATE_PARSING_GRAMMAR_HPP_INCLUDE_GUARD)
