#if !defined(ENEK_FEATURE_TEMPLATE_PARSING_STRING_LITERAL_HPP_INCLUDE_GUARD)
#define ENEK_FEATURE_TEMPLATE_PARSING_STRING_LITERAL_HPP_INCLUDE_GUARD

#include <enek/feature_template/type.hpp>
#include <boost/range/iterator_range.hpp>
#include <filesystem>
#include <iosfwd>
#include <string>


namespace Enek::FeatureTemplate::Parsing{

class StringLiteral
{
public:
  using Path = std::filesystem::path;

public:
  StringLiteral() noexcept;

  StringLiteral(StringLiteral const &);

  StringLiteral(StringLiteral &&) noexcept;

  StringLiteral &operator=(StringLiteral const &) = delete;

  bool isInitialized() const noexcept;

  template<typename Iterator, typename BaseIterator>
  void initialize(boost::iterator_range<Iterator> const &parse_range,
                  Path const &path,
                  boost::iterator_range<BaseIterator> const &text_range,
                  std::ostream &os);

  bool succeed() const;

  Enek::FeatureTemplate::Type getType() const;

  std::string const &getValue() const;

  void dumpXML(std::ostream &) const;

private:
  bool initialized_;
  bool error_;
  std::string value_;
}; // class StringLiteral

} // namespace Enek::FeatureTemplate::Parsing

#endif // !defined(ENEK_FEATURE_TEMPLATE_PARSING_STRING_LITERAL_HPP_INCLUDE_GUARD)
