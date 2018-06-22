#if !defined(ENEK_UTIL_ASSERT_HPP_INCLUDE_GUARD)
#define ENEK_UTIL_ASSERT_HPP_INCLUDE_GUARD

#if defined(ENEK_ENABLE_ASSERT)

#include <boost/stacktrace/stacktrace.hpp>
#include <boost/current_function.hpp>
#include <boost/config.hpp>
#include <sstream>
#include <iosfwd>
#include <utility>


namespace Enek::Detail{

class AssertMessenger
{
public:
  AssertMessenger(char const *function_name,
                  char const *file_name,
                  int line_number,
                  char const *expression,
                  boost::stacktrace::stacktrace &&stacktrace,
                  char const *git_commit_hash);

  AssertMessenger(AssertMessenger const &) = delete;

  AssertMessenger &operator=(AssertMessenger const &) = delete;

  template<typename T>
  AssertMessenger &operator<<(T &&x)
  {
    oss_ << std::forward<T>(x);
    return *this;
  }

  AssertMessenger &operator<<(std::ostream &(*pf)(std::ostream &));

  AssertMessenger &operator<<(std::ios &(*pf)(std::ios &));

  AssertMessenger &operator<<(std::ios_base &(*pf)(std::ios_base &));

  operator int() const noexcept;

  [[noreturn]] ~AssertMessenger();

private:
  std::ostringstream oss_;
  char const *function_name_;
  char const *file_name_;
  int line_number_;
  char const *expression_;
  boost::stacktrace::stacktrace stacktrace_;
  char const *git_commit_hash_;
}; // class AssertMessenger

} // namespace Enek::Detail

#define ENEK_ASSERT(EXPR)                                            \
  BOOST_LIKELY(!!(EXPR)) ? 0 :                                       \
  ::Enek::Detail::AssertMessenger(BOOST_CURRENT_FUNCTION,            \
                                  __FILE__,                          \
                                  __LINE__,                          \
                                  #EXPR,                             \
                                  ::boost::stacktrace::stacktrace(), \
                                  ENEK_GIT_COMMIT_HASH)              \
  /**/

#else // defined(ENEK_ENABLE_ASSERT)

#include <iosfwd>


namespace Enek::Detail{

class DummyAssertMessenger{
public:
  constexpr DummyAssertMessenger() = default;

  DummyAssertMessenger(DummyAssertMessenger const &) = delete;

  DummyAssertMessenger &operator=(DummyAssertMessenger const &) = delete;

  template<typename T>
  DummyAssertMessenger const &operator<<(T &&) const noexcept
  {
    return *this;
  }

  DummyAssertMessenger const &
  operator<<(std::ostream &(*)(std::ostream &)) const noexcept
  {
    return *this;
  }

  DummyAssertMessenger const &
  operator<<(std::ios &(*)(std::ios &)) const noexcept
  {
    return *this;
  }

  DummyAssertMessenger const &
  operator<<(std::ios_base &(*)(std::ios_base &)) const noexcept
  {
    return *this;
  }

  constexpr operator int() const noexcept
  {
    return 0;
  }
}; // class DummyAssertMessenger

} // namespace Enek::Detail

#define ENEK_ASSERT(EXPR) true ? 0 : ::Enek::Detail::DummyAssertMessenger{}

#endif // defined(ENEK_ENABLE_ASSERT)

#endif // !defined(ENEK_UTIL_ASSERT_HPP_INCLUDE_GUARD)
