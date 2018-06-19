#if !defined(ENEK_FEATURE_TEMPLATE_TYPE_HPP_INCLUDE_GUARD)
#define ENEK_FEATURE_TEMPLATE_TYPE_HPP_INCLUDE_GUARD

#include <iosfwd>


namespace Enek::FeatureTemplate{

enum class Type{
  unknown,
  integer,
  floating,
  boolean,
  string
}; // enum class Type

std::ostream &operator<<(std::ostream &os, Type rhs);

} // namespace Enek::FeatureTemplate

#endif // !defined(ENEK_FEATURE_TEMPLATE_TYPE_HPP_INCLUDE_GUARD)
