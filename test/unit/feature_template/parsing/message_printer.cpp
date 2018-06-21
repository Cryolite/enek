#include <enek/feature_template/parsing/message_printer.hpp>
#include <enek/feature_template/parsing/text_position_iterator.hpp>
#include <gtest/gtest.h>
#include <boost/range/iterator_range.hpp>
#include <boost/exception/exception.hpp>
#include <filesystem>
#include <sstream>
#include <iterator>
#include <string>
#include <stdexcept>
#include <cstddef>


namespace{

using Path = std::filesystem::path;

template<typename Iterator>
using TextPositionIterator
  = Enek::FeatureTemplate::Parsing::TextPositionIterator<Iterator>;

} // namespace *unnamed*

TEST(FeatureTemplateParsingMessagePrinterTest, testInvalidRange)
{
  std::string str("Hello, world!");
  using BaseIterator = std::string::const_iterator;
  using BaseIteratorRange = boost::iterator_range<BaseIterator>;
  BaseIteratorRange invalid_text_range(str.cend(), str.cbegin());
  using Iterator = TextPositionIterator<BaseIterator>;
  using IteratorRange = boost::iterator_range<Iterator>;
  IteratorRange caret_range
    = Iterator::makeIteratorRange(str.cbegin(), str.cbegin());
  std::ostringstream oss;
  EXPECT_THROW(
    printMessage(Path(), invalid_text_range, caret_range, "", oss),
    std::invalid_argument);
  EXPECT_THROW(
    printMessage(Path(), invalid_text_range, caret_range, "", oss),
    boost::exception);
}

TEST(FeatureTemplateParsingMessagePrinterTest, testNoTextPosition)
{
  std::string str("Hello, world!");
  using BaseIterator = std::string::const_iterator;
  using BaseIteratorRange = boost::iterator_range<BaseIterator>;
  BaseIteratorRange text_range(str.cbegin(), str.cend());
  using Iterator = TextPositionIterator<BaseIterator>;
  using IteratorRange = boost::iterator_range<Iterator>;
  IteratorRange caret_range
    = Iterator::makeIteratorRange(str.cbegin(), str.cend());
  IteratorRange invalid_caret_range(std::end(caret_range),
                                    std::end(caret_range));
  std::ostringstream oss;
  EXPECT_THROW(
    printMessage(Path(), text_range, invalid_caret_range, "", oss),
    std::invalid_argument);
  EXPECT_THROW(
    printMessage(Path(), text_range, invalid_caret_range, "", oss),
    boost::exception);
}

TEST(FeatureTemplateParsingMessagePrinterTest, testNullMessage)
{
  std::string str("Hello, world!");
  using BaseIterator = std::string::const_iterator;
  using BaseIteratorRange = boost::iterator_range<BaseIterator>;
  BaseIteratorRange text_range(str.cbegin(), str.cend());
  using Iterator = TextPositionIterator<BaseIterator>;
  using IteratorRange = boost::iterator_range<Iterator>;
  IteratorRange caret_range = Iterator::makeIteratorRange(str);
  std::ostringstream oss;
  EXPECT_THROW(
    printMessage(Path(), text_range, caret_range, nullptr, oss),
    std::invalid_argument);
  EXPECT_THROW(
    printMessage(Path(), text_range, caret_range, nullptr, oss),
    boost::exception);
}

namespace{

void test(char const *file_name,
          int line,
          std::filesystem::path const &path,
          std::string const &text,
          std::size_t first,
          std::size_t last,
          std::string const &message,
          char const *message_to_be_expected)
{
  using BaseIterator = std::string::const_iterator;
  using BaseIteratorRange = boost::iterator_range<BaseIterator>;
  BaseIteratorRange text_range(text.cbegin(), text.cend());
  using Iterator = TextPositionIterator<BaseIterator>;
  using IteratorRange = boost::iterator_range<Iterator>;
  IteratorRange caret_range
    = Iterator::makeIteratorRange(text.cbegin(), text.cbegin() + last);
  caret_range = IteratorRange(std::next(std::begin(caret_range), first),
                              std::end(caret_range));
  std::ostringstream oss;
  printMessage(path, text_range, caret_range, message, oss);
  EXPECT_STREQ(message_to_be_expected, oss.str().c_str())
    << file_name << ":" << line << ": Called from this line.";
}

} // namespace *unnamed*

TEST(FeatureTemplateParsingMessagePrinterTest, testLineFeed)
{
  test(__FILE__,
       __LINE__,
       "filename.txt",
       "First line.\nSecond line.", 6, 11,
       "error: An error message.",
       "filename.txt:1:7: error: An error message.\n"
       "First line.\n"
       "      ^^^^^\n");
  test(__FILE__,
       __LINE__,
       "filename.txt",
       "First line.\nSecond line.", 6, 12,
       "error: An error message.",
       "filename.txt:1:7: error: An error message.\n"
       "First line.\n"
       "      ^^^^^\n");
  test(__FILE__,
       __LINE__,
       "filename.txt",
       "First line.\nSecond line.", 6, 18,
       "error: An error message.",
       "filename.txt:1:7: error: An error message.\n"
       "First line.\n"
       "      ^^^^^\n"
       "Second line.\n"
       "^^^^^^\n");
}

TEST(FeatureTemplateParsingMessagePrinterTest, testCarriageReturn)
{
  test(__FILE__,
       __LINE__,
       "filename.txt",
       "First line.\rSecond line.", 6, 11,
       "error: An error message.",
       "filename.txt:1:7: error: An error message.\n"
       "First line.\n"
       "      ^^^^^\n");
  test(__FILE__,
       __LINE__,
       "filename.txt",
       "First line.\rSecond line.", 6, 12,
       "error: An error message.",
       "filename.txt:1:7: error: An error message.\n"
       "First line.\n"
       "      ^^^^^\n");
  test(__FILE__,
       __LINE__,
       "filename.txt",
       "First line.\rSecond line.", 6, 18,
       "error: An error message.",
       "filename.txt:1:7: error: An error message.\n"
       "First line.\n"
       "      ^^^^^\n"
       "Second line.\n"
       "^^^^^^\n");
}

TEST(FeatureTemplateParsingMessagePrinterTest, testCarriageReturnLineFeed)
{
  test(__FILE__,
       __LINE__,
       "filename.txt",
       "First line.\r\nSecond line.", 6, 11,
       "error: An error message.",
       "filename.txt:1:7: error: An error message.\n"
       "First line.\n"
       "      ^^^^^\n");
  test(__FILE__,
       __LINE__,
       "filename.txt",
       "First line.\r\nSecond line.", 6, 12,
       "error: An error message.",
       "filename.txt:1:7: error: An error message.\n"
       "First line.\n"
       "      ^^^^^\n");
  test(__FILE__,
       __LINE__,
       "filename.txt",
       "First line.\r\nSecond line.", 6, 13,
       "error: An error message.",
       "filename.txt:1:7: error: An error message.\n"
       "First line.\n"
       "      ^^^^^\n");
  test(__FILE__,
       __LINE__,
       "filename.txt",
       "First line.\r\nSecond line.", 6, 19,
       "error: An error message.",
       "filename.txt:1:7: error: An error message.\n"
       "First line.\n"
       "      ^^^^^\n"
       "Second line.\n"
       "^^^^^^\n");
}
