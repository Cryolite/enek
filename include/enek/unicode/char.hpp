#if !defined(ENEK_UNICODE_CHAR_HPP_INCLUDE_GUARD)
#define ENEK_UNICODE_CHAR_HPP_INCLUDE_GUARD

#include <ostream>


namespace Enek::Unicode{

char32_t getCharMaxValue() noexcept;

namespace GeneralCategory{

bool isOtherSurrogate(char32_t c) noexcept;

} // namespace GeneralCategory

enum struct EastAsianWidth
{
  fullwidth,
  halfwidth,
  wide,
  narrow,
  ambiguous,
  neutral
}; // enum struct

std::ostream &operator<<(std::ostream &os, EastAsianWidth eaw);

EastAsianWidth getEastAsianWidth(char32_t c);

} // namespace Enek::Unicode

#endif // !defined(ENEK_UNICODE_CHAR_HPP_INCLUDE_GUARD)
