#include <enek/feature_template/parsing/floating_literal.hpp>
#include <enek/feature_template/parsing/message_printer.hpp>
#include <enek/feature_template/parsing/text_position_iterator.hpp>
#include <enek/feature_template/type.hpp>
#include <enek/util/throw.hpp>
#include <enek/util/assert.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/end.hpp>
#include <boost/range/begin.hpp>
#include <sstream>
#include <ostream>
#include <string>
#include <stdexcept>


namespace Enek::FeatureTemplate::Parsing{

FloatingLiteral::FloatingLiteral() noexcept
  : initialized_()
  , error_()
  , value_()
{}

FloatingLiteral::FloatingLiteral(FloatingLiteral const &rhs) noexcept
  : initialized_(rhs.initialized_)
  , error_(rhs.error_)
  , value_(rhs.value_)
{}

bool FloatingLiteral::isInitialized() const noexcept
{
  ENEK_ASSERT(!error_ || initialized_);
  return initialized_;
}

template<typename Iterator, typename BaseIterator>
void FloatingLiteral::initialize(
  boost::iterator_range<Iterator> const &parse_range,
  Path const &path,
  boost::iterator_range<BaseIterator> const &text_range,
  std::ostream &os)
{
  if (this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "Try to initialize an already initialized object.");
  }
  std::string buf(boost::begin(parse_range), boost::end(parse_range));
  std::istringstream iss(buf);
  iss >> value_;
  ENEK_ASSERT(iss.eof());
  if (iss.fail()) {
    error_ = true;
    value_ = 0.;
    using Enek::FeatureTemplate::Parsing::makeMessagePrinter;
    makeMessagePrinter(path, text_range, parse_range, os)
      << "error: An error occurred while parsing a floating literal.";
  }
  initialized_ = true;
}

#define ENEK_INSTANTIATE(...)                                           \
  template                                                              \
  void FloatingLiteral::initialize<                                     \
    Enek::FeatureTemplate::Parsing::TextPositionIterator<__VA_ARGS__>,  \
    __VA_ARGS__                                                         \
  >(                                                                    \
    boost::iterator_range<                                              \
      Enek::FeatureTemplate::Parsing::TextPositionIterator<__VA_ARGS__> \
    > const &parse_range,                                               \
    Path const &path,                                                   \
    boost::iterator_range<__VA_ARGS__> const &text_range,               \
    std::ostream &)

ENEK_INSTANTIATE(std::string::const_iterator);

bool FloatingLiteral::succeed() const
{
  if (!this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "`succeed' is called on an uninitialized object.");
  }
  return !error_;
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

} // namespace Enek::FeatureTemplate::Parsing
