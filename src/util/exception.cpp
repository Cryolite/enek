#include <enek/util/exception.hpp>
#include <cstddef>
#include <cxxabi.h>


namespace __cxxabiv1{

struct __cxa_eh_globals{
  struct __cxa_exception *caughtExceptions;
  unsigned int            uncaughtExceptions;
};

} // namespace __cxxabiv1

namespace Enek{

std::size_t getNumUncaughtExceptions() noexcept
{
  __cxxabiv1::__cxa_eh_globals *globals = __cxxabiv1::__cxa_get_globals_fast();
  if (globals == nullptr) {
    return 0;
  }
  return globals->uncaughtExceptions;
}

} // namespace Enek
