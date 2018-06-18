#if !defined(ENEK_UNICODE_CHAR_HPP_INCLUDE_GUARD)
#define ENEK_UNICODE_CHAR_HPP_INCLUDE_GUARD

#include <boost/io/ios_state.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <iomanip>
#include <ostream>
#include <ios>
#include <type_traits>


namespace Enek::Unicode{

char32_t getCharMaxValue() noexcept;

template<typename InputIterator>
class AsCodePointManipulator
{
public:
  AsCodePointManipulator(InputIterator first, InputIterator last)
    : first_(first)
    , last_(last)
  {}

  AsCodePointManipulator(AsCodePointManipulator const &) = delete;

  AsCodePointManipulator &operator=(AsCodePointManipulator const &) = delete;

  friend std::ostream &operator<<(
    std::ostream &os, AsCodePointManipulator &&rhs)
  {
    if (rhs.first_ == rhs.last_) {
      return os;
    }
    InputIterator iter = rhs.first_;
    {
      boost::io::ios_all_saver ias(os);
      os << "U+" << std::hex << std::uppercase << std::setw(4) << std::right
         << std::setfill('0') << *iter++;
    }
    for (; iter != rhs.last_; ++iter) {
      boost::io::ios_all_saver ias(os);
      os << ' ' << "U+" << std::hex << std::uppercase << std::setw(4)
         << std::right << std::setfill('0') << *iter;
    }
    return os;
  }

private:
  InputIterator first_;
  InputIterator last_;
}; // class AsCodePointManipulator

template<typename InputIterator>
AsCodePointManipulator<InputIterator>
formatAsCodePoint(InputIterator first, InputIterator last)
{
  static_assert(
    std::is_convertible_v<
      typename boost::iterator_value<InputIterator>::type, char32_t>);
  return { first, last };
}

inline AsCodePointManipulator<char32_t const *>
formatAsCodePoint(char32_t const &c)
{
  return { &c, &c + 1};
}

namespace GeneralCategory{

bool isOtherSurrogate(char32_t c) noexcept;

} // namespace GeneralCategory

enum struct EastAsianWidth
{
  fullwidth,
  halfwidth,
  wide,
  narrow,
  ambiguous,
  neutral
}; // enum struct

std::ostream &operator<<(std::ostream &os, EastAsianWidth eaw);

EastAsianWidth getEastAsianWidth(char32_t c);

} // namespace Enek::Unicode

#endif // !defined(ENEK_UNICODE_CHAR_HPP_INCLUDE_GUARD)
