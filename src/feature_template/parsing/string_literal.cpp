#include <enek/feature_template/parsing/string_literal.hpp>
#include <enek/feature_template/parsing/message_printer.hpp>
#include <enek/feature_template/parsing/text_position_iterator.hpp>
#include <enek/feature_template/type.hpp>
#include <enek/unicode/utf8.hpp>
#include <enek/unicode/char.hpp>
#include <enek/util/throw.hpp>
#include <enek/util/assert.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/end.hpp>
#include <boost/range/begin.hpp>
#include <filesystem>
#include <ostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <stdexcept>
#include <limits>
#include <cstddef>


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

namespace{

template<typename BaseIterator, typename Iterator>
char32_t parseHexadecimalEscapeSequence(
  std::filesystem::path const &path,
  boost::iterator_range<BaseIterator> const &text_range,
  Iterator const &escape_first,
  Iterator &iter,
  std::size_t n,
  std::ostream &os)
{
  ENEK_ASSERT(('0' <= *iter && *iter <= '9')
              || ('a' <= *iter && *iter <= 'f')
              || ('A' <= *iter && *iter <= 'F'));
  char32_t c = 0;
  bool overflow = false;
  for (std::size_t i = 0; i < n; ++i) {
    char x;
    if ('0' <= *iter && *iter <= '9') {
      x = *iter - '0';
    }
    else if ('a' <= *iter && *iter <= 'f') {
      x = (*iter - 'a') + 10;
    }
    else if ('A' <= *iter && *iter <= 'F') {
      x = (*iter - 'A') + 10;
    }
    else {
      ENEK_ASSERT(i == 0 || n == std::numeric_limits<std::size_t>::max());
      break;
    }
    if (c > std::numeric_limits<char32_t>::max() / 16) {
      overflow = true;
    }
    c *= 16;
    if (c > std::numeric_limits<char32_t>::max() - x) {
      overflow = true;
    }
    if (overflow) {
      ++iter;
      continue;
    }
    c += x;
    ++iter;
  }
  if (overflow) {
    boost::iterator_range<Iterator> caret_range(escape_first, iter);
    if (*std::next(escape_first) == 'x') {
      Enek::FeatureTemplate::Parsing::printMessage(
        path, text_range, caret_range,
        "error: A hexadecimal escape sequence is too large.", os);
    }
    else {
      ENEK_THROW<std::logic_error>("A universal character name overflows.");
    }
    return std::numeric_limits<char32_t>::max();
  }
  if (c > Enek::Unicode::getCharMaxValue()) {
    boost::iterator_range<Iterator> caret_range(escape_first, iter);
    if (*std::next(escape_first) == 'x') {
      Enek::FeatureTemplate::Parsing::printMessage(
        path, text_range, caret_range,
        "error: A hexadecimal escape sequence exceeds the highest Unicode "
        "code point value.", os);
    }
    else {
      Enek::FeatureTemplate::Parsing::printMessage(
        path, text_range, caret_range,
        "error: A universal character name exceeds the highest Unicode code "
        "point value.", os);
    }
    return std::numeric_limits<char32_t>::max();
  }
  if (Enek::Unicode::GeneralCategory::isOtherSurrogate(c)) {
    boost::iterator_range<Iterator> caret_range(escape_first, iter);
    if (*std::next(escape_first) == 'x') {
      Enek::FeatureTemplate::Parsing::printMessage(
        path, text_range, caret_range,
        "error: A hexadecimal escape sequence is a Unicode surrogate code "
        "point.", os);
    }
    else {
      Enek::FeatureTemplate::Parsing::printMessage(
        path, text_range, caret_range,
        "error: A universal character name is a Unicode surrogate code point.",
        os);
    }
    return std::numeric_limits<char32_t>::max();
  }
  return c;
}

} // namespace *unnamed*

template<typename Iterator, typename BaseIterator>
void StringLiteral::initialize(
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
  using IteratorRange = boost::iterator_range<Iterator>;
  Iterator iter = boost::begin(parse_range);
  Iterator const last = boost::end(parse_range);
  ENEK_ASSERT(iter != last);
  ENEK_ASSERT(*iter == '"');
  ++iter;
  bool error = false;
  for (;;) {
    ENEK_ASSERT(iter != last);
    if (error || *iter == '"') {
      break;
    }
    if (*iter != '\\') {
      value_.push_back(*iter);
      ++iter;
      continue;
    }
    Iterator escape_first = iter;
    ++iter;
    ENEK_ASSERT(iter != last);
    switch (*iter) {
    case '\'':
      value_.push_back('\'');
      ++iter;
      continue;
    case '"':
      value_.push_back('"');
      ++iter;
      break;
    case '?':
      value_.push_back('?');
      ++iter;
      break;
    case '\\':
      value_.push_back('\\');
      ++iter;
      break;
    case 'a':
      value_.push_back('\a');
      ++iter;
      break;
    case 'b':
      value_.push_back('\b');
      ++iter;
      break;
    case 'f':
      value_.push_back('\f');
      ++iter;
      break;
    case 'n':
      value_.push_back('\n');
      ++iter;
      break;
    case 'r':
      value_.push_back('\r');
      ++iter;
      break;
    case 't':
      value_.push_back('\t');
      ++iter;
      break;
    case 'v':
      value_.push_back('\v');
      ++iter;
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    {
      char32_t c = *iter++ - '0';
      ENEK_ASSERT(iter != last);
      if ('0' <= *iter && *iter < '8') {
        c *= 8;
        c += *iter++ - '0';
        ENEK_ASSERT(iter != last);
        if ('0' <= *iter && *iter < '8') {
          c *= 8;
          c += *iter++ - '0';
          ENEK_ASSERT(iter != last);
        }
      }
      ENEK_ASSERT(c <= Enek::Unicode::getCharMaxValue());
      ENEK_ASSERT(!Enek::Unicode::GeneralCategory::isOtherSurrogate(c));
      value_.push_back(c);
      break;
    }
    case 'x':
    case 'u':
    case 'U':
    {
      std::size_t n = (*iter == 'x') ? std::numeric_limits<std::size_t>::max()
                                     : (*iter == 'u') ? 4 : 8;
      ++iter;
      char32_t c = parseHexadecimalEscapeSequence(
        path, text_range, escape_first, iter, n, os);
      if (c == std::numeric_limits<char32_t>::max()) {
        error = true;
        break;
      }
      Enek::Unicode::UTF8::appendCodePoint(value_, c);
      break;
    }
    default:
      ENEK_THROW<std::logic_error>("");
    }
  }
  initialized_ = true;
  if (!error) {
    ++iter;
    ENEK_ASSERT(iter == last);
    return;
  }
  error_ = true;
  value_.clear();
}

#define ENEK_INSTANTIATE(...)                                            \
  template                                                               \
  void StringLiteral::initialize<                                        \
    Enek::FeatureTemplate::Parsing::TextPositionIterator<__VA_ARGS__>,   \
    __VA_ARGS__>(                                                        \
    boost::iterator_range<                                               \
      Enek::FeatureTemplate::Parsing::TextPositionIterator<__VA_ARGS__>  \
    > const &parse_range,                                                \
    Path const &path,                                                    \
    boost::iterator_range<__VA_ARGS__> const &text_range,                \
    std::ostream &os)

ENEK_INSTANTIATE(std::string::const_iterator);

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
