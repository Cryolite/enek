#if !defined(ENEK_FEATURE_TEMPLATE_PARSING_FLOATING_LITERAL_HPP_INCLUDE_GUARD)
#define ENEK_FEATURE_TEMPLATE_PARSING_FLOATING_LITERAL_HPP_INCLUDE_GUARD

#include <enek/feature_template/type.hpp>
#include <enek/util/throw.hpp>
#include <enek/util/assert.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/end.hpp>
#include <boost/range/begin.hpp>
#include <sstream>
#include <ostream>
#include <string>
#include <stdexcept>


namespace Enek::FeatureTemplate::Parsing{

class FloatingLiteral
{
public:
  FloatingLiteral() noexcept;

  FloatingLiteral(FloatingLiteral const &rhs) noexcept;

  FloatingLiteral &operator=(FloatingLiteral const &rhs) = delete;

  bool isInitialized() const noexcept;

  template<typename Iterator>
  void initialize(boost::iterator_range<Iterator> const &parse_range)
  {
    if (this->isInitialized()) {
      ENEK_THROW<std::invalid_argument>(
        "Try to initialize an already initialized object.");
    }
    std::string buf(boost::begin(parse_range), boost::end(parse_range));
    std::istringstream iss(buf);
    iss >> value_;
    initialized_ = true;
  }

  bool succeed() const;

  Enek::FeatureTemplate::Type getType() const;

  double getValue() const;

  void dumpXML(std::ostream &os) const;

private:
  bool initialized_;
  double value_;
}; // class FloatingLiteral

} // namespace Enek::FeatureTemplate::Parsing

#endif // !defined(ENEK_FEATURE_TEMPLATE_PARSING_FLOATING_LITERAL_HPP_INCLUDE_GUARD)
