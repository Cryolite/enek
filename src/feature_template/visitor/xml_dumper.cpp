#include <enek/feature_template/visitor/xml_dumper.hpp>
#include <enek/feature_template/parsing/ast.hpp>
#include <enek/util/throw.hpp>
#include <enek/util/assert.hpp>
#include <boost/io/ios_state.hpp>
#include <ostream>
#include <ios>
#include <stdexcept>


namespace Enek::FeatureTemplate{

XMLDumper::XMLDumper(std::ostream &os) noexcept
  : os_(os)
{}

void XMLDumper::leave(IntegerLiteral const &node)
{
  ENEK_ASSERT(node.isInitialized());
  os_ << "<integer_literal";
  if (!node.succeed()) {
    os_ << " succeed=\"false\"/>";
    return;
  }
  os_ << '>' << node.getValue() << "</integer_literal>";
}

void XMLDumper::leave(FloatingLiteral const &node)
{
  ENEK_ASSERT(node.isInitialized());
  os_ << "<floating_literal";
  if (!node.succeed()) {
    os_ << " succeed=\"false\"/>";
    return;
  }
  os_ << '>' << node.getValue() << "</floating_literal>";
}

void XMLDumper::leave(BooleanLiteral const &node)
{
  ENEK_ASSERT(node.isInitialized());
  os_ << "<boolean_literal";
  if (!node.succeed()) {
    // LCOV_EXCL_START
    os_ << " succeed=\"false\"/>";
    return;
    // LCOV_EXCL_STOP
  }
  boost::io::ios_flags_saver ifs(os_);
  os_ << '>' << std::boolalpha << node.getValue() << "</boolean_literal>";
}

void XMLDumper::leave(StringLiteral const &node)
{
  ENEK_ASSERT(node.isInitialized());
  os_ << "<string_literal";
  if (!node.succeed()) {
    os_ << " succeed=\"false\"/>";
    return;
  }
  os_ << '>' << node.getValue() << "</string_literal>";
}

void dumpXML(Enek::FeatureTemplate::Parsing::AST const &ast, std::ostream &os)
{
  if (!ast.isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "`dumpXML' is called on an uninitialized object.");
  }
  XMLDumper xml_dumper(os);
  Enek::FeatureTemplate::visit(xml_dumper, ast);
}

} // namespace Enek::FeatureTemplate
