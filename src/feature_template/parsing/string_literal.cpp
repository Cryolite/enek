#include <enek/feature_template/parsing/string_literal.hpp>

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
  return initialized_;
}

bool StringLiteral::succeed() const noexcept
{
  return !error_;
}

void StringLiteral::swap(StringLiteral &rhs) noexcept
{
  using std::swap;
  swap(initialized_, rhs.initialized_);
  swap(error_,       rhs.error_);
  swap(value_,       rhs.value_);
}

StringLiteral &StringLiteral::operator=(StringLiteral const &rhs)
{
  ENEK_ASSERT(rhs.isInitialized());
  ENEK_ASSERT(!this->isInitialized());
  StringLiteral(rhs).swap(*this);
  return *this;
}

StringLiteral &StringLiteral::operator=(StringLiteral &&rhs) noexcept
{
  ENEK_ASSERT(rhs.isInitialized());
  ENEK_ASSERT(!this->isInitialized());
  StringLiteral(std::move(rhs)).swap(*this);
  return *this;
}

std::string const &StringLiteral::getValue() const noexcept
{
  ENEK_ASSERT(this->isInitialized());
  ENEK_ASSERT(this->succeed());
  return value_;
}

void StringLiteral::dumpXML(std::ostream &os) const
{
  if (!this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "`dumpXML' is called on an uninitialized `StringLiteral'.");
  }
  os << "<string_literal>" << value_ << "</string_literal>";
}

} // namespace Enek::FeatureTemplate::Parsing
