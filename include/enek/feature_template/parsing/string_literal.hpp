#if !defined(ENEK_FEATURE_TEMPLATE_PARSING_STRING_LITERAL_HPP_INCLUDE_GUARD)
#define ENEK_FEATURE_TEMPLATE_PARSING_STRING_LITERAL_HPP_INCLUDE_GUARD

#include <enek/feature_template/parsing/print_message.hpp>
#include <enek/unicode/utf8.hpp>
#include <enek/unicode/char.hpp>
#include <ostream>
#include <string>
#include <type_traits>


namespace Enek::FeatureTemplate::Parsing{

class StringLiteral
{
public:
  StringLiteral() noexcept;

  StringLiteral(StringLiteral const &);

  StringLiteral(StringLiteral &&) noexcept;

  bool isInitialized() const noexcept;

  template<typename Iterator, typename BaseIterator>
  void initialize(boost::iterator_range<Iterator> const &parse_range,
                  std::filesystem::path const &path,
                  boost::iterator_range<BaseIterator> const &text_range,
                  std::ostream &os)
  {
    static_assert(
      std::is_same<typename Iterator::BaseIterator, BaseIterator>::value);
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
      {
        ++iter;
        ENEK_ASSERT(iter != last);
        ENEK_ASSERT(('0' <= *iter && *iter <= '9')
                    || ('a' <= *iter && *iter <= 'f'));
        char32_t c = 0;
        bool overflow = false;
        for (;;) {
          char x = 0;
          if ('0' <= *iter && *iter <= '9') {
            x = *iter - '0';
          }
          else if ('a' <= *iter && *iter <= 'f') {
            x = (*iter - 'a') + 10;
          }
          else {
            break;
          }
          if (c > std::numeric_limits<char32_t>::max() / 16) {
            overflow = true;
          }
          c *= 16;
          if (c > std::numeric_limits<char32_t>::max() - x) {
            overflow = true;
          }
          c += x;
          ++iter;
          ENEK_ASSERT(iter != last);
          ENEK_ASSERT(*iter != '"');
        }
        if (overflow) {
          IteratorRange const escape_range(escape_first, iter);
          Enek::FeatureTemplate::Parsing::printMessage(
            path, text_range, escape_range,
            "error: A hexadecimal escape sequence is out of range.", os);
          error = true;
          break;
        }
        if (c > Enek::Unicode::getCharMaxValue()) {
          IteratorRange const escape_range(escape_first, iter);
          Enek::FeatureTemplate::Parsing::printMessage(
            path, text_range, escape_range,
            "error: A hexadecimal escape sequence exceeds the highest Unicode "
            "code point value.", os);
          error = true;
          break;
        }
        if (Enek::Unicode::GeneralCategory::isOtherSurrogate(c)) {
          IteratorRange const escape_range(escape_first, iter);
          Enek::FeatureTemplate::Parsing::printMessage(
            path, text_range, escape_range,
            "error: A hexadecimal escape sequence is a surrogate code point.",
            os);
          error = true;
          break;
        }
        Enek::Unicode::UTF8::appendCodePoint(value_, c);
        break;
      }
      case 'u':
      {
        std::size_t n = 4;
        ENEK_THROW<std::invalid_argument>("TODO: Unimplemented.");
      }
      case 'U':
      {
        std::size_t n = 8;
        ++iter;
        ENEK_ASSERT(iter != last);
        ENEK_ASSERT(('0' <= *iter && *iter <= '9')
                    || ('a' <= *iter && *iter <= 'f'));
        std::size_t i = 0;
        char32_t c = 0;
        if ('0' <= *iter && *iter <= '9') {
          c += *iter - '0';
        }
        else {
          c += (*iter - 'a') + 10;
        }
        for (++i, ++iter; i < n; ++i, ++iter) {
          c *= 16;
          ENEK_ASSERT(iter != last);
          ENEK_ASSERT(('0' <= *iter && *iter <= '9')
                      || ('a' <= *iter && *iter <= 'f'));
          if ('0' <= *iter && *iter <= '9') {
            c += *iter - '0';
          }
          else {
            c += (*iter - 'a') + 10;
          }
        }
        ENEK_ASSERT(iter != last);
        ENEK_ASSERT(*iter != '"');
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

  bool succeed() const noexcept;

  void swap(StringLiteral &) noexcept;

  friend void swap(StringLiteral &, StringLiteral &) noexcept;

  StringLiteral &operator=(StringLiteral const &);

  StringLiteral &operator=(StringLiteral &&) noexcept;

  std::string const &getValue() const noexcept;

  void dumpXML(std::ostream &) const;

private:
  bool initialized_;
  bool error_;
  std::string value_;
}; // class StringLiteral

} // namespace Enek::FeatureTemplate::Parsing

#endif // !defined(ENEK_FEATURE_TEMPLATE_PARSING_STRING_LITERAL_HPP_INCLUDE_GUARD)
