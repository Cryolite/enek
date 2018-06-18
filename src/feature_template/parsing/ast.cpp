#include <enek/feature_template/parsing/ast.hpp>

#include <enek/feature_template/parsing/floating_literal.hpp>
#include <enek/feature_template/parsing/integer_literal.hpp>
#include <enek/feature_template/parsing/boolean_literal.hpp>
#include <enek/feature_template/parsing/string_literal.hpp>
#include <enek/util/throw.hpp>
#include <enek/util/assert.hpp>
#include <enek/util/type_name.hpp>
#include <ostream>
#include <variant>
#include <utility>
#include <stdexcept>


namespace Enek::FeatureTemplate::Parsing{

namespace{

class IsInitializedASTVisitor
{
public:
  IsInitializedASTVisitor()
  {}

  bool operator()(UninitializedASTNode const &) const
  {
    ENEK_THROW<std::logic_error>(
      "error: `isInitialized' is called on `UninitializedASTNode'.");
    return false;
  }

  template<typename ASTNode>
  bool operator()(ASTNode const &node) const noexcept
  {
    return node.isInitialized();
  }
}; // class IsInitializedASTVisitor

bool isInitialized(Enek::FeatureTemplate::Parsing::ASTNode const &node)
{
  IsInitializedASTVisitor vis;
  return std::visit(vis, node);
}

class SucceedASTVisitor
{
public:
  SucceedASTVisitor()
  {}

  bool operator()(UninitializedASTNode const &) const
  {
    ENEK_THROW<std::logic_error>(
      "error: `succeed' is called on `UninitializedASTNode'.");
    return false;
  }

  template<typename ASTNode>
  bool operator()(ASTNode const &node) const noexcept
  {
    return node.succeed();
  }
}; // class SucceedASTVisitor

bool succeed(ASTNode const &node)
{
  SucceedASTVisitor vis;
  return std::visit(vis, node);
}

class DumpXMLASTVisitor
{
public:
  explicit DumpXMLASTVisitor(std::ostream &os)
    : os_(os)
  {}

  void operator()(UninitializedASTNode const &)
  {
    ENEK_THROW<std::logic_error>(
      "error: `dumpXML' is called on `UninitializedASTNode'.");
  }

  template<typename ASTNode>
  void operator()(ASTNode const &node)
  {
    node.dumpXML(os_);
  }

private:
  std::ostream &os_;
}; // class DumpXMLASTVisitor

void dumpXML(ASTNode const &node, std::ostream &os)
{
  DumpXMLASTVisitor vis(os);
  std::visit(vis, node);
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
  return root_node_.index() != 0;
}

template<typename ASTNode>
void AST::moveAssign(ASTNode &&root_node)
{
  static_assert(!std::is_reference<ASTNode>::value);
  ENEK_ASSERT(Enek::FeatureTemplate::Parsing::isInitialized(root_node))
    << "info: The type of `root_node' is `" << Enek::getTypeName(root_node)
    << "'." << std::endl;
  ENEK_ASSERT(!this->isInitialized());
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

bool AST::succeed() const noexcept
{
  ENEK_ASSERT(this->isInitialized());
  return Enek::FeatureTemplate::Parsing::succeed(root_node_);
}

void AST::dumpXML(std::ostream &os) const
{
  if (!this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "error: `dumpXML' is called on an uninitialized `AST'.");
  }
  os << "<feature_template>";
  Enek::FeatureTemplate::Parsing::dumpXML(root_node_, os);
  os << "</feature_template>";
}

} // namespace Enek::FeatureTemplate::Parsing
