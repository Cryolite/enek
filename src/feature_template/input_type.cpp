#include <enek/feature_template/input_type.hpp>
#include <enek/util/throw.hpp>
#include <ostream>
#include <functional>
#include <stdexcept>


namespace Enek::FeatureTemplate{

std::ostream &operator<<(std::ostream &os, InputType rhs)
{
  using std::placeholders::_1;
  switch (rhs) {
  case InputType::word:
    os << "word";
    break;
  case InputType::ngram:
    os << "ngram";
    break;
  default:
    ENEK_THROW<std::logic_error>(_1)
      <<"An unknown value `" << static_cast<int>(rhs)
      << "' of type `Enek::FeatureTemplate::InputType'.";
    break;
  }
  return os;
}

} // namespace Enek::FeatureTemplate
