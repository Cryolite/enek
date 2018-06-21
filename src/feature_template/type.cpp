#include <enek/feature_template/type.hpp>
#include <enek/util/throw.hpp>
#include <enek/util/type_name.hpp>
#include <ostream>
#include <functional>
#include <stdexcept>


namespace Enek::FeatureTemplate{

std::ostream &operator<<(std::ostream &os, Type rhs)
{
  using std::placeholders::_1;
  switch (rhs) {
  case Type::unknown:
    os << "unknown";
    break;
  case Type::integer:
    os << "integer";
    break;
  case Type::floating:
    os << "floating";
    break;
  case Type::boolean:
    os << "boolean";
    break;
  case Type::string:
    os << "string";
    break;
    // LCOV_EXCL_START
  default:
    ENEK_THROW<std::logic_error>(_1)
      << "An unknown value `" << static_cast<int>(rhs) << "' of type `"
      << Enek::getTypeName(rhs) << "'.";
    break;
    // LCOV_EXCL_STOP
  }
  return os;
}

} // namespace Enek::FeatureTemplate
