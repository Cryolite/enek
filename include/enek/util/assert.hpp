#if !defined(ENEK_UTIL_ASSERT_HPP_INCLUDE_GUARD)
#define ENEK_UTIL_ASSERT_HPP_INCLUDE_GUARD

#if defined(ENEK_ENABLE_ASSERT)

#include <boost/stacktrace/stacktrace.hpp>
#include <boost/current_function.hpp>
#include <boost/config.hpp>
#include <sstream>
#include <ostream>
#include <utility>

namespace Enek::Detail{

class AbortMessanger
{
public:
  AbortMessanger(char const *function_name,
                 char const *file_name,
                 int line_number,
                 char const *expression,
                 boost::stacktrace::stacktrace &&st,
                 char const *git_commit_hash);

  AbortMessanger(AbortMessanger const &) = delete;

  AbortMessanger &operator=(AbortMessanger const &) = delete;

  template<typename T>
  std::ostream &operator<<(T &&x)
  {
    return oss_ << std::forward<T>(x);
  }

  ~AbortMessanger();

private:
  std::ostringstream oss_;
  char const *function_name_;
  char const *file_name_;
  int line_number_;
  char const *expression_;
  boost::stacktrace::stacktrace stacktrace_;
  char const *git_commit_hash_;
}; // class AbortMessange

} // namespace Enek::Detail

#define ENEK_ASSERT(EXPR)                                                    \
  (BOOST_LIKELY(!!(EXPR)) ? ((void)0) :                                      \
   ((void)(::Enek::Detail::AbortMessanger(BOOST_CURRENT_FUNCTION,            \
                                          __FILE__,                          \
                                          __LINE__,                          \
                                          #EXPR,                             \
                                          ::boost::stacktrace::stacktrace(), \
                                          ENEK_GIT_COMMIT_HASH))))

#define ENEK_ASSERT_MSG(EXPR, ...)                                             \
  (BOOST_LIKELY(!!(EXPR)) ? ((void)0) :                                        \
   ((void)(::Enek::Detail::AbortMessanger(BOOST_CURRENT_FUNCTION,              \
                                          __FILE__,                            \
                                          __LINE__,                            \
                                          #EXPR,                               \
                                          ::boost::stacktrace::stacktrace(),   \
                                          ENEK_GIT_COMMIT_HASH) __VA_ARGS__)))

#else // defined(ENEK_ENABLE_ASSERT)

namespace Enek::Detail{

class DummyAbortMessanger{
public:
  constexpr DummyAbortMessanger() = default;

  DummyAbortMessanger(DummyAbortMessanger const &) = delete;

  DummyAbortMessanger &operator=(DummyAbortMessanger const &) = delete;

  template<typename T>
  constexpr DummyAbortMessanger &operator<<(T &&) const noexcept
  {
    return *this;
  }
}; // class DummyAbortMessanger

} // namespace Enek::Detail

# define ENEK_ASSERT(EXPR) ((void)0)

# define ENEK_ASSERT_MSG(EXPR, ...)                                \
  ((true) ? ((void)0) : ((void)DummyAbortMessanger{} __VA_ARGS__))

#endif // defined(ENEK_ENABLE_ASSERT)

#endif // !defined(ENEK_UTIL_ASSERT_HPP_INCLUDE_GUARD)
