#if !defined(ENEK_FEATURE_TEMPLATE_PARSING_INPUT_TYPE_HPP_INCLUDE_GUARD)
#define ENEK_FEATURE_TEMPLATE_PARSING_INPUT_TYPE_HPP_INCLUDE_GUARD

#include <iosfwd>


namespace Enek::FeatureTemplate::Parsing{

enum class InputType
{
  word,
  ngram
}; // enum class InputType

std::ostream &operator<<(std::ostream &os, InputType rhs);

} // namespace Enek::FeatureTemplate::Parsing

#endif // !defined(ENEK_FEATURE_TEMPLATE_PARSING_INPUT_TYPE_HPP_INCLUDE_GUARD)
