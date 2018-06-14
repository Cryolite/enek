#if !defined(ENEK_UTIL_THROW_HPP_INCLUDE_GUARD)
#define ENEK_UTIL_THROW_HPP_INCLUDE_GUARD

#include <boost/exception/enable_error_info.hpp>
#include <boost/exception/info.hpp>
#include <boost/exception/exception.hpp>
#include <boost/stacktrace/stacktrace.hpp>
#include <boost/current_function.hpp>
#include <sstream>
#include <ostream>
#include <utility>


namespace Enek{

namespace Detail{

struct StackTraceErrorInfoTag;

struct GitCommitHashErrorInfoTag;

} // namespace Detail

typedef boost::error_info<Detail::StackTraceErrorInfoTag,
                          boost::stacktrace::stacktrace> StackTraceErrorInfo;

typedef boost::error_info<Detail::GitCommitHashErrorInfoTag,
                          char const *> GitCommitHashErrorInfo;

namespace Detail{

template<typename Exception>
class ExceptionThrower
{
public:
  ExceptionThrower(char const *function_name,
                   char const *file_name,
                   int line_number,
                   boost::stacktrace::stacktrace &&st,
                   char const *git_commit_hash)
    : oss_(),
      function_name_(function_name),
      file_name_(file_name),
      line_number_(line_number),
      stacktrace_(std::move(st)),
      git_commit_hash_(git_commit_hash)
  {}

  ExceptionThrower(ExceptionThrower const &) = delete;

  ExceptionThrower &operator=(ExceptionThrower const &) = delete;

  template<typename T>
  std::ostream &operator<<(T &&x)
  {
    return oss_ << std::forward<T &&>(x);
  }

  ~ExceptionThrower() noexcept(false)
  {
    throw boost::enable_error_info(Exception(oss_.str()))
      << boost::throw_function(function_name_)
      << boost::throw_file(file_name_)
      << boost::throw_line(line_number_)
      << StackTraceErrorInfo(std::move(stacktrace_))
      << GitCommitHashErrorInfo(git_commit_hash_);
  }

private:
  std::ostringstream oss_;
  char const *function_name_;
  char const *file_name_;
  int line_number_;
  boost::stacktrace::stacktrace stacktrace_;
  char const *git_commit_hash_;
}; // class ExceptionThrower

} // namespace Detail

} // namespace Enek

#define ENEK_THROW(...)                          \
  ::Enek::Detail::ExceptionThrower<__VA_ARGS__>( \
    BOOST_CURRENT_FUNCTION,                      \
    __FILE__,                                    \
    __LINE__,                                    \
    ::boost::stacktrace::stacktrace(),           \
    ENEK_GIT_COMMIT_HASH)

namespace Enek::Detail{

class TerminateHandlerSetter
{
private:
  [[noreturn]] static void terminate_handler_() noexcept;

public:
  TerminateHandlerSetter() noexcept;

  TerminateHandlerSetter(TerminateHandlerSetter const &) = delete;

  TerminateHandlerSetter &operator=(TerminateHandlerSetter const &) = delete;
}; // class TerminateHandlerSetter

// The initialization of this variable triggers the establishment of a function
// provided by this program as a customized handler function for terminating
// exception processing. If the following variable definition (not declaration)
// moves to a .cpp file, the initialization may not execute unless some
// function that compiles in the same translation unit is called.
inline TerminateHandlerSetter terminate_handler_setter;

} // namespace Enek::Detail

#endif // !defined(ENEK_UTIL_THROW_HPP_INCLUDE_GUARD)
