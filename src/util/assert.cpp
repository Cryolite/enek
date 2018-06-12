#include <enek/util/assert.hpp>

#if defined(ENEK_ENABLE_ASSERT)

#include <boost/stacktrace/stacktrace.hpp>
#include <iostream>
#include <utility>
#include <cstdlib>

namespace Enek::Detail{

AbortMessenger::AbortMessenger(char const *function_name,
                               char const *file_name,
                               int line_number,
                               char const *expression,
                               boost::stacktrace::stacktrace &&st,
                               char const *git_commit_hash)
  : oss_(),
    function_name_(function_name),
    file_name_(file_name),
    line_number_(line_number),
    expression_(expression),
    stacktrace_(std::move(st)),
    git_commit_hash_(git_commit_hash)
{}

AbortMessenger::~AbortMessenger()
{
  std::cerr << file_name_ << ':' << line_number_ << ": " << function_name_
            << ": " << "Assertion `" << expression_ << "' failed.\n";
  if (!oss_.str().empty()) {
    std::cerr << ' ' << oss_.str() << '\n';
  }
  if (*git_commit_hash_ != 0) {
    std::cerr << "Git commit hash: " << git_commit_hash_ << '\n';
  }
  if (stacktrace_) {
    std::cerr << "Backtrace:\n" << stacktrace_ << '\n';
  }
  std::cerr << std::flush;
  std::abort();
}

} // namespace Enek::Detail

#endif // defined(ENEK_ENABLE_ASSERT)
