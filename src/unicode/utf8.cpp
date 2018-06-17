#include <enek/unicode/utf8.hpp>
#include <enek/unicode/char.hpp>
#include <enek/unicode/icu_error.hpp>
#include <enek/util/throw.hpp>
#include <enek/util/assert.hpp>
#include <unicode/ubrk.h>
#include <unicode/uloc.h>
#include <unicode/utf8.h>
#include <unicode/ustring.h>
#include <unicode/utypes.h>
#include <unicode/umachine.h>
#include <ios>
#include <string>
#include <memory>
#include <system_error>
#include <stdexcept>
#include <cstdint>
#include <cstddef>

#include <boost/io/ios_state.hpp>
#include <sstream>


namespace Enek::Unicode::UTF8{

void appendCodePoint(std::string &s, char32_t c)
{
  using std::placeholders::_1;
  if (c > Enek::Unicode::getCharMaxValue()) {
    ENEK_THROW<std::invalid_argument>(_1)
      << "The argument `c' (= `U+" << std::hex << std::uppercase
      << static_cast<std::uint_fast32_t>(c)
      << "') exceeds the highest Unicode code point value.";
  }
  constexpr std::size_t max_length = U8_MAX_LENGTH;
  std::size_t const old_size = s.size();
  s.resize(old_size + max_length);
  char *p = s.data();
  std::size_t i = old_size;
  UBool is_error = false;
  U8_APPEND(p, i, s.size(), c, is_error);
  if (is_error) {
    ENEK_THROW<std::logic_error>(_1)
      << "Failed to append the Unicode code point `U+" << std::hex
      << std::uppercase << static_cast<std::uint_fast32_t>(c) << "'.";
  }
  ENEK_ASSERT(old_size < i && i <= s.size());
  s.resize(i);
}

std::size_t getWidthOnConsole(std::string const &str)
{
  using std::placeholders::_1;
  std::size_t const length = [](std::string const &source) -> std::size_t {
    std::int32_t length_;
    UErrorCode error_code = UErrorCode::U_ZERO_ERROR;
    u_strFromUTF8(
      nullptr, 0, &length_, source.data(), source.size(), &error_code);
    if (U_FAILURE(error_code)) {
      if (error_code == UErrorCode::U_INVALID_CHAR_FOUND) {
        ENEK_THROW<Enek::Unicode::UTF8::InvalidStringError>(
          "The argument `str' is not a valid UTF-8 string.");
      }
      if (error_code != UErrorCode::U_BUFFER_OVERFLOW_ERROR) {
        ENEK_THROW<std::system_error>(
          static_cast<int>(error_code), Enek::Unicode::getICUErrorCategory());
      }
    }
    return static_cast<std::size_t>(length_);
  }(str);

  std::unique_ptr<UChar[]> const p_ustr = std::make_unique<UChar[]>(length);
  {
    std::int32_t length_;
    UErrorCode error_code = UErrorCode::U_ZERO_ERROR;
    u_strFromUTF8(
      p_ustr.get(), length, &length_, str.data(), str.size(), &error_code);
    ENEK_ASSERT(length_ == length);
    if (U_FAILURE(error_code)) {
      // LCOV_EXCL_START
      if (error_code == UErrorCode::U_INVALID_CHAR_FOUND) {
        ENEK_THROW<std::logic_error>(
          "An already validated UTF-8 string is deemed invalid.");
      }
      ENEK_THROW<std::system_error>(
        static_cast<int>(error_code), Enek::Unicode::getICUErrorCategory());
      // LCOV_EXCL_STOP
    }
  }

  using UBreakIteratorPtr
    = std::unique_ptr<UBreakIterator, void (*)(UBreakIterator *)>;
  auto const p_ubrk = [&]() -> UBreakIteratorPtr {
    UErrorCode error_code = UErrorCode::U_ZERO_ERROR;
    UBreakIteratorPtr p(
      ubrk_open(UBreakIteratorType::UBRK_CHARACTER,
                uloc_getDefault(),
                p_ustr.get(),
                length,
                &error_code),
      &ubrk_close);
    if (U_FAILURE(error_code)) {
      ENEK_THROW<std::system_error>(
        static_cast<int>(error_code), Enek::Unicode::getICUErrorCategory());
    }
    return p;
  }();

  std::size_t width = 0;
  {
    std::size_t prev = ubrk_current(p_ubrk.get());
    ENEK_ASSERT(prev == 0);
    for (;;) {
      std::size_t const curr = ubrk_next(p_ubrk.get());
      if (curr == UBRK_DONE) {
        break;
      }
      std::size_t i = prev;
      {
        UChar32 c;
        U16_NEXT(p_ustr.get(), i, curr, c);
        if (Enek::Unicode::GeneralCategory::isOtherSurrogate(c)) {
          // LCOV_EXCL_START
          ENEK_THROW<std::logic_error>(
            "`UBreakIterator' stops the point next to a high surrogate.");
          // LCOV_EXCL_STOP
        }
        switch (Enek::Unicode::getEastAsianWidth(c)) {
        case EastAsianWidth::fullwidth:
          width += 2;
          break;
        case EastAsianWidth::halfwidth:
          width += 1;
          break;
        case EastAsianWidth::wide:
          width += 2;
          break;
        case EastAsianWidth::narrow:
          width += 1;
          break;
        case EastAsianWidth::ambiguous:
          width += 1;
          break;
        case EastAsianWidth::neutral:
          width += 1;
          break;
          // LCOV_EXCL_START
        default:
          ENEK_THROW<std::logic_error>(_1)
            << "An unknown `EastAsianWidth' value `" << c << "'.";
          // LCOV_EXCL_STOP
        }
      }
      while (i < curr) {
        UChar32 c;
        U16_NEXT(p_ustr.get(), i, curr, c);
        if (Enek::Unicode::GeneralCategory::isOtherSurrogate(c)) {
          // LCOV_EXCL_START
          ENEK_THROW<std::logic_error>(
            "`UBreakIterator' stops the point next to a high surrogate.");
          // LCOV_EXCL_STOP
        }
        Enek::Unicode::EastAsianWidth const eaw
          = Enek::Unicode::getEastAsianWidth(c);
        if (eaw != EastAsianWidth::neutral && eaw != EastAsianWidth::ambiguous) {
          // LCOV_EXCL_START
          ENEK_THROW<std::runtime_error>(_1)
            << "A character defined as other than `East Asian Neutral' and "
               "`East Asian Ambiguous' (see UAX #11 for more detail) is found "
               "in the " << i << "th code point of a grapheme cluster `"
            << Enek::Unicode::formatAsCodePoint(&p_ustr[prev], &p_ustr[curr])
            << "' (" << eaw << ").";
          // LCOV_EXCL_STOP
        }
      }
      ENEK_ASSERT(i == curr) << "i = " << i << ", curr = " << curr;
      prev = curr;
    }
  }
  return width;
}

} // namespace Enek::Unicode::UTF8
