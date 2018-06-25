#if !defined(ENEK_DICTIONARY_FUNDAMENTAL_HPP_INCLUDE_GUARD)
#define ENEK_DICTIONARY_FUNDAMENTAL_HPP_INCLUDE_GUARD

#include <type_traits>
#include <cstdint>
#include <limits>


namespace Enek::Dictionary{

typedef std::uint32_t AttributeIndex;

constexpr AttributeIndex getSingularAttributeIndex() noexcept
{
  return std::numeric_limits<AttributeIndex>::max();
}

constexpr AttributeIndex getMaxNumAttributesInColumn() noexcept
{
  using AttributeIndexDifference = std::make_signed_t<AttributeIndex>;
  return std::numeric_limits<AttributeIndexDifference>::max();
}

typedef std::uint8_t ColumnIndex;

constexpr ColumnIndex getSingularColumnIndex() noexcept
{
  return std::numeric_limits<ColumnIndex>::max();
}

constexpr ColumnIndex getMaxNumColumns() noexcept
{
  using ColumnIndexDifference = std::make_signed_t<ColumnIndex>;
  return std::numeric_limits<ColumnIndexDifference>::max();
}

typedef std::uint32_t WordIndex;

constexpr WordIndex getSingularWordIndex() noexcept
{
  return std::numeric_limits<WordIndex>::max();
}

constexpr WordIndex getMaxNumWordsInDictionary() noexcept
{
  using WordIndexDifference = std::make_signed_t<WordIndex>;
  return std::numeric_limits<WordIndexDifference>::max();
}

} // namespace Enek::Dictionary

#endif // !defined(ENEK_DICTIONARY_FUNDAMENTAL_HPP_INCLUDE_GUARD)
