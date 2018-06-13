#if !defined(ENEK_UTIL_ASSERT_HPP_INCLUDE_GUARD)
#define ENEK_UTIL_ASSERT_HPP_INCLUDE_GUARD

#if defined(ENEK_ENABLE_ASSERT)

#include <boost/stacktrace/stacktrace.hpp>
#include <boost/current_function.hpp>
#include <boost/config.hpp>
#include <sstream>
#include <ostream>
#include <ios>
#include <utility>


namespace Enek::Detail{

class AbortMessenger
{
public:
  AbortMessenger(char const *function_name,
                 char const *file_name,
                 int line_number,
                 char const *expression,
                 boost::stacktrace::stacktrace &&st,
                 char const *git_commit_hash);

  AbortMessenger(AbortMessenger const &) = delete;

  AbortMessenger &operator=(AbortMessenger const &) = delete;

  template<typename T>
  AbortMessenger &operator<<(T &&x)
  {
    oss_ << std::forward<T>(x);
    return *this;
  }

  AbortMessenger &operator<<(std::ostream &(*pf)(std::ostream &));

  AbortMessenger &operator<<(std::ios &(*pf)(std::ios &));

  AbortMessenger &operator<<(std::ios_base &(*pf)(std::ios_base &));

  operator int() const noexcept;

  ~AbortMessenger();

private:
  std::ostringstream oss_;
  char const *function_name_;
  char const *file_name_;
  int line_number_;
  char const *expression_;
  boost::stacktrace::stacktrace stacktrace_;
  char const *git_commit_hash_;
}; // class AbortMessenge

} // namespace Enek::Detail

#define ENEK_ASSERT(EXPR)                                            \
  BOOST_LIKELY(!!(EXPR)) ? 0 :                                       \
   ::Enek::Detail::AbortMessenger(BOOST_CURRENT_FUNCTION,            \
                                  __FILE__,                          \
                                  __LINE__,                          \
                                  #EXPR,                             \
                                  ::boost::stacktrace::stacktrace(), \
                                  ENEK_GIT_COMMIT_HASH)

#else // defined(ENEK_ENABLE_ASSERT)

#include <ostream>
#include <ios>


namespace Enek::Detail{

class DummyAbortMessenger{
public:
  constexpr DummyAbortMessenger() = default;

  DummyAbortMessenger(DummyAbortMessenger const &) = delete;

  DummyAbortMessenger &operator=(DummyAbortMessenger const &) = delete;

  template<typename T>
  DummyAbortMessenger const &operator<<(T &&) const noexcept
  {
    return *this;
  }

  DummyAbortMessenger const &
  operator<<(std::ostream &(*)(std::ostream &)) const noexcept
  {
    return *this;
  }

  DummyAbortMessenger const &
  operator<<(std::ios &(*)(std::ios &)) const noexcept
  {
    return *this;
  }

  DummyAbortMessenger const &
  operator<<(std::ios_base &(*)(std::ios_base &)) const noexcept
  {
    return *this;
  }

  constexpr operator int() const noexcept
  {
    return 0;
  }
}; // class DummyAbortMessenger

} // namespace Enek::Detail

# define ENEK_ASSERT(EXPR) true ? 0 : ::Enek::Detail::DummyAbortMessenger{}

#endif // defined(ENEK_ENABLE_ASSERT)

#endif // !defined(ENEK_UTIL_ASSERT_HPP_INCLUDE_GUARD)
