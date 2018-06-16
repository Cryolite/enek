#if !defined(ENEK_UNICODE_UTF8_HPP_INCLUDE_GUARD)
#define ENEK_UNICODE_UTF8_HPP_INCLUDE_GUARD

#include <string>
#include <stdexcept>
#include <cstddef>


namespace Enek::Unicode::UTF8{

class InvalidStringError
  : public std::runtime_error
{
public:
  explicit InvalidStringError(std::string const &what_arg)
    : std::runtime_error(what_arg)
  {}

  explicit InvalidStringError(char const *what_arg)
    : std::runtime_error(what_arg)
  {}
}; // class InvalidStringError

void appendCodePoint(std::string &s, char32_t c);

} // namespace Enek::Unicode::UTF8

#endif // !defined(ENEK_UNICODE_UTF8_HPP_INCLUDE_GUARD)
