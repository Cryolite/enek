#include <enek/feature_template/parsing/boolean_literal.hpp>
#include <enek/feature_template/type.hpp>
#include <enek/util/throw.hpp>
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

void BooleanLiteral::initialize(bool value)
{
  if (this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "Try to initialize an already initialized object.");
  }
  value_ = value;
  initialized_ = true;
}

bool BooleanLiteral::succeed() const
{
  if (!this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "`succeed' is called on an uninitialized object.");
  }
  return true;
}

Enek::FeatureTemplate::Type BooleanLiteral::getType() const
{
  if (!this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "`getType' is called on an uninitialized object.");
  }
  return Enek::FeatureTemplate::Type::boolean;
}

bool BooleanLiteral::getValue() const
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

} // namespace Enek::FeatureTemplate::Parsing
