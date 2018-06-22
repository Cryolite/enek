#include <enek/util/abort.hpp>
#include <boost/stacktrace/stacktrace.hpp>
#include <iostream>
#include <utility>
#include <cstdlib>


#if defined(ENEK_ENABLE_COVERAGE)
extern "C" void __gcov_flush();
#endif // defined(ENEK_ENABLE_COVERAGE)

namespace Enek::Detail{

AbortMessenger::AbortMessenger(char const *function_name,
                               char const *file_name,
                               int line_number,
                               boost::stacktrace::stacktrace &&stacktrace,
                               char const *git_commit_hash)
  : oss_(),
    function_name_(function_name),
    file_name_(file_name),
    line_number_(line_number),
    stacktrace_(std::move(stacktrace)),
    git_commit_hash_(git_commit_hash)
{}

AbortMessenger &AbortMessenger::operator<<(std::ostream &(*pf)(std::ostream &))
{
  oss_ << pf;
  return *this;
}

AbortMessenger &AbortMessenger::operator<<(std::ios &(*pf)(std::ios &))
{
  oss_ << pf;
  return *this;
}

AbortMessenger &
AbortMessenger::operator<<(std::ios_base &(*pf)(std::ios_base &))
{
  oss_ << pf;
  return *this;
}

[[noreturn]] AbortMessenger::~AbortMessenger()
{
  std::cerr << file_name_ << ':' << line_number_ << ": " << function_name_
            << ": " << oss_.str() << '\n';
  if (*git_commit_hash_ != 0) {
    std::cerr << "Git commit hash: " << git_commit_hash_ << '\n';
  }
  if (stacktrace_) {
    std::cerr << "Backtrace:\n" << stacktrace_;
  }
  std::cerr << std::flush;
#if defined(ENEK_ENABLE_COVERAGE)
  __gcov_flush(); std::abort();
#else // defined(ENEK_ENABLE_COVERAGE)
  std::abort();
#endif // defined(ENEK_ENABLE_COVERAGE)
}

} // namespace Enek::Detail
