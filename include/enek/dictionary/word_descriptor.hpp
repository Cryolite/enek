#if !defined(ENEK_DICTIONARY_WORD_DESCRIPTOR_HPP_INCLUDE_GUARD)
#define ENEK_DICTIONARY_WORD_DESCRIPTOR_HPP_INCLUDE_GUARD

#include <enek/dictionary/fundamental.hpp>
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <functional>
#include <memory>
#include <cstddef>
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)


namespace Enek::Dictionary{

#if defined(ENEK_ENABLE_ASSERT_PARANOID)

class Dictionary;

class WordDescriptor
{
private:
  friend class Enek::Dictionary::Dictionary;

  WordDescriptor() noexcept;

  WordDescriptor(
    Enek::Dictionary::WordIndex index,
    std::shared_ptr<void const> const &validation_pointer) noexcept;

public:
  WordDescriptor(WordDescriptor const &rhs) noexcept;

  WordDescriptor(WordDescriptor &&rhs) noexcept;

  void swap(WordDescriptor &rhs) noexcept;

  WordDescriptor &operator=(WordDescriptor const &rhs) noexcept;

  WordDescriptor &operator=(WordDescriptor &&rhs) noexcept;

private:
  bool isIndexSingular_() const noexcept;

  bool isValidationPointerEmpty_() const noexcept;

private:
  bool isSingular() const noexcept;

  bool isInvalidated() const noexcept;

  std::shared_ptr<void const> getValidationPointer() const noexcept;

  Enek::Dictionary::WordIndex getIndex() const noexcept;

private:
  void assertComparableTo_(WordDescriptor const &rhs) const noexcept;

public:
  bool operator==(WordDescriptor const &rhs) const noexcept;

  bool operator!=(WordDescriptor const &rhs) const noexcept;

  bool operator<(WordDescriptor const &rhs) const noexcept;

  bool operator>(WordDescriptor const &rhs) const noexcept;

  bool operator<=(WordDescriptor const &rhs) const noexcept;

  bool operator>=(WordDescriptor const &rhs) const noexcept;

private:
  friend class std::hash<WordDescriptor>;

private:
  Enek::Dictionary::WordIndex index_;
  std::weak_ptr<void const> validation_pointer_;
}; // class WordDescriptor

void swap(WordDescriptor &lhs, WordDescriptor &rhs) noexcept;

} // namespace Enek::Dictionary

namespace std{

template<>
struct hash<Enek::Dictionary::WordDescriptor>
{
  std::size_t operator()(Enek::Dictionary::WordDescriptor const &k) const;
}; // struct hash<Enek::Dictionary::WordDescriptor>

} // namespace std

namespace Enek::Dictionary{

#else // defined(ENEK_ENABLE_ASSERT_PARANOID)

typedef WordIndex WordDescriptor;

#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)

} // namespace Enek::Dictionary

#endif // !defined(ENEK_DICTIONARY_WORD_DESCRIPTOR_HPP_INCLUDE_GUARD)
