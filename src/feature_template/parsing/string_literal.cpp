#include <enek/feature_template/parsing/string_literal.hpp>
#include <enek/feature_template/type.hpp>
#include <enek/util/throw.hpp>
#include <enek/util/assert.hpp>
#include <ostream>
#include <string>
#include <utility>
#include <stdexcept>


namespace Enek::FeatureTemplate::Parsing{

StringLiteral::StringLiteral() noexcept
  : initialized_()
  , error_()
  , value_()
{}

StringLiteral::StringLiteral(StringLiteral const &) = default;

StringLiteral::StringLiteral(StringLiteral &&rhs) noexcept
  : initialized_(rhs.initialized_)
  , error_(rhs.error_)
  , value_(std::move(rhs.value_))
{
  rhs.initialized_ = false;
  rhs.error_ = false;
  rhs.value_.clear();
}

bool StringLiteral::isInitialized() const noexcept
{
  ENEK_ASSERT(!error_ || initialized_);
  return initialized_;
}

bool StringLiteral::succeed() const
{
  if (!this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "`succeed' is called on an uninitialized object.");
  }
  return !error_;
}

Enek::FeatureTemplate::Type StringLiteral::getType() const
{
  if (!this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "`getType' is called on an uninitialized object.");
  }
  return Enek::FeatureTemplate::Type::string;
}

std::string const &StringLiteral::getValue() const
{
  if (!this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "`getValue' is called on an uninitialized object.");
  }
  if (!this->succeed()) {
    ENEK_THROW<std::invalid_argument>(
      "`getValue' is caleld on an object initialized with failed parse.");
  }
  return value_;
}

void StringLiteral::dumpXML(std::ostream &os) const
{
  if (!this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "`dumpXML' is called on an uninitialized object.");
  }
  if (this->succeed()) {
    os << "<string_literal>" << this->getValue() << "</string_literal>";
  }
  else {
    os << "<string_literal succeed=\"false\"/>";
  }
}

} // namespace Enek::FeatureTemplate::Parsing
