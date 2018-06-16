#include <enek/unicode/icu_error.hpp>
#include <unicode/utypes.h>
#include <sstream>
#include <string>
#include <system_error>


namespace Enek::Unicode{

ICUErrorCategory::ICUErrorCategory() noexcept = default;

char const *ICUErrorCategory::name() const noexcept
{
  return "icu";
}

std::string ICUErrorCategory::message(int ev) const
{
  std::ostringstream oss;
  oss << "ICU: " << u_errorName(static_cast<UErrorCode>(ev))
      << " (UErrorCode = " << ev << ')';
  return oss.str();
}

std::error_category &getICUErrorCategory()
{
  static ICUErrorCategory icu_error_category;
  return icu_error_category;
}

} // namespace Enek::Unicode
