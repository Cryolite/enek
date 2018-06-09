#if !defined(ENEK_FEATURE_TEMPLATE_PARSING_BOOLEAN_LITERAL_HPP_INCLUDE_GUARD)
#define ENEK_FEATURE_TEMPLATE_PARSING_BOOLEAN_LITERAL_HPP_INCLUDE_GUARD

#include <ostream>

namespace Enek::FeatureTemplate::Parsing{

class BooleanLiteral
{
public:
  BooleanLiteral() noexcept;

  BooleanLiteral(BooleanLiteral const &) noexcept;

  bool isInitialized() const noexcept;

  void initialize(bool value) noexcept;

  bool succeed() const noexcept;

  void swap(BooleanLiteral &) noexcept;

  friend void swap(BooleanLiteral &, BooleanLiteral &) noexcept;

  BooleanLiteral &operator=(BooleanLiteral const &) noexcept;

  bool getValue() const noexcept;

  void dumpXML(std::ostream &) const;

private:
  bool initialized_;
  bool value_;
}; // class BooleanLiteral

} // namespace Enek::FeatureTemplate::Parsing

#endif // !defined(ENEK_FEATURE_TEMPLATE_PARSING_BOOLEAN_LITERAL_HPP_INCLUDE_GUARD)
