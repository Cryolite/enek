#include <enek/feature_template/parsing/input_type.hpp>
#include <enek/util/throw.hpp>
#include <enek/util/type_name.hpp>
#include <ostream>
#include <functional>
#include <stdexcept>


namespace Enek::FeatureTemplate::Parsing{

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
    // LCOV_EXCL_START
  default:
    ENEK_THROW<std::logic_error>(_1)
      <<"An unknown value `" << static_cast<int>(rhs) << "' of type `"
      << Enek::getTypeName(rhs) << "'.";
    break;
    // LCOV_EXCL_STOP
  }
  return os;
}

} // namespace Enek::FeatureTemplate::Parsing
