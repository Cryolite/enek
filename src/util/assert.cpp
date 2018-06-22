#include <enek/util/assert.hpp>

#if defined(ENEK_ENABLE_ASSERT)

#include <boost/stacktrace/stacktrace.hpp>
#include <iostream>
#include <utility>
#include <cstdlib>


#if defined(ENEK_ENABLE_COVERAGE)
extern "C" void __gcov_flush();
#endif // defined(ENEK_ENABLE_COVERAGE)

namespace Enek::Detail{

AssertMessenger::AssertMessenger(char const *function_name,
                                 char const *file_name,
                                 int line_number,
                                 char const *expression,
                                 boost::stacktrace::stacktrace &&stacktrace,
                                 char const *git_commit_hash)
  : oss_(),
    function_name_(function_name),
    file_name_(file_name),
    line_number_(line_number),
    expression_(expression),
    stacktrace_(std::move(stacktrace)),
    git_commit_hash_(git_commit_hash)
{}

AssertMessenger &
AssertMessenger::operator<<(std::ostream &(*pf)(std::ostream &))
{
  oss_ << pf;
  return *this;
}

AssertMessenger &AssertMessenger::operator<<(std::ios &(*pf)(std::ios &))
{
  oss_ << pf;
  return *this;
}

AssertMessenger &
AssertMessenger::operator<<(std::ios_base &(*pf)(std::ios_base &))
{
  oss_ << pf;
  return *this;
}

AssertMessenger::operator int() const noexcept
{
  return 0;
}

[[noreturn]] AssertMessenger::~AssertMessenger()
{
  std::cerr << file_name_ << ':' << line_number_ << ": " << function_name_
            << ": " << "Assertion `" << expression_ << "' failed.\n";
  if (!oss_.str().empty()) {
    std::cerr << oss_.str() << '\n';
  }
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

#endif // defined(ENEK_ENABLE_ASSERT)
