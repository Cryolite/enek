#if !defined(ENEK_FEATURE_TEMPLATE_VISITOR_BASE_HPP_INCLUDE_GUARD)
#define ENEK_FEATURE_TEMPLATE_VISITOR_BASE_HPP_INCLUDE_GUARD

#include <enek/feature_template/parsing/string_literal.hpp>
#include <enek/feature_template/parsing/boolean_literal.hpp>
#include <enek/feature_template/parsing/floating_literal.hpp>
#include <enek/feature_template/parsing/integer_literal.hpp>


namespace Enek::FeatureTemplate{

class VisitorBase
{
protected:
  using IntegerLiteral = Enek::FeatureTemplate::Parsing::IntegerLiteral;
  using FloatingLiteral = Enek::FeatureTemplate::Parsing::FloatingLiteral;
  using BooleanLiteral = Enek::FeatureTemplate::Parsing::BooleanLiteral;
  using StringLiteral = Enek::FeatureTemplate::Parsing::StringLiteral;

protected:
  template<typename ASTNode>
  void enter(ASTNode const &)
  {}

  template<typename ASTNode>
  void leave(ASTNode const &)
  {}
}; // class VisitorBase

} // namespace Enek::FeatureTemplate

#endif // !defined(ENEK_FEATURE_TEMPLATE_VISITOR_BASE_HPP_INCLUDE_GUARD)
