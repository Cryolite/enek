#if !defined(ENEK_FEATURE_TEMPLATE_PARSING_INTEGER_LITERAL_HPP_INCLUDE_GUARD)
#define ENEK_FEATURE_TEMPLATE_PARSING_INTEGER_LITERAL_HPP_INCLUDE_GUARD

#include <boost/range/iterator_range.hpp>
#include <filesystem>
#include <iosfwd>
#include <cstdint>


namespace Enek::FeatureTemplate::Parsing{

class IntegerLiteral
{
public:
  using Path = std::filesystem::path;

public:
  IntegerLiteral() noexcept;

  IntegerLiteral(IntegerLiteral const &rhs) noexcept;

  bool isInitialized() const noexcept;

  template<typename Iterator, typename BaseIterator>
  void initialize(
    boost::iterator_range<Iterator> const &parse_range,
    Path const &path,
    boost::iterator_range<BaseIterator> const &text_range,
    std::ostream &os);

  bool succeed() const;

  std::int_fast64_t getValue() const;

  void dumpXML(std::ostream &os) const;

private:
  bool initialized_;
  bool error_;
  std::int_fast64_t value_;
}; // class IntegerLiteral

} // namespace Enek::FeatureTemplate::Parsing

#endif // !defined(ENEK_FEATURE_TEMPLATE_PARSING_INTEGER_LITERAL_HPP_INCLUDE_GUARD)
