#include <enek/feature_template/parsing/floating_literal.hpp>

#include <enek/util/throw.hpp>
#include <enek/util/assert.hpp>
#include <ostream>
#include <utility>
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

bool FloatingLiteral::succeed() const noexcept
{
  ENEK_ASSERT(this->isInitialized());
  return true;
}

void FloatingLiteral::swap(FloatingLiteral &rhs) noexcept
{
  using std::swap;
  swap(initialized_, rhs.initialized_);
  swap(value_,       rhs.value_);
}

void swap(FloatingLiteral &lhs, FloatingLiteral &rhs) noexcept
{
  lhs.swap(rhs);
}

FloatingLiteral &
FloatingLiteral::operator=(FloatingLiteral const &rhs) noexcept
{
  ENEK_ASSERT(rhs.isInitialized());
  ENEK_ASSERT(!this->isInitialized());
  FloatingLiteral(rhs).swap(*this);
  return *this;
}

double FloatingLiteral::getValue() const noexcept
{
  ENEK_ASSERT(this->isInitialized());
  return value_;
}

void FloatingLiteral::dumpXML(std::ostream &os) const
{
  if (!this->isInitialized()) {
    ENEK_THROW(std::invalid_argument)
      << "error: `dumpXML' is called on an uninitialized `FloatingLiteral'.";
  }
  os << "<floating_literal>" << value_ << "</floating_literal>";
}

} // namespace Enek::FeatureTemplate::Parsing
