#if !defined(ENEK_FEATURE_TEMPLATE_PARSING_BOOLEAN_LITERAL_HPP_INCLUDE_GUARD)
#define ENEK_FEATURE_TEMPLATE_PARSING_BOOLEAN_LITERAL_HPP_INCLUDE_GUARD

#include <enek/feature_template/type.hpp>
#include <iosfwd>


namespace Enek::FeatureTemplate::Parsing{

template<typename Iterator>
class Grammar;

class BooleanLiteral
{
public:
  BooleanLiteral() noexcept;

  BooleanLiteral(BooleanLiteral const &) noexcept;

  BooleanLiteral &operator=(BooleanLiteral const &) = delete;

  bool isInitialized() const noexcept;

private:
  template<typename Iterator>
  friend class Grammar;

  void initialize(bool value);

public:
  bool succeed() const;

  Enek::FeatureTemplate::Type getType() const;

  bool getValue() const;

  void dumpXML(std::ostream &) const;

private:
  bool initialized_;
  bool value_;
}; // class BooleanLiteral

} // namespace Enek::FeatureTemplate::Parsing

#endif // !defined(ENEK_FEATURE_TEMPLATE_PARSING_BOOLEAN_LITERAL_HPP_INCLUDE_GUARD)
