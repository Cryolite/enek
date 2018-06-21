#include <enek/feature_template/parsing/ast.hpp>
#include <gtest/gtest.h>
#include <boost/exception/exception.hpp>
#include <stdexcept>


namespace Parsing = Enek::FeatureTemplate::Parsing;

namespace{

using Parsing::AST;

class Visitor
{
public:
  template<typename ASTNode>
  void enter(ASTNode const &)
  {}

  template<typename ASTNode>
  void leave(ASTNode const &)
  {}
}; // class Visitor

} // namespace *unnamed*

TEST(FeatureTemplateParsingASTTest, testDefaultConstructor)
{
  AST ast;
  EXPECT_FALSE(ast.isInitialized());
  EXPECT_THROW(ast.succeed(), std::invalid_argument);
  EXPECT_THROW(ast.succeed(), boost::exception);
  EXPECT_THROW(ast.getType(), std::invalid_argument);
  EXPECT_THROW(ast.getType(), boost::exception);
  EXPECT_THROW(Enek::FeatureTemplate::visit(Visitor(), ast),
               std::invalid_argument);
  EXPECT_THROW(Enek::FeatureTemplate::visit(Visitor(), ast),
               boost::exception);
}
