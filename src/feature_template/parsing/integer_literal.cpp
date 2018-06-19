#include <enek/feature_template/parsing/integer_literal.hpp>
#include <enek/feature_template/parsing/message_printer.hpp>
#include <enek/feature_template/parsing/text_position_iterator.hpp>
#include <enek/feature_template/type.hpp>
#include <enek/util/throw.hpp>
#include <enek/util/assert.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/end.hpp>
#include <boost/range/begin.hpp>
#include <ostream>
#include <string>
#include <type_traits>
#include <stdexcept>
#include <cstdint>
#include <limits>


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

template<typename Iterator, typename BaseIterator>
void IntegerLiteral::initialize(
  boost::iterator_range<Iterator> const &parse_range,
  Path const &path,
  boost::iterator_range<BaseIterator> const &text_range,
  std::ostream &os)
{
  static_assert(
    std::is_same<typename Iterator::BaseIterator, BaseIterator>::value);
  if (this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "Try to initialize an already initialized object.");
  }
  ENEK_ASSERT(!error_);
  ENEK_ASSERT(value_ == 0);
  ENEK_ASSERT(boost::begin(parse_range) != boost::end(parse_range));
  Iterator iter = boost::begin(parse_range);
  if (!iter.hasTextPosition()) {
    ENEK_THROW<std::invalid_argument>(
      "The argument `parse_range' does not have any text position.");
  }
  Iterator const parse_last = boost::end(parse_range);

  enum class ErrorKind
  {
    too_large,
    too_small
  } error_kind;

  if (*iter != '-') {
    for (; iter != parse_last; ++iter) {
      ENEK_ASSERT('0' <= *iter);
      ENEK_ASSERT(*iter <= '9');
      char const value = *iter - '0';
      if (value_ > std::numeric_limits<std::int_fast64_t>::max() / 10) {
        initialized_ = true;
        error_ = true;
        value_ = 0;
        error_kind = ErrorKind::too_large;
        break;
      }
      value_ *= 10;
      if (value_ > std::numeric_limits<std::int_fast64_t>::max() - value) {
        initialized_ = true;
        error_ = true;
        value_ = 0;
        error_kind = ErrorKind::too_large;
        break;
      }
      value_ += value;
    }
  }
  else {
    ++iter;
    for (; iter != parse_last; ++iter) {
      ENEK_ASSERT('0' <= *iter);
      ENEK_ASSERT(*iter <= '9');
      char const value = *iter - '0';
      if (value_ < std::numeric_limits<std::int_fast64_t>::min() / 10) {
        initialized_ = true;
        error_ = true;
        value_ = 0;
        error_kind = ErrorKind::too_small;
        break;
      }
      value_ *= 10;
      if (value_ < std::numeric_limits<std::int_fast64_t>::min() + value) {
        initialized_ = true;
        error_ = true;
        value_ = 0;
        error_kind = ErrorKind::too_small;
        break;
      }
      value_ -= value;
    }
  }
  initialized_ = true;
  if (error_) {
    if (error_kind == ErrorKind::too_large) {
      Enek::FeatureTemplate::Parsing::makeMessagePrinter(
        path, text_range, parse_range, os)
        << "error: An integer literal is too large.";
    }
    else {
      Enek::FeatureTemplate::Parsing::makeMessagePrinter(
        path, text_range, parse_range, os)
        << "error: An integer literal is too small.";
    }
  }
}

#define ENEK_INSTANTIATE(...)                                                                  \
  template                                                                                     \
  void IntegerLiteral::initialize<                                                             \
    Enek::FeatureTemplate::Parsing::TextPositionIterator<__VA_ARGS__>,                         \
    __VA_ARGS__>(                                                                              \
      boost::iterator_range<                                                                   \
        Enek::FeatureTemplate::Parsing::TextPositionIterator<__VA_ARGS__>> const &parse_range, \
      Path const &path,                                                                        \
      boost::iterator_range<__VA_ARGS__> const &text_range,                                    \
      std::ostream &os)

ENEK_INSTANTIATE(std::string::const_iterator);

bool IntegerLiteral::succeed() const
{
  if (!this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "`succeed' is called on an uninitialized object.");
  }
  return !error_;
}

Enek::FeatureTemplate::Type IntegerLiteral::getType() const
{
  if (!this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "`getType' is called on an uninitialized object.");
  }
  return Enek::FeatureTemplate::Type::integer;
}

std::int_fast64_t IntegerLiteral::getValue() const
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

void IntegerLiteral::dumpXML(std::ostream &os) const
{
  if (!this->isInitialized()) {
    ENEK_THROW<std::invalid_argument>(
      "`dumpXML' is called on an uninitialized object.");
  }
  if (this->succeed()) {
    os << "<integer_literal>" << this->getValue() << "</integer_literal>";
  }
  else {
    os << "<integer_literal succeed=\"false\"/>";
  }
}

} // namespace Enek::FeatureTemplate::Parsing
