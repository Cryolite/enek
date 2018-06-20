#if !defined(ENEK_FEATURE_TEMPLATE_PARSING_AST_HPP_INCLUDE_GUARD)
#define ENEK_FEATURE_TEMPLATE_PARSING_AST_HPP_INCLUDE_GUARD

#include <enek/feature_template/parsing/string_literal.hpp>
#include <enek/feature_template/parsing/boolean_literal.hpp>
#include <enek/feature_template/parsing/floating_literal.hpp>
#include <enek/feature_template/parsing/integer_literal.hpp>
#include <enek/feature_template/type.hpp>
#include <enek/util/throw.hpp>
#include <ostream>
#include <type_traits>
#include <variant>
#include <utility>
#include <stdexcept>


namespace Enek::FeatureTemplate{

namespace Parsing{

class AST;

} // namespace Parsing

template<typename Visitor>
void visit(Visitor &&visitor, Parsing::AST const &ast);

namespace Parsing{

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

  AST &operator=(AST const &) = delete;

  bool isInitialized() const noexcept;

  template<typename ASTNode>
  void moveAssign(ASTNode &&root_node);

  bool succeed() const;

  Enek::FeatureTemplate::Type getType() const;

  template<typename Visitor>
  friend void Enek::FeatureTemplate::visit(Visitor &&visitor, AST const &ast);

private:
  ASTNode root_node_;
}; // class AST

} // namespace Parsing

namespace Detail{

template<typename Visitor>
class VisitImpl
{
private:
  static_assert(std::is_reference_v<Visitor>);

public:
  using IntegerLiteral = Enek::FeatureTemplate::Parsing::IntegerLiteral;
  using FloatingLiteral = Enek::FeatureTemplate::Parsing::FloatingLiteral;
  using BooleanLiteral = Enek::FeatureTemplate::Parsing::BooleanLiteral;
  using StringLiteral = Enek::FeatureTemplate::Parsing::StringLiteral;

public:
  explicit VisitImpl(Visitor visitor) noexcept
    : visitor_(std::forward<Visitor>(visitor))
  {}

  VisitImpl(VisitImpl const &) = delete;

  VisitImpl &operator=(VisitImpl const &) = delete;

  // LCOV_EXCL_START
  void operator()(Parsing::UninitializedASTNode const &) const
  {
    ENEK_THROW<std::logic_error>(
      "A logic error in `Enek::FeatureTemplate::visit'. "
      "It should throw if the argument is not initialized.");
  }
  // LCOV_EXCL_STOP

  void operator()(IntegerLiteral const &node) const
  {
    visitor_.enter(node);
    visitor_.leave(node);
  }

  void operator()(FloatingLiteral const &node) const
  {
    visitor_.enter(node);
    visitor_.leave(node);
  }

  void operator()(BooleanLiteral const &node) const
  {
    visitor_.enter(node);
    visitor_.leave(node);
  }

  void operator()(StringLiteral const &node) const
  {
    visitor_.enter(node);
    visitor_.leave(node);
  }

private:
  Visitor visitor_;
}; // class VisitImpl

} // namespace Detail

template<typename Visitor>
void visit(Visitor &&visitor, Parsing::AST const &ast)
{
  if (!ast.isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "`visit' is called on an uninitialized object.");
  }
  Detail::VisitImpl<Visitor &&> vis(std::forward<Visitor>(visitor));
  std::visit(vis, ast.root_node_);
}

} // namespace Enek::FeatureTemplate

#endif // !defined(ENEK_FEATURE_TEMPLATE_PARSING_AST_HPP_INCLUDE_GUARD)
