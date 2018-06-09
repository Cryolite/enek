#if !defined(ENEK_FEATURE_TEMPLATE_PARSING_AST_HPP_INCLUDE_GUARD)
#define ENEK_FEATURE_TEMPLATE_PARSING_AST_HPP_INCLUDE_GUARD

#include <enek/feature_template/parsing/floating_literal.hpp>
#include <enek/feature_template/parsing/integer_literal.hpp>
#include <enek/feature_template/parsing/boolean_literal.hpp>
#include <enek/feature_template/parsing/string_literal.hpp>
#include <ostream>
#include <variant>


namespace Enek::FeatureTemplate::Parsing{

class UninitializedASTNode
{
public:
  constexpr UninitializedASTNode() noexcept
  {}
}; // class UninitializedASTNode

using ASTNode = std::variant<
  UninitializedASTNode,
  Enek::FeatureTemplate::Parsing::IntegerLiteral,
  Enek::FeatureTemplate::Parsing::FloatingLiteral,
  Enek::FeatureTemplate::Parsing::BooleanLiteral,
  Enek::FeatureTemplate::Parsing::StringLiteral>;

class AST
{
public:
  AST() noexcept;

  AST(AST const &rhs);

  AST(AST &&rhs) noexcept;

  bool isInitialized() const noexcept;

  template<typename ASTNode>
  void moveAssign(ASTNode &&root_node);

  bool succeed() const noexcept;

  void dumpXML(std::ostream &os) const;

private:
  ASTNode root_node_;
}; // class AST

} // namespace Enek::FeatureTemplate::Parsing

#endif // !defined(ENEK_FEATURE_TEMPLATE_PARSING_AST_HPP_INCLUDE_GUARD)
