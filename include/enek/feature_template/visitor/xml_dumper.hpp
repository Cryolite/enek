#if !defined(ENEK_FEATURE_TEMPLATE_VISITOR_XML_DUMPER_HPP_INCLUDE_GUARD)
#define ENEK_FEATURE_TEMPLATE_VISITOR_XML_DUMPER_HPP_INCLUDE_GUARD

#include <enek/feature_template/visitor/base.hpp>
#include <enek/feature_template/parsing/ast.hpp>
#include <iosfwd>


namespace Enek::FeatureTemplate{

class XMLDumper
  : private Enek::FeatureTemplate::VisitorBase
{
public:
  using VisitorBase::IntegerLiteral;
  using VisitorBase::FloatingLiteral;
  using VisitorBase::BooleanLiteral;
  using VisitorBase::StringLiteral;

public:
  explicit XMLDumper(std::ostream &os) noexcept;

  XMLDumper(XMLDumper const &) = delete;

  XMLDumper &operator=(XMLDumper const &) = delete;

  using VisitorBase::enter;

  using VisitorBase::leave;

  void leave(IntegerLiteral const &node);

  void leave(FloatingLiteral const &node);

  void leave(BooleanLiteral const &node);

  void leave(StringLiteral const &node);

private:
  std::ostream &os_;
}; // class XMLDumper

void dumpXML(Enek::FeatureTemplate::Parsing::AST const &node,
             std::ostream &os);

} // namespace Enek::FeatureTemplate

#endif // !defined(ENEK_FEATURE_TEMPLATE_VISITOR_XML_DUMPER_HPP_INCLUDE_GUARD)
