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
                  boost::iterator_range<Iterator> const &caret_range,
                  char const *message,
                  std::ostream &os)
{
  static_assert(
    std::is_same<BaseIterator, typename Iterator::BaseIterator>::value);
  BaseIterator const text_first = boost::begin(text_range);
  BaseIterator const text_last = boost::end(text_range);
  {
    using BaseTraversal
      = typename boost::iterator_traversal<BaseIterator>::type;
    constexpr bool base_iterator_is_random_access
      = std::is_convertible<BaseTraversal, boost::random_access_traversal_tag>::value;
    if constexpr (base_iterator_is_random_access) {
      if (text_first > text_last) {
        ENEK_THROW<std::invalid_argument>(
          "error: `boost::begin(text_range)' is not reachable to "
          "`boost::end(text_range)'.");
      }
    }
  }
  Iterator iter = boost::begin(caret_range);
  if (!iter.hasTextPosition()) {
    ENEK_THROW<std::invalid_argument>(
      "error: `boost::begin(caret_range)' does not have any text position.");
  }
  if (message == nullptr) {
    ENEK_THROW<std::invalid_argument>(
      "error: The argument `message' is the null pointer.");
  }
  if (!path.empty()) {
    os << path << ':';
  }
  std::size_t column = iter.getColumnNumber();
  os << iter.getLineNumber() + 1 << ':' << column + 1 << ": " << message
     << '\n';
  Iterator const caret_last = boost::end(caret_range);
  BaseIterator line_first = iter.getLineFirstPosition();
  for (;;) {
    {
      BaseIterator const base_last = iter.getBaseIterator();
      for (BaseIterator jter = line_first; jter != base_last; ++jter) {
        os << *jter;
      }
    }
    while (iter != caret_last && *iter != '\r' && *iter != '\n') {
      os << *iter++;
    }
    if (iter == caret_last) {
      BaseIterator const base_last = iter.getLineLastPosition(text_last);
      for (BaseIterator jter = iter.getBaseIterator(); jter != base_last; ++jter) {
        os << *jter;
      }
    }
    os << '\n';
    std::size_t const caret_last_column = iter.getColumnNumber();
    {
      std::size_t i = 0;
      for (; i < column; ++i) {
        os << ' ';
      }
      for (; i < caret_last_column; ++i) {
        os << '^';
      }
    }
    os << '\n';
    if (iter == caret_last) {
      break;
    }
    ENEK_ASSERT(*iter == '\r' || *iter == '\n');
    if (*iter == '\r') {
      ++iter;
      if (iter == caret_last) {
        break;
      }
      if (*iter == '\n') {
        ++iter;
        if (iter == caret_last) {
          break;
        }
      }
    }
    else if (*iter == '\n') {
      ++iter;
      if (iter == caret_last) {
        break;
      }
    }
    line_first = iter.getBaseIterator();
    column = 0;
  }
}


template<typename BaseIterator, typename Iterator>
void printMessage(std::filesystem::path const &path,
                  boost::iterator_range<BaseIterator> const &text_range,
                  boost::iterator_range<Iterator> const &caret_range,
                  std::string const &message,
                  std::ostream &os)
{
  printMessage(path, text_range, caret_range, message.c_str(), os);
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
    IteratorRange const &caret_range,
    std::ostream &os)
    : path_(path)
    , text_range_(text_range)
    , caret_range_(caret_range)
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
      path_, text_range_, caret_range_, oss_.str(), os_);
  }

private:
  Path path_;
  BaseIteratorRange text_range_;
  IteratorRange caret_range_;
  std::ostringstream oss_;
  std::ostream &os_;
}; // class MessagePrinter

template<typename BaseIterator, typename Iterator>
MessagePrinter<Iterator> makeMessagePrinter(
  std::filesystem::path const &path,
  boost::iterator_range<BaseIterator> const &text_range,
  boost::iterator_range<Iterator> const &caret_range,
  std::ostream &os)
{
  static_assert(
    std::is_same<BaseIterator, typename Iterator::BaseIterator>::value);
  return { path, text_range, caret_range, os };
}

} // namespace Enek::FeatureTemplate::Parsing

#endif // !defined(ENEK_FEATURE_TEMPLATE_PARSING_PRINT_MESSAGE_HPP_INCLUDE_GUARD)
