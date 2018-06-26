#if !defined(ENEK_UTIL_STRING_KEY_HPP_INCLUDE_GUARD)
#define ENEK_UTIL_STRING_KEY_HPP_INCLUDE_GUARD

#include <string_view>
#include <string>
#include <functional>
#include <variant>
#include <cstddef>


namespace Enek{

class StringKey
{
public:
  StringKey() noexcept;

  explicit StringKey(std::string const &string);

  explicit StringKey(std::string &&string) noexcept;

  explicit StringKey(std::string_view const &string_view) noexcept;

  StringKey(StringKey const &rhs);

  StringKey(StringKey &&rhs) noexcept;

  void swap(StringKey &rhs) noexcept;

  StringKey &operator=(StringKey const &rhs);

  StringKey &operator=(StringKey &&rhs) noexcept;

  std::string moveString() noexcept;

  std::string_view getStringView() const noexcept;

  bool operator==(StringKey const &rhs) const noexcept;

  bool operator!=(StringKey const &rhs) const noexcept;

  bool operator<(StringKey const &rhs) const noexcept;

  bool operator>(StringKey const &rhs) const noexcept;

  bool operator<=(StringKey const &rhs) const noexcept;

  bool operator>=(StringKey const &rhs) const noexcept;

  friend class std::hash<StringKey>;

private:
  using Impl_ = std::variant<std::string, std::string_view>;
  Impl_ impl_;
}; // class StringKey

void swap(StringKey &lhs, StringKey &rhs) noexcept;

} // namespace Enek

namespace std{

template<>
struct hash<Enek::StringKey>
{
  std::size_t operator()(Enek::StringKey const &key) const noexcept;
}; // struct hash<Enek::StringKey>

} // namespace std

#endif // !defined(ENEK_UTIL_STRING_KEY_HPP_INCLUDE_GUARD)
