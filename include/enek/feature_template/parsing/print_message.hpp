#if !defined(ENEK_FEATURE_TEMPLATE_PARSING_PRINT_MESSAGE_HPP_INCLUDE_GUARD)
#define ENEK_FEATURE_TEMPLATE_PARSING_PRINT_MESSAGE_HPP_INCLUDE_GUARD

#include <enek/util/throw.hpp>
#include <enek/util/assert.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/end.hpp>
#include <boost/range/begin.hpp>
#include <filesystem>
#include <ostream>
#include <stdexcept>
#include <cstddef>


namespace Enek::FeatureTemplate::Parsing{

template<typename BaseIterator, typename Iterator>
void printMessage(std::filesystem::path const &path,
                  boost::iterator_range<BaseIterator> const &text_range,
                  boost::iterator_range<Iterator> const &outer_caret_range,
                  boost::iterator_range<Iterator> const &inner_caret_range,
                  char const *message,
                  std::ostream &os)
{
  static_assert(
    std::is_same<BaseIterator, typename Iterator::BaseIterator>::value);
  if (!path.empty()) {
    os << path << ':';
  }
  BaseIterator const text_last = boost::end(text_range);
  Iterator const outer_caret_first = boost::begin(outer_caret_range);
  if (!outer_caret_first.holdsTextPosition()) {
    ENEK_THROW(std::logic_error)
      << "`boost::begin(outer_caret_range)' does not hold any text position.";
  }
  std::size_t const first_line = outer_caret_first.getLineNumber();
  std::size_t const first_column = outer_caret_first.getColumnNumber();
  os << first_line + 1 << ':' << first_column + 1 << ": " << message << '\n';

  Iterator const outer_caret_last = boost::end(outer_caret_range);
  if (!outer_caret_last.holdsTextPosition()) {
    ENEK_THROW(std::logic_error)
      << "`boost::end(outer_caret_range)' does not hold any text position.";
  }
  std::size_t const last_line = outer_caret_last.getLineNumber();
  if (last_line < first_line) {
    ENEK_THROW(std::logic_error) << "`outer_caret_range' is not valid.";
  }
  std::size_t const last_column = outer_caret_last.getColumnNumber();
  if (first_line == last_line
      || (first_line + 1 == last_line && last_column == 0)) {
    BaseIterator const line_first = outer_caret_first.getLineFirstPosition();
    BaseIterator const line_last
      = outer_caret_last.getLineLastPosition(text_last);
    for (BaseIterator iter = line_first; iter != line_last; ++iter) {
      os << *iter;
    }
    os << '\n';
    BaseIterator iter = line_first;
    BaseIterator const outer_caret_first_base
      = outer_caret_first.getBaseIterator();
    for (; iter != outer_caret_first_base; ++iter) {
      os << ' ';
    }
    Iterator const inner_caret_first = boost::begin(inner_caret_range);
    BaseIterator const inner_caret_first_base
      = inner_caret_first.getBaseIterator();
    for (; iter != inner_caret_first_base; ++iter) {
      os << '~';
    }
    Iterator const inner_caret_last = boost::end(inner_caret_range);
    BaseIterator const inner_caret_last_base
      = inner_caret_last.getBaseIterator();
    for (; iter != inner_caret_last_base; ++iter) {
      os << '^';
    }
    BaseIterator const outer_caret_last_base
      = outer_caret_last.getBaseIterator();
    for (; iter != outer_caret_last_base; ++iter) {
      os << '~';
    }
    os << std::endl;
    return;
  }

  ENEK_ASSERT(first_line < last_line);
  Iterator const inner_caret_first = boost::begin(inner_caret_range);
  BaseIterator const inner_caret_first_base
    = inner_caret_first.getBaseIterator();
  Iterator const inner_caret_last = boost::end(inner_caret_range);
  BaseIterator const inner_caret_last_base
    = inner_caret_last.getBaseIterator();
  char caret = '~';
  BaseIterator iter = outer_caret_first.getLineFirstPosition();
  {
    BaseIterator jter = iter;
    for (; (*iter != '\r') && (*iter != '\n'); ++iter) {
      os << *iter;
    }
    BaseIterator const line_last = iter;
    {
      char const prev = *iter++;
      if ((prev == '\r' && *iter == '\n') || (prev == '\n' && *iter == '\r')) {
        ++iter;
      }
      os << '\n';
    }
    BaseIterator const outer_caret_first_base
      = outer_caret_first.getBaseIterator();
    for (; jter != outer_caret_first_base; ++jter) {
      os << ' ';
    }
    for (; jter != line_last; ++jter) {
      if (jter == inner_caret_first_base) {
        caret = '^';
      }
      if (jter == inner_caret_last_base) {
        caret = '~';
      }
      os << caret;
    }
  }
  for (std::size_t line = first_line + 1; line < last_line; ++line) {
    BaseIterator jter = iter;
    for (; (*iter != '\r') && (*iter != '\n'); ++iter) {
      os << *iter;
    }
    BaseIterator const line_last = iter;
    {
      char const prev = *iter++;
      if ((prev == '\r' && *iter == '\n') || (prev == '\n' && *iter == '\r')) {
        ++iter;
      }
      os << '\n';
    }
    for (; jter != line_last; ++jter) {
      if (jter == inner_caret_first_base) {
        caret = '^';
      }
      if (jter == inner_caret_last_base) {
        caret = '~';
      }
      os << caret;
    }
  }
  {
    BaseIterator jter = iter;
    for (; iter != text_last; ++iter) {
      if ((*iter == '\r') || (*iter == '\n')) {
        break;
      }
      os << *iter;
    }
    BaseIterator const outer_caret_last_base
      = outer_caret_last.getBaseIterator();
    for (; jter != outer_caret_last_base; ++jter) {
      if (jter == inner_caret_first_base) {
        caret = '^';
      }
      if (iter == inner_caret_last_base) {
        caret = '~';
      }
      os << caret;
    }
  }
}

template<typename BaseIterator, typename Iterator>
void printMessage(std::filesystem::path const &path,
                  boost::iterator_range<BaseIterator> const &text_range,
                  boost::iterator_range<Iterator> const &outer_caret_range,
                  boost::iterator_range<Iterator> const &inner_caret_range,
                  std::string const &message,
                  std::ostream &os)
{
  Enek::FeatureTemplate::Parsing::printMessage(
    path, text_range, outer_caret_range, inner_caret_range, message.c_str(), os);
}

template<typename Iterator>
class MessagePrinter
{
private:
  using BaseIterator = typename Iterator::BaseIterator;

public:
  using Path = std::filesystem::path;

  using BaseIteratorRange = boost::iterator_range<BaseIterator>;

  using IteratorRange = boost::iterator_range<Iterator>;

public:
  MessagePrinter(
    Path const &path,
    BaseIteratorRange const &text_range,
    IteratorRange const &outer_caret_range,
    IteratorRange const &inner_caret_range,
    std::ostream &os)
    : path_(path)
    , text_range_(text_range)
    , outer_caret_range_(outer_caret_range)
    , inner_caret_range_(inner_caret_range)
    , oss_()
    , os_(os)
  {}

  MessagePrinter(MessagePrinter const &) = delete;

  MessagePrinter &operator=(MessagePrinter const &) = delete;

  template<typename T>
  std::ostream &operator<<(T &&value)
  {
    return oss_ << std::forward<T>(value);
  }

  ~MessagePrinter() noexcept(false)
  {
    Enek::FeatureTemplate::Parsing::printMessage(
      path_, text_range_, outer_caret_range_, inner_caret_range_, oss_.str(), os_);
  }

private:
  Path path_;
  BaseIteratorRange text_range_;
  IteratorRange outer_caret_range_;
  IteratorRange inner_caret_range_;
  std::ostringstream oss_;
  std::ostream &os_;
}; // class MessagePrinter

template<typename BaseIterator, typename Iterator>
MessagePrinter<Iterator> makeMessagePrinter(
  std::filesystem::path const &path,
  boost::iterator_range<BaseIterator> const &text_range,
  boost::iterator_range<Iterator> const &outer_caret_range,
  boost::iterator_range<Iterator> const &inner_caret_range,
  std::ostream &os)
{
  static_assert(
    std::is_same<BaseIterator, typename Iterator::BaseIterator>::value);
  return { path, text_range, outer_caret_range, inner_caret_range, os };
}

template<typename BaseIterator, typename Iterator>
MessagePrinter<Iterator> makeMessagePrinter(
  std::filesystem::path const &path,
  boost::iterator_range<BaseIterator> const &text_range,
  boost::iterator_range<Iterator> const &caret_range,
  std::ostream &os)
{
  static_assert(
    std::is_same<BaseIterator, typename Iterator::BaseIterator>::value);
  return { path, text_range, caret_range, caret_range, os };
}

} // namespace Enek::FeatureTemplate::Parsing

#endif // !defined(ENEK_FEATURE_TEMPLATE_PARSING_PRINT_MESSAGE_HPP_INCLUDE_GUARD)
