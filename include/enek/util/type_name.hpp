#if !defined(ENEK_UTIL_TYPE_NAME_HPP_INCLUDE_GUARD)
#define ENEK_UTIL_TYPE_NAME_HPP_INCLUDE_GUARD

#include <string>
#include <typeinfo>

namespace Enek{

std::string getTypeName(std::type_info const &ti);

template<typename T>
std::string getTypeName(T const &x)
{
  return Enek::getTypeName(typeid(x));
}

} // namespace Enek

#endif // !defined(ENEK_UTIL_TYPE_NAME_HPP_INCLUDE_GUARD)
