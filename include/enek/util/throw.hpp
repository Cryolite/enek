#if !defined(ENEK_UTIL_THROW_HPP_INCLUDE_GUARD)
#define ENEK_UTIL_THROW_HPP_INCLUDE_GUARD

#include <boost/exception/enable_error_info.hpp>
#include <boost/exception/info.hpp>
#include <boost/exception/exception.hpp>
#include <boost/stacktrace/stacktrace.hpp>
#include <boost/current_function.hpp>
#include <sstream>
#include <ostream>
#include <ios>
#include <string>
#include <type_traits>
#include <functional>
#include <tuple>
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

template<typename Exception, typename HasPlaceholder, typename... Args>
class ExceptionThrower;

template<typename Exception, typename... Args>
class ExceptionThrower<Exception, std::false_type, Args...>
{
private:
  static_assert(!std::is_reference_v<Exception>);
  static_assert(!std::is_const_v<Exception>);
  static_assert(!std::is_volatile_v<Exception>);
  static_assert((... && std::is_reference_v<Args>));

private:
  static Exception constructException(Args... args)
    noexcept(noexcept(Exception(std::forward<Args>(args)...)))
  {
    return Exception(std::forward<Args>(args)...);
  }

public:
  ExceptionThrower(char const *function_name,
                   char const *file_name,
                   int line_number,
                   boost::stacktrace::stacktrace &&stacktrace,
                   char const *git_commit_hash,
                   std::tuple<Args...> args) noexcept
    : function_name_(function_name)
    , file_name_(file_name)
    , line_number_(line_number)
    , stacktrace_(std::move(stacktrace))
    , git_commit_hash_(git_commit_hash)
    , args_(std::move(args))
  {}

  ExceptionThrower(ExceptionThrower const &) = delete;

  ExceptionThrower &operator=(ExceptionThrower const &) = delete;

  ~ExceptionThrower() noexcept(false)
  {
    throw boost::enable_error_info(
      std::apply(&ExceptionThrower::constructException, std::move(args_)))
      << boost::throw_function(function_name_)
      << boost::throw_file(file_name_)
      << boost::throw_line(line_number_)
      << StackTraceErrorInfo(std::move(stacktrace_))
      << GitCommitHashErrorInfo(git_commit_hash_);
  }

private:
  char const *function_name_;
  char const *file_name_;
  int line_number_;
  boost::stacktrace::stacktrace stacktrace_;
  char const *git_commit_hash_;
  std::tuple<Args...> args_;
}; // class ExceptionThrower

template<typename Exception, typename... Args>
class ExceptionThrower<Exception, std::true_type, Args...>
{
private:
  static_assert(!std::is_reference_v<Exception>);
  static_assert(!std::is_const_v<Exception>);
  static_assert(!std::is_volatile_v<Exception>);
  static_assert((... && std::is_reference_v<Args>));

private:
  template<typename T>
  using RemoveCVRef = std::remove_cv_t<std::remove_reference_t<T>>;

  template<typename T>
  static constexpr bool is_placeholder_v
    = (std::is_placeholder<RemoveCVRef<T>>::value == 1);

  template<typename T, typename U>
  static std::enable_if_t<!is_placeholder_v<T>, T &&>
  substitutePlaceholder(T &&arg, U &&subst) noexcept
  {
    return arg;
  }

  template<typename T, typename U>
  static std::enable_if_t<is_placeholder_v<T>, U &&>
  substitutePlaceholder(T &&arg, U &&substitute) noexcept
  {
    return substitute;
  }

  static Exception constructException(std::string const &what, Args... args)
    noexcept(noexcept(Exception(substitutePlaceholder(std::forward<Args>(args), what)...)))
  {
    return Exception(substitutePlaceholder(std::forward<Args>(args), what)...);
  }

public:
  ExceptionThrower(char const *function_name,
                   char const *file_name,
                   int line_number,
                   boost::stacktrace::stacktrace &&stacktrace,
                   char const *git_commit_hash,
                   std::tuple<Args...> args) noexcept
    : function_name_(function_name)
    , file_name_(file_name)
    , line_number_(line_number)
    , stacktrace_(std::move(stacktrace))
    , git_commit_hash_(git_commit_hash)
    , args_(std::move(args))
    , oss_()
  {}

  ExceptionThrower(ExceptionThrower const &) = delete;

  ExceptionThrower &operator=(ExceptionThrower const &) = delete;

  template<typename T>
  ExceptionThrower &operator<<(T &&value)
  {
    oss_ << std::forward<T>(value);
    return *this;
  }

  ExceptionThrower &operator<<(std::ostream &(*pf)(std::ostream &))
  {
    oss_ << pf;
    return *this;
  }

  ExceptionThrower &operator<<(std::ios &(*pf)(std::ios &))
  {
    oss_ << pf;
    return *this;
  }

  ExceptionThrower &operator<<(std::ios_base &(*pf)(std::ios_base &))
  {
    oss_ << pf;
    return *this;
  }

  ~ExceptionThrower() noexcept(false)
  {
    throw boost::enable_error_info(
      std::apply(
        &ExceptionThrower::constructException,
        std::tuple_cat(std::forward_as_tuple(oss_.str()), std::move(args_))))
      << boost::throw_function(function_name_)
      << boost::throw_file(file_name_)
      << boost::throw_line(line_number_)
      << StackTraceErrorInfo(std::move(stacktrace_))
      << GitCommitHashErrorInfo(git_commit_hash_);
  }

private:
  char const *function_name_;
  char const *file_name_;
  int line_number_;
  boost::stacktrace::stacktrace stacktrace_;
  char const *git_commit_hash_;
  std::tuple<Args...> args_;
  std::ostringstream oss_;
}; // class ExceptionThrower

class ExceptionInfoHolder
{
public:
  ExceptionInfoHolder(char const *function_name,
                      char const *file_name,
                      int line_number,
                      boost::stacktrace::stacktrace &&stacktrace,
                      char const *git_commit_hash) noexcept
    : function_name_(function_name)
    , file_name_(file_name)
    , line_number_(line_number)
    , stacktrace_(std::move(stacktrace))
    , git_commit_hash_(git_commit_hash)
  {}

  ExceptionInfoHolder(ExceptionInfoHolder const &) = delete;

  ExceptionInfoHolder &operator=(ExceptionInfoHolder const &) = delete;

  template<typename T>
  using RemoveCVRef = std::remove_cv_t<std::remove_reference_t<T>>;

  template<typename... Args>
  using HasPlaceholder
    = std::bool_constant<
      (... || (std::is_placeholder<RemoveCVRef<Args>>::value == 1))>;

  template<typename Exception, typename... Args>
  using Thrower = ExceptionThrower<Exception, HasPlaceholder<Args...>, Args &&...>;

  template<typename Exception, typename... Args>
  Thrower<Exception, Args...> setException(Args &&... args) noexcept
  {
    static_assert(!std::is_reference_v<Exception>);
    static_assert(!std::is_const_v<Exception>);
    static_assert(!std::is_volatile_v<Exception>);
    static_assert(
      (... && (std::is_placeholder<RemoveCVRef<Args>>::value <= 1)));
    return Thrower<Exception, Args...>(
      function_name_,
      file_name_,
      line_number_,
      std::move(stacktrace_),
      git_commit_hash_,
      std::forward_as_tuple(std::forward<Args>(args)...));
  }

private:
  char const *function_name_;
  char const *file_name_;
  int line_number_;
  boost::stacktrace::stacktrace stacktrace_;
  char const *git_commit_hash_;
}; // class ExceptionInfoHolder

} // namespace Detail

} // namespace Enek

#define ENEK_THROW                              \
  ::Enek::Detail::ExceptionInfoHolder(          \
    BOOST_CURRENT_FUNCTION,                     \
    __FILE__,                                   \
    __LINE__,                                   \
    ::boost::stacktrace::stacktrace(),          \
    ENEK_GIT_COMMIT_HASH).template setException

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
