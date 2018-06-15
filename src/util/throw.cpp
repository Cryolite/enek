#include <enek/util/throw.hpp>
#include <enek/util/type_name.hpp>
#include <boost/exception/get_error_info.hpp>
#include <boost/exception/exception.hpp>
#include <boost/stacktrace/stacktrace.hpp>
#include <iostream>
#include <ostream>
#include <exception>
#include <cstdlib>


#if defined(ENEK_ENABLE_COVERAGE)
extern "C" void __gcov_flush();
#endif // defined(ENEK_ENABLE_COVERAGE)

namespace Enek::Detail{

namespace{

void printCommonErrorMessage()
{
  if (ENEK_GIT_COMMIT_HASH[0] != '\0') {
    std::cerr << "Git commit hash: " << ENEK_GIT_COMMIT_HASH << '\n';
  }
  boost::stacktrace::stacktrace stacktrace;
  if (!stacktrace.empty()) {
    std::cerr << "Backtrace:\n" << stacktrace;
  }
}

} // namespace *unnamed*

[[noreturn]] void TerminateHandlerSetter::terminate_handler_() noexcept
{
  std::exception_ptr const p = std::current_exception();
  if (p != nullptr) {
    try {
      std::rethrow_exception(p);
    }
    catch (boost::exception const &e) {
      std::cerr << "`std::terminate' is called after throwing an instance of `"
                << Enek::getTypeName(e) << "'.\n";
      if (char const * const * const p
            = boost::get_error_info<boost::throw_file>(e)) {
        std::cerr << *p << ':';
      }
      if (int const *p = boost::get_error_info<boost::throw_line>(e)) {
        std::cerr << *p << ": ";
      }
      if (char const * const * const p
            = boost::get_error_info<boost::throw_function>(e)) {
        std::cerr << *p << ": ";
      }
      if (std::exception const *p = dynamic_cast<std::exception const *>(&e)) {
        std::cerr << p->what();
      }
      std::cerr << '\n';
      if (char const * const * const p
            = boost::get_error_info<Enek::GitCommitHashErrorInfo>(e)) {
        std::cerr << "Git commit hash: " << *p << '\n';
      }
      if (boost::stacktrace::stacktrace const * const p
            = boost::get_error_info<Enek::StackTraceErrorInfo>(e)) {
        if (p->size() != 0) {
          std::cerr << "Backtrace:\n" << *p;
        }
      }
    }
    catch (std::exception const &e) {
      std::cerr << "`std::terminate' is called after throwing an instance of `"
                << Enek::getTypeName(e) << "'.\n" << e.what() << '\n';
      printCommonErrorMessage();
    }
    catch (...) {
      std::cerr << "`std::terminate' is called after throwing an instance of an unknown type.\n";
      printCommonErrorMessage();
    }
  }
  else {
    std::cerr << "`std::terminate' is called without throwing any exception.\n";
    printCommonErrorMessage();
  }
  std::cerr << std::flush;
#if defined(ENEK_ENABLE_COVERAGE)
  __gcov_flush(); std::abort();
#else // defined(ENEK_ENABLE_COVERAGE)
  std::abort();
#endif // defined(ENEK_ENABLE_COVERAGE)
}

TerminateHandlerSetter::TerminateHandlerSetter() noexcept
{
  std::set_terminate(&terminate_handler_);
}

} // namespace Enek::Detail
