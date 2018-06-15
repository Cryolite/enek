#if !defined(ENEK_UNICODE_CHAR_HPP_INCLUDE_GUARD)
#define ENEK_UNICODE_CHAR_HPP_INCLUDE_GUARD

namespace Enek::Unicode{

char32_t getCharMaxValue() noexcept;

namespace GeneralCategory{

bool isOtherSurrogate(char32_t c) noexcept;

} // namespace GeneralCategory

} // namespace Enek::Unicode

#endif // !defined(ENEK_UNICODE_CHAR_HPP_INCLUDE_GUARD)
