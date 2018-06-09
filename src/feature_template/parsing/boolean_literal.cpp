#include <enek/feature_template/parsing/boolean_literal.hpp>

#include <enek/util/throw.hpp>
#include <enek/util/assert.hpp>
#include <boost/io/ios_state.hpp>
#include <ostream>
#include <ios>
#include <utility>
#include <stdexcept>

namespace Enek::FeatureTemplate::Parsing{

BooleanLiteral::BooleanLiteral() noexcept
  : initialized_()
  , value_()
{}

BooleanLiteral::BooleanLiteral(BooleanLiteral const &rhs) noexcept = default;

bool BooleanLiteral::isInitialized() const noexcept
{
  return initialized_;
}

void BooleanLiteral::initialize(bool value) noexcept
{
  ENEK_ASSERT(!this->isInitialized());
  value_ = value;
  initialized_ = true;
}

bool BooleanLiteral::succeed() const noexcept
{
  ENEK_ASSERT(this->isInitialized());
  return true;
}

void BooleanLiteral::swap(BooleanLiteral &rhs) noexcept
{
  using std::swap;
  swap(value_,       rhs.value_);
  swap(initialized_, rhs.initialized_);
}

void swap(BooleanLiteral &lhs, BooleanLiteral &rhs) noexcept
{
  lhs.swap(rhs);
}

BooleanLiteral &BooleanLiteral::operator=(BooleanLiteral const &rhs) noexcept
{
  ENEK_ASSERT(rhs.isInitialized());
  ENEK_ASSERT(!this->isInitialized());
  BooleanLiteral(rhs).swap(*this);
  return *this;
}

bool BooleanLiteral::getValue() const noexcept
{
  ENEK_ASSERT(this->isInitialized());
  return value_;
}

void BooleanLiteral::dumpXML(std::ostream &os) const
{
  if (!this->isInitialized()) {
    ENEK_THROW(std::invalid_argument)
      << "`dumpXML' is called on an uninitialized `BooleanLiteral'.";
  }
  boost::io::ios_flags_saver saver(os);
  os << "<boolean_literal>" << std::boolalpha << value_
     << "</boolean_literal>";
}

} // namespace Enek::FeatureTemplate::Parsing
