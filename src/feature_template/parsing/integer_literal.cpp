#include <enek/feature_template/parsing/integer_literal.hpp>

#include <enek/util/throw.hpp>
#include <enek/util/assert.hpp>
#include <ostream>
#include <utility>
#include <stdexcept>
#include <cstdint>


namespace Enek::FeatureTemplate::Parsing{

IntegerLiteral::IntegerLiteral() noexcept
  : initialized_()
  , error_()
  , value_(0)
{}

IntegerLiteral::IntegerLiteral(IntegerLiteral const &rhs) noexcept
  : initialized_(rhs.initialized_)
  , error_(rhs.error_)
  , value_(rhs.value_)
{}

bool IntegerLiteral::isInitialized() const noexcept
{
  ENEK_ASSERT(!error_ || initialized_);
  return initialized_;
}

bool IntegerLiteral::succeed() const noexcept
{
  ENEK_ASSERT(this->isInitialized());
  return !error_;
}

std::int_fast64_t IntegerLiteral::getValue() const noexcept
{
  ENEK_ASSERT(this->isInitialized());
  return value_;
}

void IntegerLiteral::dumpXML(std::ostream &os) const
{
  if (!this->isInitialized()) {
    ENEK_THROW(std::invalid_argument)
      << "error: `dumpXML' is called on an uninitialized `IntegerLiteral'.";
  }
  os << "<integer_literal>" << value_ << "</integer_literal>";
}

} // namespace Enek::FeatureTemplate::Parsing
