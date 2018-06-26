#if !defined(ENEK_UTIL_EXCEPTION_GUARD_HPP_INCLUDE_GUARD)
#define ENEK_UTIL_EXCEPTION_GUARD_HPP_INCLUDE_GUARD

#include <enek/util/exception.hpp>
#include <enek/util/assert.hpp>
#include <type_traits>
#include <utility>
#include <cstddef>


namespace Enek::Detail{

template<typename F>
class ExceptionGuard
{
private:
  static_assert(!std::is_reference_v<F>);
  static_assert(!std::is_const_v<F>);
  static_assert(!std::is_volatile_v<F>);

public:
  ExceptionGuard(F &&f) noexcept
    : f_(std::move(f))
    , num_uncaught_exceptions_(Enek::getNumUncaughtExceptions())
  {}

  ExceptionGuard(ExceptionGuard const &) = delete;

  ExceptionGuard &operator=(ExceptionGuard const &) = delete;

  ~ExceptionGuard() noexcept
  {
    std::size_t const n = Enek::getNumUncaughtExceptions();
    if (n != num_uncaught_exceptions_) {
      ENEK_ASSERT(n == num_uncaught_exceptions_ + 1);
      f_();
    }
  }

private:
  F f_;
  std::size_t num_uncaught_exceptions_;
}; // class ExceptionGuard

class ExceptionGuardInitializer
{
public:
  constexpr ExceptionGuardInitializer() = default;

  ExceptionGuardInitializer(ExceptionGuardInitializer const &) = delete;

  ExceptionGuardInitializer &operator=(ExceptionGuardInitializer const &) = delete;

  template<typename F>
  ExceptionGuard<F> operator->*(F &&f) const noexcept
  {
    static_assert(!std::is_reference_v<F>);
    static_assert(!std::is_const_v<F>);
    static_assert(!std::is_volatile_v<F>);
    return { std::move(f) };
  }
}; // class ExceptionGuardInitializer

} // namespace Enek::Detail

#define ENEK_EXCEPTION_GUARD                                                 \
  auto const &enek_exception_guard_ ## __LINE__                              \
    = ::Enek::Detail::ExceptionGuardInitializer() ->* [&]() noexcept -> void \
  /**/

#endif // !defined(ENEK_UTIL_EXCEPTION_GUARD_HPP_INCLUDE_GUARD)
