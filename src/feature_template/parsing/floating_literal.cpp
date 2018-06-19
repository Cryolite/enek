#include <enek/feature_template/parsing/floating_literal.hpp>
#include <enek/feature_template/type.hpp>
#include <enek/util/throw.hpp>
#include <enek/util/assert.hpp>
#include <ostream>
#include <stdexcept>


namespace Enek::FeatureTemplate::Parsing{

FloatingLiteral::FloatingLiteral() noexcept
  : initialized_()
  , value_()
{}

FloatingLiteral::FloatingLiteral(FloatingLiteral const &rhs) noexcept
  : initialized_(rhs.initialized_)
  , value_(rhs.value_)
{}

bool FloatingLiteral::isInitialized() const noexcept
{
  return initialized_;
}

bool FloatingLiteral::succeed() const
{
  if (!this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "`succeed' is called on an uninitialized object.");
  }
  return true;
}

Enek::FeatureTemplate::Type FloatingLiteral::getType() const
{
  if (!this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "`getType' is called on an uninitialized object.");
  }
  return Enek::FeatureTemplate::Type::floating;
}

double FloatingLiteral::getValue() const
{
  if (!this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "`getValue' is called on an uninitialized object.");
  }
  if (!this->succeed()) {
    ENEK_THROW<std::invalid_argument>(
      "`getValue' is called on an object initialized with failed parse.");
  }
  return value_;
}

void FloatingLiteral::dumpXML(std::ostream &os) const
{
  if (!this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "`dumpXML' is called on an uninitialized object.");
  }
  os << "<floating_literal>" << value_ << "</floating_literal>";
}

} // namespace Enek::FeatureTemplate::Parsing
