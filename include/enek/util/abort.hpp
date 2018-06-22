#if !defined(ENEK_UTIL_ABORT_HPP_INCLUDE_GUARD)
#define ENEK_UTIL_ABORT_HPP_INCLUDE_GUARD

#include <boost/stacktrace/stacktrace.hpp>
#include <boost/current_function.hpp>
#include <sstream>
#include <iosfwd>
#include <utility>


namespace Enek::Detail{

class AbortMessenger
{
public:
  AbortMessenger(char const *function_name,
                 char const *file_name,
                 int line_number,
                 boost::stacktrace::stacktrace &&stacktrace,
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

  [[noreturn]] ~AbortMessenger();

private:
  std::ostringstream oss_;
  char const *function_name_;
  char const *file_name_;
  int line_number_;
  boost::stacktrace::stacktrace stacktrace_;
  char const *git_commit_hash_;
}; // class AbortMessenge

} // namespace Enek::Detail

#define ENEK_ABORT                                                   \
  ::Enek::Detail::AbortMessenger(BOOST_CURRENT_FUNCTION,             \
                                 __FILE__,                           \
                                 __LINE__,                           \
                                 ::boost::stacktrace::stacktrace(),  \
                                 ENEK_GIT_COMMIT_HASH)               \
  /**/

#endif // !defined(ENEK_UTIL_ABORT_HPP_INCLUDE_GUARD)
