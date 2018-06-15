#if !defined(ENEK_FEATURE_TEMPLATE_PARSING_INTEGER_LITERAL_HPP_INCLUDE_GUARD)
#define ENEK_FEATURE_TEMPLATE_PARSING_INTEGER_LITERAL_HPP_INCLUDE_GUARD

#include <enek/feature_template/parsing/print_message.hpp>
#include <enek/util/assert.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/end.hpp>
#include <boost/range/begin.hpp>
#include <filesystem>
#include <ostream>
#include <type_traits>
#include <cstdint>
#include <limits>

namespace Enek::FeatureTemplate::Parsing{

class IntegerLiteral
{
public:
  using Path = std::filesystem::path;

public:
  IntegerLiteral() noexcept;

  IntegerLiteral(IntegerLiteral const &rhs) noexcept;

  bool isInitialized() const noexcept;

  template<typename Iterator, typename BaseIterator>
  void initialize(
    boost::iterator_range<Iterator> const &parse_range,
    Path const &path,
    boost::iterator_range<BaseIterator> const &text_range,
    std::ostream &os) noexcept
  {
    static_assert(
      std::is_same<typename Iterator::BaseIterator, BaseIterator>::value);
    ENEK_ASSERT(!initialized_);
    ENEK_ASSERT(!error_);
    ENEK_ASSERT(value_ == 0);
    ENEK_ASSERT(boost::begin(parse_range) != boost::end(parse_range));
    Iterator iter = boost::begin(parse_range);
    if (!iter.holdsTextPosition()) {
      ENEK_THROW(std::invalid_argument)
        << "The argument `parse_range' does not have any text position.";
    }
    Iterator const parse_last = boost::end(parse_range);
    if (*iter != '-') {
      for (; iter != parse_last; ++iter) {
        ENEK_ASSERT('0' <= *iter);
        ENEK_ASSERT(*iter <= '9');
        char const value = *iter - '0';
        if (value_ > std::numeric_limits<std::int_fast64_t>::max() / 10) {
          initialized_ = true;
          error_ = true;
          value_ = 0;
          break;
        }
        value_ *= 10;
        if (value_ > std::numeric_limits<std::int_fast64_t>::max() - value) {
          initialized_ = true;
          error_ = true;
          value_ = 0;
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
          break;
        }
        value_ *= 10;
        if (value_ < std::numeric_limits<std::int_fast64_t>::min() + value) {
          initialized_ = true;
          error_ = true;
          value_ = 0;
          break;
        }
        value_ -= value;
      }
    }
    initialized_ = true;
    if (error_) {
      Enek::FeatureTemplate::Parsing::makeMessagePrinter(
        path, text_range, parse_range, os)
        << "error: An integer literal is too large. "
           "It should be within the range ["
        << std::numeric_limits<std::int_fast64_t>::min() << ", "
        << std::numeric_limits<std::int_fast64_t>::max() << "].";
    }
  }

  bool succeed() const noexcept;

  std::int_fast64_t getValue() const noexcept;

  void dumpXML(std::ostream &os) const;

private:
  bool initialized_;
  bool error_;
  std::int_fast64_t value_;
}; // class IntegerLiteral

} // namespace Enek::FeatureTemplate::Parsing

#endif // !defined(ENEK_FEATURE_TEMPLATE_PARSING_INTEGER_LITERAL_HPP_INCLUDE_GUARD)
