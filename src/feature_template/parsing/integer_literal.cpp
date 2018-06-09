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

void IntegerLiteral::swap(IntegerLiteral &rhs) noexcept
{
  using std::swap;
  swap(initialized_, rhs.initialized_);
  swap(error_,       rhs.error_);
  swap(value_,       rhs.value_);
}

void swap(IntegerLiteral &lhs, IntegerLiteral &rhs) noexcept
{
  lhs.swap(rhs);
}

IntegerLiteral &IntegerLiteral::operator=(IntegerLiteral const &rhs) noexcept
{
  ENEK_ASSERT(rhs.isInitialized());
  ENEK_ASSERT(!this->isInitialized());
  IntegerLiteral(rhs).swap(*this);
  return *this;
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
