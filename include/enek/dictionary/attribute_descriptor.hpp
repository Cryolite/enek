#if !defined(ENEK_DICTIONARY_ATTRIBUTE_DESCRIPTOR_HPP_INCLUDE_GUARD)
#define ENEK_DICTIONARY_ATTRIBUTE_DESCRIPTOR_HPP_INCLUDE_GUARD

#include <enek/dictionary/fundamental.hpp>
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <functional>
#include <memory>
#include <cstddef>
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)


namespace Enek::Dictionary{

#if defined(ENEK_ENABLE_ASSERT_PARANOID)

class AttributeSet;

class AttributeDescriptor
{
private:
  friend class Enek::Dictionary::AttributeSet;

  AttributeDescriptor() noexcept;

  AttributeDescriptor(
    Enek::Dictionary::AttributeIndex index,
    std::shared_ptr<void const> const &validation_pointer) noexcept;

public:
  AttributeDescriptor(AttributeDescriptor const &rhs) noexcept;

  AttributeDescriptor(AttributeDescriptor &&rhs) noexcept;

  void swap(AttributeDescriptor &rhs) noexcept;

  AttributeDescriptor &operator=(AttributeDescriptor const &rhs) noexcept;

  AttributeDescriptor &operator=(AttributeDescriptor &&rhs) noexcept;

private:
  bool isIndexSingular_() const noexcept;

  bool isValidationPointerEmpty_() const noexcept;

private:
  bool isSingular() const noexcept;

  bool isInvalidated() const noexcept;

  std::shared_ptr<void const> getValidationPointer() const noexcept;

  Enek::Dictionary::AttributeIndex getIndex() const noexcept;

private:
  void assertComparableTo_(AttributeDescriptor const &rhs) const noexcept;

public:
  bool operator==(AttributeDescriptor const &rhs) const noexcept;

  bool operator!=(AttributeDescriptor const &rhs) const noexcept;

  bool operator<(AttributeDescriptor const &rhs) const noexcept;

  bool operator>(AttributeDescriptor const &rhs) const noexcept;

  bool operator<=(AttributeDescriptor const &rhs) const noexcept;

  bool operator>=(AttributeDescriptor const &rhs) const noexcept;

private:
  friend class std::hash<AttributeDescriptor>;

private:
  Enek::Dictionary::AttributeIndex index_;
  std::weak_ptr<void const> validation_pointer_;
}; // class AttributeDescriptor

void swap(AttributeDescriptor &lhs, AttributeDescriptor &rhs) noexcept;

} // namespace Enek::Dictionary

namespace std{

template<>
struct hash<Enek::Dictionary::AttributeDescriptor>
{
  std::size_t operator()(Enek::Dictionary::AttributeDescriptor const &k) const;
}; // struct hash<Enek::Dictionary::AttributeDescriptor>

} // namespace std

namespace Enek::Dictionary{

#else // defined(ENEK_ENABLE_ASSERT_PARANOID)

typedef AttributeIndex AttributeDescriptor;

#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)

} // namespace Enek::Dictionary

#endif // !defined(ENEK_DICTIONARY_ATTRIBUTE_DESCRIPTOR_HPP_INCLUDE_GUARD)
