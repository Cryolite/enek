#if !defined(ENEK_FEATURE_TEMPLATE_PARSING_PLACEHOLDER_TYPE_HPP_INCLUDE_GUARD)
#define ENEK_FEATURE_TEMPLATE_PARSING_PLACEHOLDER_TYPE_HPP_INCLUDE_GUARD

#include <iosfwd>


namespace Enek::FeatureTemplate::Parsing{

enum class PlaceholderType
{
  word,
  ngram
}; // enum class PlaceholderType

std::ostream &operator<<(std::ostream &os, PlaceholderType rhs);

} // namespace Enek::FeatureTemplate::Parsing

#endif // !defined(ENEK_FEATURE_TEMPLATE_PARSING_PLACEHOLDER_TYPE_HPP_INCLUDE_GUARD)
