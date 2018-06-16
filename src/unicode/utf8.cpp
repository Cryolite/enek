#include <enek/unicode/utf8.hpp>
#include <enek/unicode/char.hpp>
#include <enek/util/throw.hpp>
#include <enek/util/assert.hpp>
#include <unicode/utf8.h>
#include <ios>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <cstddef>


namespace Enek::Unicode{

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

} // namespace Enek::Unicode
