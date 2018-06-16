#if !defined(ENEK_UNICODE_ICU_ERROR_HPP_INCLUDE_GUARD)
#define ENEK_UNICODE_ICU_ERROR_HPP_INCLUDE_GUARD

#include <string>
#include <system_error>


namespace Enek::Unicode{

class ICUErrorCategory
  : public std::error_category
{
public:
  ICUErrorCategory() noexcept;

  ICUErrorCategory(ICUErrorCategory const &) = delete;

  ICUErrorCategory &operator=(ICUErrorCategory const &) = delete;

  char const *name() const noexcept override;

  std::string message(int ev) const override;
}; // class ICUErrorCategory

std::error_category &getICUErrorCategory();

} // namespace Enek::Unicode

#endif // !defined(ENEK_UNICODE_ICU_ERROR_HPP_INCLUDE_GUARD)
