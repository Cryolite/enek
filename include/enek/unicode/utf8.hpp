#if !defined(ENEK_UNICODE_UTF8_HPP_INCLUDE_GUARD)
#define ENEK_UNICODE_UTF8_HPP_INCLUDE_GUARD

#include <string>


namespace Enek::Unicode{

void appendCodePoint(std::string &s, char32_t c);

} // namespace Enek::Unicode

#endif // !defined(ENEK_UNICODE_UTF8_HPP_INCLUDE_GUARD)
