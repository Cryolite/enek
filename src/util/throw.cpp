#include <enek/util/throw.hpp>
#include <enek/util/type_name.hpp>
#include <boost/exception/get_error_info.hpp>
#include <boost/exception/exception.hpp>
#include <boost/stacktrace/stacktrace.hpp>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <exception>
#include <typeinfo>
#include <cstdlib>

namespace Enek{ namespace{

class TerminateHandlerSetter
{
private:
  [[noreturn]] static void terminate_handler_() noexcept
  {
    std::exception_ptr const p = std::current_exception();
    if (!p) {
      std::abort();
    }
    try {
      std::rethrow_exception(p);
    }
    catch (boost::exception const &e) {
      std::cerr << "Terminate called after throwing an instance of '"
                << Enek::getTypeName(e) << "'\n";
      if (char const * const *p
            = boost::get_error_info<boost::throw_file>(e)) {
        std::cerr << *p << ':';
      }
      if (int const *p = boost::get_error_info<boost::throw_line>(e)) {
        std::cerr << *p << ':';
      }
      if (char const * const *p
            = boost::get_error_info<boost::throw_function>(e)) {
        std::cerr << *p << ": ";
      }
      if (std::exception const *p = dynamic_cast<std::exception const *>(&e)) {
        std::cerr << p->what();
      }
      std::cerr << '\n';
      if (boost::stacktrace::stacktrace const *p
            = boost::get_error_info<Enek::StackTraceErrorInfo>(e)) {
        if (p->size() != 0) {
          std::cerr << "Backtrace:\n";
          std::cerr << *p << '\n';
        }
      }
    }
    std::abort();
  }

public:
  TerminateHandlerSetter() noexcept
  {
    std::set_terminate(&terminate_handler_);
  }
}; // class TerminateHandlerSetter

TerminateHandlerSetter terminate_handler_setter;

}} // namespace Enek::*unnamed*
