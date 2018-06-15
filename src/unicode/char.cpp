#include <enek/unicode/char.hpp>

#include <enek/util/assert.hpp>
#include <unicode/uchar.h>


namespace Enek::Unicode{

char32_t getCharMaxValue() noexcept
{
  return UCHAR_MAX_VALUE;
}

namespace GeneralCategory{

bool isOtherSurrogate(char32_t c) noexcept
{
  ENEK_ASSERT(c <= getCharMaxValue());
  return u_charType(c) == UCharCategory::U_SURROGATE;
}

} // namespace GeneralCategory

} // namespace Enek::Unicode
