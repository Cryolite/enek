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
    std::shared_ptr<Enek::Dictionary::WordIndex> const &p) noexcept;

private:
  bool isIndexSingular_() const noexcept;

  bool isWeakPointerEmpty_() const noexcept;

  bool isSingular_() const noexcept;

  bool isInvalidated_() const noexcept;

  void assertComparableTo_(WordDescriptor const &rhs) const noexcept;

public:
  WordDescriptor(WordDescriptor const &rhs) noexcept;

  WordDescriptor(WordDescriptor &&rhs) noexcept;

  void swap(WordDescriptor &rhs) noexcept;

  WordDescriptor &operator=(WordDescriptor const &rhs) noexcept;

  WordDescriptor &operator=(WordDescriptor &&rhs) noexcept;

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
  std::weak_ptr<Enek::Dictionary::WordIndex> p_;
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
