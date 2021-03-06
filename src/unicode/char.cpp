#include <enek/unicode/char.hpp>
#include <enek/util/throw.hpp>
#include <enek/util/assert.hpp>
#include <enek/util/type_name.hpp>
#include <unicode/uchar.h>
#include <unicode/umachine.h>
#include <ostream>
#include <functional>
#include <stdexcept>
#include <cstdint>


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

std::ostream &operator<<(std::ostream &os, EastAsianWidth eaw)
{
  using std::placeholders::_1;
  switch (eaw) {
  case EastAsianWidth::fullwidth:
    os << "East Asian Fullwidth";
    break;
  case EastAsianWidth::halfwidth:
    os << "East Asian Halfwidth";
    break;
  case EastAsianWidth::wide:
    os << "East Asian Wide";
    break;
  case EastAsianWidth::narrow:
    os << "East Asian Narrow";
    break;
  case EastAsianWidth::ambiguous:
    os << "East Asian Ambiguous";
    break;
  case EastAsianWidth::neutral:
    os << "East Asian Neutral";
    break;
    // LCOV_EXCL_START
  default:
    ENEK_THROW<std::logic_error>(_1)
      << "An unknown value `" << static_cast<int>(eaw) << "' of type `"
      << Enek::getTypeName(eaw) << "'.";
    break;
    // LCOV_EXCL_STOP
  }
  return os;
}

EastAsianWidth getEastAsianWidth(char32_t c)
{
  using std::placeholders::_1;
  EastAsianWidth result;
  {
    std::int32_t const v = u_getIntPropertyValue(
      static_cast<UChar32>(c), UCHAR_EAST_ASIAN_WIDTH);
    switch (static_cast<UEastAsianWidth>(v)) {
    case UEastAsianWidth::U_EA_NEUTRAL:
      result = EastAsianWidth::neutral;
      break;
    case UEastAsianWidth::U_EA_AMBIGUOUS:
      result = EastAsianWidth::ambiguous;
      break;
    case UEastAsianWidth::U_EA_HALFWIDTH:
      result = EastAsianWidth::halfwidth;
      break;
    case UEastAsianWidth::U_EA_FULLWIDTH:
      result = EastAsianWidth::fullwidth;
      break;
    case UEastAsianWidth::U_EA_NARROW:
      result = EastAsianWidth::narrow;
      break;
    case UEastAsianWidth::U_EA_WIDE:
      result = EastAsianWidth::wide;
      break;
      // LCOV_EXCL_START
    default:
      ENEK_THROW<std::logic_error>(_1)
        << "`u_getIntPropertyValue(c, UCHAR_EAST_ASIAN_WIDTH)' returns an "
           "unknown value `" << v << "'.";
      break;
      // LCOV_EXCL_STOP
    }
  }
  return result;
}

} // namespace Enek::Unicode
