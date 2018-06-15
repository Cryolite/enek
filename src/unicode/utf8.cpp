#include <enek/unicode/utf8.hpp>

#include <enek/unicode/char.hpp>
#include <enek/util/throw.hpp>
#include <enek/util/assert.hpp>
#include <unicode/utf8.h>
#include <string>
#include <stdexcept>
#include <cstddef>


namespace Enek::Unicode{

void appendCodePoint(std::string &s, char32_t c)
{
  if (c > Enek::Unicode::getCharMaxValue()) {
    ENEK_THROW(std::invalid_argument)
      << "`c' exceeds the highest Unicode code point value.";
  }
  constexpr std::size_t max_length = U8_MAX_LENGTH;
  std::size_t const old_size = s.size();
  s.resize(s.size() + max_length);
  char *p = s.data();
  std::size_t i = old_size;
  UBool is_error;
  U8_APPEND(p, i, max_length, c, is_error);
  if (is_error) {
    ENEK_THROW(std::logic_error) << "`U8_APPEND' failed.";
  }
  ENEK_ASSERT(old_size < i && i <= s.size());
  s.resize(i);
}

} // namespace Enek::Unicode
