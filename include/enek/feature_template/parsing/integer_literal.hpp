#if !defined(ENEK_FEATURE_TEMPLATE_PARSING_INTEGER_LITERAL_HPP_INCLUDE_GUARD)
#define ENEK_FEATURE_TEMPLATE_PARSING_INTEGER_LITERAL_HPP_INCLUDE_GUARD

#include <enek/feature_template/type.hpp>
#include <boost/range/iterator_range.hpp>
#include <filesystem>
#include <iosfwd>
#include <cstdint>


namespace Enek::FeatureTemplate::Parsing{

template<typename Iterator>
class Grammar;

class IntegerLiteral
{
public:
  using Path = std::filesystem::path;

public:
  IntegerLiteral() noexcept;

  IntegerLiteral(IntegerLiteral const &rhs) noexcept;

  IntegerLiteral &operator=(IntegerLiteral const &) = delete;

  bool isInitialized() const noexcept;

private:
  template<typename Iterator>
  friend class Grammar;

  template<typename Iterator, typename BaseIterator>
  void initialize(
    boost::iterator_range<Iterator> const &parse_range,
    Path const &path,
    boost::iterator_range<BaseIterator> const &text_range,
    std::ostream &os);

public:
  bool succeed() const;

  Enek::FeatureTemplate::Type getType() const;

  std::int_fast64_t getValue() const;

  void dumpXML(std::ostream &os) const;

private:
  bool initialized_;
  bool error_;
  std::int_fast64_t value_;
}; // class IntegerLiteral

} // namespace Enek::FeatureTemplate::Parsing

#endif // !defined(ENEK_FEATURE_TEMPLATE_PARSING_INTEGER_LITERAL_HPP_INCLUDE_GUARD)
