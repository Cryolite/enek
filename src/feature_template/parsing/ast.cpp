#include <enek/feature_template/parsing/ast.hpp>
#include <enek/feature_template/parsing/floating_literal.hpp>
#include <enek/feature_template/parsing/integer_literal.hpp>
#include <enek/feature_template/parsing/boolean_literal.hpp>
#include <enek/feature_template/parsing/string_literal.hpp>
#include <enek/feature_template/type.hpp>
#include <enek/util/throw.hpp>
#include <enek/util/type_name.hpp>
#include <type_traits>
#include <functional>
#include <variant>
#include <utility>
#include <stdexcept>


namespace Enek::FeatureTemplate::Parsing{

namespace{

class IsInitializedASTVisitor
{
public:
  constexpr IsInitializedASTVisitor() = default;

  IsInitializedASTVisitor(IsInitializedASTVisitor const &) = delete;

  IsInitializedASTVisitor &operator=(IsInitializedASTVisitor const &) = delete;

  // LCOV_EXCL_START
  bool operator()(UninitializedASTNode const &node) const noexcept
  {
    using std::placeholders::_1;
    ENEK_THROW<std::logic_error>(_1)
      << "`isInitialized' is called on an object of type `"
      << Enek::getTypeName(node) << "'.";
    return false;
  }
  // LCOV_EXCL_STOP

  template<typename ASTNode>
  bool operator()(ASTNode const &node) const noexcept
  {
    return node.isInitialized();
  }
}; // class IsInitializedASTVisitor

bool isInitialized(Enek::FeatureTemplate::Parsing::ASTNode const &node) noexcept
{
  IsInitializedASTVisitor vis;
  return std::visit(vis, node);
}

class SucceedASTVisitor
{
public:
  constexpr SucceedASTVisitor() = default;

  SucceedASTVisitor(SucceedASTVisitor const &) = delete;

  SucceedASTVisitor &operator=(SucceedASTVisitor const &) = delete;

  // LCOV_EXCL_START
  bool operator()(UninitializedASTNode const &node) const
  {
    using std::placeholders::_1;
    ENEK_THROW<std::logic_error>(_1)
      << "`succeed' is called on an object of type `"
      << Enek::getTypeName(node) << "'.";
    return false;
  }
  // LCOV_EXCL_STOP

  template<typename ASTNode>
  bool operator()(ASTNode const &node) const
  {
    return node.succeed();
  }
}; // class SucceedASTVisitor

bool succeed(ASTNode const &node)
{
  SucceedASTVisitor vis;
  return std::visit(vis, node);
}

class GetTypeASTVisitor
{
public:
  constexpr GetTypeASTVisitor() = default;

  GetTypeASTVisitor(GetTypeASTVisitor const &) = delete;

  GetTypeASTVisitor &operator=(GetTypeASTVisitor const &) = delete;

  // LCOV_EXCL_START
  Enek::FeatureTemplate::Type operator()(
    UninitializedASTNode const &node) const
  {
    using std::placeholders::_1;
    ENEK_THROW<std::logic_error>(_1)
      << "`getType' is called on an object of type `"
      << Enek::getTypeName(node) << "'.";
    return Enek::FeatureTemplate::Type::unknown;
  }
  // LCOV_EXCL_STOP

  template<typename ASTNode>
  Enek::FeatureTemplate::Type operator()(ASTNode const &node) const
  {
    return node.getType();
  }
}; // class GetTypeASTVisitor

Enek::FeatureTemplate::Type getType(ASTNode const &node)
{
  GetTypeASTVisitor vis;
  return std::visit(vis, node);
}

} // namespace *unnamed*

AST::AST() noexcept
  : root_node_()
{}

AST::AST(AST const &rhs)
  : root_node_(rhs.root_node_)
{}

AST::AST(AST &&rhs) noexcept
  : root_node_(std::move(rhs.root_node_))
{}

bool AST::isInitialized() const noexcept
{
  if (root_node_.index() == 0) {
    return false;
  }
  return Enek::FeatureTemplate::Parsing::isInitialized(root_node_);
}

template<typename ASTNode>
void AST::moveAssign(ASTNode &&root_node)
{
  static_assert(!std::is_reference<ASTNode>::value);
  using std::placeholders::_1;
  if (this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "Try to initialize an already initialized object.");
  }
  if (!Enek::FeatureTemplate::Parsing::isInitialized(root_node)) {
    ENEK_THROW<std::invalid_argument>(_1)
      << "Try to initialize with an uninitialized object of type `"
      << Enek::getTypeName(root_node) << "'.";
  }
  root_node_.emplace<ASTNode>(std::move(root_node));
}

template
void AST::moveAssign<Enek::FeatureTemplate::Parsing::IntegerLiteral>(
  Enek::FeatureTemplate::Parsing::IntegerLiteral &&);
template
void AST::moveAssign<Enek::FeatureTemplate::Parsing::FloatingLiteral>(
  Enek::FeatureTemplate::Parsing::FloatingLiteral &&);
template
void AST::moveAssign<Enek::FeatureTemplate::Parsing::BooleanLiteral>(
  Enek::FeatureTemplate::Parsing::BooleanLiteral &&);
template
void AST::moveAssign<Enek::FeatureTemplate::Parsing::StringLiteral>(
  Enek::FeatureTemplate::Parsing::StringLiteral &&);

bool AST::succeed() const
{
  if (!this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "`succeed' is called on an uninitialized object.");
  }
  return Enek::FeatureTemplate::Parsing::succeed(root_node_);
}

Enek::FeatureTemplate::Type AST::getType() const
{
  if (!this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "`getType' is called on an uninitialized object.");
  }
  return Enek::FeatureTemplate::Parsing::getType(root_node_);
}

} // namespace Enek::FeatureTemplate::Parsing
